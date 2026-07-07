/*
  WRO Future Engineers - Arduino Mega
  Logica con 2 sensores:
  - Sensor frontal/centro
  - Sensor derecho

  Servo:
  25  = izquierda
  90  = recto
  130 = derecha

  Logica:
  - Si derecha libre > 50 cm, gira derecha.
  - Si derecha ocupada y frente <= 30 cm, gira izquierda.
  - Si frente <= 30 cm y derecha libre, gira derecha.
  - Una vez empieza un giro, NO vuelve a decidir hasta terminar.
*/

#include <Servo.h>

// ---------------- PINES ----------------

const int PIN_SERVO = 9;

// L298N
const int PIN_MOTOR_ENA = 5;
const int PIN_MOTOR_IN1 = 6;
const int PIN_MOTOR_IN2 = 7;

// Ultrasonico frontal
const int PIN_TRIG_FRONT = 22;
const int PIN_ECHO_FRONT = 23;

// Ultrasonico derecho
const int PIN_TRIG_RIGHT = 24;
const int PIN_ECHO_RIGHT = 25;

// ---------------- SERVO ----------------

const int SERVO_LEFT = 25;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT = 130;

// ---------------- VELOCIDADES ----------------

const int SPEED_STOP = 0;
const int SPEED_SLOW = 190;
const int SPEED_NORMAL = 180;
const int SPEED_TURN = 255;

// ---------------- DISTANCIAS ----------------

const float FRONT_TURN_CM = 80.0;
const float FRONT_CLEAR_AFTER_TURN_CM = 75.0;

const float RIGHT_FREE_CM = 80.0;
const float RIGHT_TOO_CLOSE_CM = 75.0;
const float RIGHT_TARGET_CM = 60.0;

const float MAX_DISTANCE_CM = 300.0;

const int RIGHT_FREE_CONFIRM_READS = 3;

// ---------------- TIEMPOS ----------------

const unsigned long SONAR_TIMEOUT_US = 12000;

// El giro no termina antes de este tiempo
const unsigned long MIN_TURN_MS = 1200;

// Seguridad: aunque el sensor no vea libre, termina al llegar aquí
const unsigned long MAX_TURN_MS = 1000;

// Tiempo de avance recto después del giro sin tomar decisiones
const unsigned long POST_TURN_STRAIGHT_MS = 950;

// ---------------- ESTADOS ----------------

enum RobotState {
  STRAIGHT,
  TURNING_LEFT,
  TURNING_RIGHT,
  POST_TURN_STRAIGHT
};

RobotState state = STRAIGHT;

Servo steeringServo;

float frontCm = 0;
float rightCm = 0;

float lastFrontCm = 0;
float lastRightCm = 0;

int rightFreeCount = 0;

// Evita repetir giro a la derecha mientras sigue viendo libre
bool rightWasBlockedSinceLastRightTurn = true;

unsigned long stateStartMs = 0;
unsigned long lastDebugMs = 0;

// ---------------- FUNCIONES BASE ----------------

void setState(RobotState nextState) {
  state = nextState;
  stateStartMs = millis();
}

void setServo(int angle) {
  angle = constrain(angle, SERVO_LEFT, SERVO_RIGHT);
  steeringServo.write(angle);
}

void motorStop() {
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  analogWrite(PIN_MOTOR_ENA, SPEED_STOP);
}

void motorForward(int speedPWM) {
  speedPWM = constrain(speedPWM, 0, 255);

  digitalWrite(PIN_MOTOR_IN1, HIGH);
  digitalWrite(PIN_MOTOR_IN2, LOW);

  analogWrite(PIN_MOTOR_ENA, speedPWM);
}

// ---------------- ULTRASONICOS ----------------

float readUltrasonicCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, SONAR_TIMEOUT_US);

  if (duration == 0) {
    return MAX_DISTANCE_CM;
  }

  float distance = duration * 0.0343 / 2.0;

  if (distance < 2 || distance > MAX_DISTANCE_CM) {
    return MAX_DISTANCE_CM;
  }

  return distance;
}

float filterDistance(float previous, float current) {
  if (previous <= 0) {
    return current;
  }

  if (current > previous + 70) {
    return previous * 0.80 + current * 0.20;
  }

  return previous * 0.65 + current * 0.35;
}

void readSensors() {
  float rawFront = readUltrasonicCm(PIN_TRIG_FRONT, PIN_ECHO_FRONT);
  delay(8);

  float rawRight = readUltrasonicCm(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT);

  frontCm = filterDistance(lastFrontCm, rawFront);
  rightCm = filterDistance(lastRightCm, rawRight);

  lastFrontCm = frontCm;
  lastRightCm = rightCm;
}

// ---------------- CORRECCION SUAVE ----------------

int rightWallFollowServoAngle() {
  int angle = SERVO_CENTER;

  if (rightCm < RIGHT_TOO_CLOSE_CM) {
    angle = SERVO_CENTER - 18;
  }

  else if (rightCm > RIGHT_TARGET_CM && rightCm <= RIGHT_FREE_CM) {
    angle = SERVO_CENTER + 15;
  }

  else {
    angle = SERVO_CENTER;
  }

  return constrain(angle, 65, 115);
}

// ---------------- INICIO DE GIROS ----------------

void beginTurnLeft() {
  rightFreeCount = 0;

  setServo(SERVO_LEFT);
  motorForward(SPEED_TURN);

  setState(TURNING_LEFT);
}

void beginTurnRight() {
  rightFreeCount = 0;

  // No permite otro giro a la derecha hasta volver a detectar algo a la derecha
  rightWasBlockedSinceLastRightTurn = false;

  setServo(SERVO_RIGHT);
  motorForward(SPEED_TURN);

  setState(TURNING_RIGHT);
}

// ---------------- LOGICA PRINCIPAL ----------------

void handleStraight() {
  bool rightIsFree = rightCm > RIGHT_FREE_CM;
  bool rightIsBlocked = rightCm <= RIGHT_FREE_CM;
  bool frontSeesWall = frontCm <= FRONT_TURN_CM;

  // Si el derecho volvió a detectar pared/cuadrado, habilita otro giro derecho futuro
  if (rightIsBlocked) {
    rightWasBlockedSinceLastRightTurn = true;
    rightFreeCount = 0;
  } else {
    rightFreeCount++;
  }

  /*
    PRIORIDAD 1:
    Si el frontal detecta pared, decide usando el derecho.
  */
  if (frontSeesWall) {
    if (rightIsFree) {
      beginTurnRight();
      return;
    } else {
      beginTurnLeft();
      return;
    }
  }

  /*
    PRIORIDAD 2:
    Si el derecho está libre por varias lecturas y antes estuvo bloqueado,
    gira a la derecha.
  */
  if (
    rightFreeCount >= RIGHT_FREE_CONFIRM_READS &&
    rightWasBlockedSinceLastRightTurn
  ) {
    beginTurnRight();
    return;
  }

  // Avance normal
  int steering = rightWallFollowServoAngle();
  setServo(steering);
  motorForward(SPEED_NORMAL);
}

// ---------------- GIRO IZQUIERDA ----------------

void handleTurningLeft() {
  // Aquí NO se decide nada.
  // Solo mantiene el giro hasta terminar.
  setServo(SERVO_LEFT);
  motorForward(SPEED_TURN);

  unsigned long elapsed = millis() - stateStartMs;

  bool minTimePassed = elapsed >= MIN_TURN_MS;
  bool frontIsClear = frontCm > FRONT_CLEAR_AFTER_TURN_CM;
  bool maxTimePassed = elapsed >= MAX_TURN_MS;

  if ((minTimePassed && frontIsClear) || maxTimePassed) {
    setServo(SERVO_CENTER);
    setState(POST_TURN_STRAIGHT);
  }
}

// ---------------- GIRO DERECHA ----------------

void handleTurningRight() {
  // Aquí NO se decide nada.
  // Solo mantiene el giro hasta terminar.
  setServo(SERVO_RIGHT);
  motorForward(SPEED_TURN);

  unsigned long elapsed = millis() - stateStartMs;

  bool minTimePassed = elapsed >= MIN_TURN_MS;
  bool frontIsClear = frontCm > FRONT_CLEAR_AFTER_TURN_CM;
  bool maxTimePassed = elapsed >= MAX_TURN_MS;

  if ((minTimePassed && frontIsClear) || maxTimePassed) {
    setServo(SERVO_CENTER);
    setState(POST_TURN_STRAIGHT);
  }
}

// ---------------- SALIDA DEL GIRO ----------------

void handlePostTurnStraight() {
  /*
    Después de girar, avanza recto un momento.
    Durante este estado NO se permite iniciar otro giro.
    Esto evita que se encadene/repite el giro.
  */

  setServo(SERVO_CENTER);
  motorForward(SPEED_SLOW);

  if (millis() - stateStartMs >= POST_TURN_STRAIGHT_MS) {
    rightFreeCount = 0;
    setState(STRAIGHT);
  }
}

// ---------------- DEBUG ----------------

void debugPrint() {
  if (millis() - lastDebugMs < 300) return;
  lastDebugMs = millis();

  Serial.print("Front: ");
  Serial.print(frontCm);
  Serial.print(" cm | Right: ");
  Serial.print(rightCm);
  Serial.print(" cm | Estado: ");

  if (state == STRAIGHT) Serial.print("STRAIGHT");
  else if (state == TURNING_LEFT) Serial.print("TURNING_LEFT");
  else if (state == TURNING_RIGHT) Serial.print("TURNING_RIGHT");
  else if (state == POST_TURN_STRAIGHT) Serial.print("POST_TURN_STRAIGHT");

  Serial.print(" | RightFreeCount: ");
  Serial.print(rightFreeCount);

  Serial.print(" | RightReady: ");
  Serial.println(rightWasBlockedSinceLastRightTurn ? "YES" : "NO");
}

// ---------------- SETUP ----------------

void setup() {
  Serial.begin(9600);

  pinMode(PIN_MOTOR_ENA, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  pinMode(PIN_TRIG_FRONT, OUTPUT);
  pinMode(PIN_ECHO_FRONT, INPUT);

  pinMode(PIN_TRIG_RIGHT, OUTPUT);
  pinMode(PIN_ECHO_RIGHT, INPUT);

  steeringServo.attach(PIN_SERVO);

  setServo(SERVO_CENTER);
  motorStop();

  delay(1000);

  for (int i = 0; i < 10; i++) {
    readSensors();
    delay(50);
  }

  rightFreeCount = 0;
  rightWasBlockedSinceLastRightTurn = true;

  setState(STRAIGHT);

  Serial.println("Robot iniciado. Giros bloqueados hasta terminar.");
}

// ---------------- LOOP ----------------

void loop() {
  readSensors();

  switch (state) {
    case STRAIGHT:
      handleStraight();
      break;

    case TURNING_LEFT:
      handleTurningLeft();
      break;

    case TURNING_RIGHT:
      handleTurningRight();
      break;

    case POST_TURN_STRAIGHT:
      handlePostTurnStraight();
      break;
  }

  debugPrint();
}
