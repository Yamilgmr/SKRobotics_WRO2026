#include <Servo.h>
#include <NewPing.h>

/*
  SKRobotics WRO 2026 - Open Challenge starter firmware

  Hardware baseline:
  - Arduino Mega 2560
  - 3 ultrasonic sensors: front, left, right
  - AD002 steering servo
  - DC motor through an L298N motor driver

  The motor output is disabled by default until the L298N wiring, common ground,
  and wheel-lift safety test are verified.
*/

const bool MOTOR_OUTPUT_ENABLED = false;

// ---------------- Pin map ----------------
const uint8_t SERVO_PIN = 6;
const uint8_t MOTOR_PWM_PIN = 5;
const uint8_t MOTOR_DIR_PIN = 4;

const uint8_t US_FRONT_TRIG = 22;
const uint8_t US_FRONT_ECHO = 23;
const uint8_t US_LEFT_TRIG = 24;
const uint8_t US_LEFT_ECHO = 25;
const uint8_t US_RIGHT_TRIG = 26;
const uint8_t US_RIGHT_ECHO = 27;

const uint8_t START_BUTTON_PIN = A0;
const uint8_t STATUS_LED_PIN = 13;

// ---------------- Sensor and actuator constants ----------------
const unsigned int MAX_DISTANCE_CM = 200;
const int SERVO_CENTER = 90;
const int SERVO_LEFT_LIMIT = 55;
const int SERVO_RIGHT_LIMIT = 125;

// Positive steering offset means right, negative means left.
const int TURN_DIRECTION = -1;
const int TURN_SERVO_OFFSET = 30;

// ---------------- Open Challenge tuning constants ----------------
const int BASE_SPEED_PWM = 120;
const int TARGET_SIDE_DISTANCE_CM = 30;
const int FRONT_PREFIRE_CM = 38;
const int SIDE_CORRECTION_GAIN = 2;
const unsigned long TURN_HOLD_MS = 520;
const unsigned long RECENTER_MS = 300;
const uint8_t TOTAL_TURNS_TARGET = 12;
const bool FOLLOW_LEFT_WALL = true;

// ---------------- Objects ----------------
Servo steeringServo;
NewPing sonarFront(US_FRONT_TRIG, US_FRONT_ECHO, MAX_DISTANCE_CM);
NewPing sonarLeft(US_LEFT_TRIG, US_LEFT_ECHO, MAX_DISTANCE_CM);
NewPing sonarRight(US_RIGHT_TRIG, US_RIGHT_ECHO, MAX_DISTANCE_CM);

enum RobotState {
  WAIT_FOR_START,
  FOLLOW_WALL,
  PREFIRE_TURN,
  RECENTER,
  FINISHED
};

RobotState state = WAIT_FOR_START;
unsigned long stateStartedAt = 0;
unsigned long lastDebugAt = 0;
uint8_t turnCount = 0;

void setup() {
  Serial.begin(115200);

  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);
  pinMode(STATUS_LED_PIN, OUTPUT);

  steeringServo.attach(SERVO_PIN);
  writeSteering(SERVO_CENTER);
  stopMotor();

  digitalWrite(STATUS_LED_PIN, HIGH);
  Serial.println("SKRobotics Open Challenge firmware ready.");
  Serial.println("Motor output is disabled until MOTOR_OUTPUT_ENABLED is set to true.");
}

void loop() {
  switch (state) {
    case WAIT_FOR_START:
      handleWaitForStart();
      break;
    case FOLLOW_WALL:
      handleFollowWall();
      break;
    case PREFIRE_TURN:
      handlePrefireTurn();
      break;
    case RECENTER:
      handleRecenter();
      break;
    case FINISHED:
      handleFinished();
      break;
  }

  debugOutput();
}

void handleWaitForStart() {
  stopMotor();
  writeSteering(SERVO_CENTER);

  if (digitalRead(START_BUTTON_PIN) == LOW) {
    delay(250);
    digitalWrite(STATUS_LED_PIN, LOW);
    transitionTo(FOLLOW_WALL);
  }
}

void handleFollowWall() {
  setMotor(BASE_SPEED_PWM);

  int steering = calculateWallFollowSteering();
  writeSteering(steering);

  int frontCm = readDistanceCm(sonarFront);
  if (isValidDistance(frontCm) && frontCm <= FRONT_PREFIRE_CM) {
    turnCount++;
    transitionTo(PREFIRE_TURN);
  }
}

void handlePrefireTurn() {
  setMotor(BASE_SPEED_PWM);
  int turnSteering = SERVO_CENTER + (TURN_DIRECTION * TURN_SERVO_OFFSET);
  writeSteering(turnSteering);

  if (millis() - stateStartedAt >= TURN_HOLD_MS) {
    transitionTo(RECENTER);
  }
}

void handleRecenter() {
  setMotor(BASE_SPEED_PWM);
  writeSteering(calculateWallFollowSteering());

  if (millis() - stateStartedAt >= RECENTER_MS) {
    if (turnCount >= TOTAL_TURNS_TARGET) {
      transitionTo(FINISHED);
    } else {
      transitionTo(FOLLOW_WALL);
    }
  }
}

void handleFinished() {
  stopMotor();
  writeSteering(SERVO_CENTER);
  digitalWrite(STATUS_LED_PIN, HIGH);
}

void transitionTo(RobotState nextState) {
  state = nextState;
  stateStartedAt = millis();

  Serial.print("State -> ");
  Serial.print(stateName(state));
  Serial.print(" | turns=");
  Serial.println(turnCount);
}

int calculateWallFollowSteering() {
  NewPing &sideSensor = FOLLOW_LEFT_WALL ? sonarLeft : sonarRight;
  int sideCm = readDistanceCm(sideSensor);

  if (!isValidDistance(sideCm)) {
    return SERVO_CENTER;
  }

  int error = TARGET_SIDE_DISTANCE_CM - sideCm;
  int correction = constrain(error * SIDE_CORRECTION_GAIN, -25, 25);

  if (FOLLOW_LEFT_WALL) {
    return SERVO_CENTER + correction;
  }

  return SERVO_CENTER - correction;
}

int readDistanceCm(NewPing &sonar) {
  const uint8_t samples = 5;
  const int minValid = 3;
  const int maxValid = 150;
  int values[samples];
  uint8_t validCount = 0;
  int sum = 0;

  for (uint8_t i = 0; i < samples; i++) {
    int cm = sonar.ping_cm();
    delay(3);
    if (cm >= minValid && cm <= maxValid) {
      values[validCount++] = cm;
      sum += cm;
    }
  }

  if (validCount == 0) {
    return 0;
  }

  int average = sum / validCount;
  int filteredSum = 0;
  uint8_t filteredCount = 0;

  for (uint8_t i = 0; i < validCount; i++) {
    if (abs(values[i] - average) <= 10) {
      filteredSum += values[i];
      filteredCount++;
    }
  }

  if (filteredCount == 0) {
    return 0;
  }

  return filteredSum / filteredCount;
}

bool isValidDistance(int cm) {
  return cm > 0;
}

void writeSteering(int angle) {
  angle = constrain(angle, SERVO_LEFT_LIMIT, SERVO_RIGHT_LIMIT);
  steeringServo.write(angle);
}

void setMotor(int pwm) {
  pwm = constrain(pwm, -255, 255);

  if (!MOTOR_OUTPUT_ENABLED) {
    stopMotor();
    return;
  }

  digitalWrite(MOTOR_DIR_PIN, pwm >= 0 ? HIGH : LOW);
  analogWrite(MOTOR_PWM_PIN, abs(pwm));
}

void stopMotor() {
  analogWrite(MOTOR_PWM_PIN, 0);
  digitalWrite(MOTOR_DIR_PIN, LOW);
}

void debugOutput() {
  if (millis() - lastDebugAt < 250) {
    return;
  }

  lastDebugAt = millis();
  Serial.print("state=");
  Serial.print(stateName(state));
  Serial.print(" turns=");
  Serial.print(turnCount);
  Serial.print(" front=");
  Serial.print(readDistanceCm(sonarFront));
  Serial.print(" left=");
  Serial.print(readDistanceCm(sonarLeft));
  Serial.print(" right=");
  Serial.println(readDistanceCm(sonarRight));
}

const char *stateName(RobotState s) {
  switch (s) {
    case WAIT_FOR_START: return "WAIT_FOR_START";
    case FOLLOW_WALL: return "FOLLOW_WALL";
    case PREFIRE_TURN: return "PREFIRE_TURN";
    case RECENTER: return "RECENTER";
    case FINISHED: return "FINISHED";
  }

  return "UNKNOWN";
}

