/*
  WRO Future Engineers 2026 - Open Challenge
  Robot: Olibot Ackermann + Arduino Mega 2560 + MG996R + L298N
  Sensors: 3 x HC-SR04 (front, right, left)

  IMPORTANT
  - No start button or initial delay: starts moving immediately.
  - 3 laps = 12 completed corners. CORNERS_TO_FINISH must remain 12.
  - Side sensors are the primary corner detectors.
  - The front sensor is only a secondary corner cue and collision guard.
  - All physical calibration values are grouped at the top.
  - This program intentionally uses only the standard Servo library.
*/

#include <Servo.h>

// ============================================================================
// 1. PIN MAP
// ============================================================================
const uint8_t PIN_SERVO = 9;
const uint8_t PIN_ENA = 5;   // PWM: must keep the ENA jumper removed
const uint8_t PIN_IN1 = 6;
const uint8_t PIN_IN2 = 7;

const uint8_t PIN_FRONT_TRIG = 42;
const uint8_t PIN_FRONT_ECHO = 43;
const uint8_t PIN_RIGHT_TRIG = 46;
const uint8_t PIN_RIGHT_ECHO = 47;
const uint8_t PIN_LEFT_TRIG  = 52;
const uint8_t PIN_LEFT_ECHO  = 53;

// Change these two levels only if the rear motor turns backwards.
const uint8_t MOTOR_FORWARD_IN1 = HIGH;
const uint8_t MOTOR_FORWARD_IN2 = LOW;

// ============================================================================
// 2. SERVO CALIBRATION
// ============================================================================
// Solo tres posiciones fisicas. El servo no cambia de angulo dentro del giro.
const int16_t SERVO_LEFT_DEG        = 10;
const int16_t SERVO_CENTER_DEG      = 90;
const int16_t SERVO_RIGHT_DEG       = 120;
const int16_t SERVO_MIN_DEG         = 10;
const int16_t SERVO_MAX_DEG         = 120;

// Reverse this if a smaller servo angle physically turns the wheels right.
const bool SERVO_SMALL_ANGLE_IS_LEFT = true;

// ============================================================================
// 3. MOTOR CALIBRATION (0..255)
// ============================================================================
const uint8_t START_PWM    = 255; // Salida fuerte sin usar siempre el maximo.
const uint8_t CRUISE_PWM   = 200; // Un poco mas rapido sin usar 255 en recta.
const uint8_t DECISION_PWM = 190; // Rapido mientras decide el primer sentido.
const uint8_t TURN_PWM     = 245;
const uint8_t ALIGN_PWM    = 205; // Estabiliza al centrar las ruedas.
const uint8_t EXIT_PWM     = 245; // Recupera velocidad al salir.
const uint8_t FINAL_PWM    = 205; // Recta final controlada antes de frenar.

// Soft ramp protects the gearbox and supply from abrupt PWM steps.
const uint8_t PWM_RAMP_STEP = 10;
const uint16_t PWM_RAMP_INTERVAL_MS = 8;

// ============================================================================
// 4. TRACK AND ULTRASONIC CALIBRATION
// ============================================================================
const float TRACK_CORRIDOR_CM = 100.0f;
const float SENSOR_OUTER_SPAN_CM = 10.1f;
// The official Open Challenge can use a corridor near 100 cm or 60 cm. This
// starts at 100 cm, then updates automatically whenever both walls are seen.
float estimatedCorridorCm = TRACK_CORRIDOR_CM;

const uint16_t SONAR_MAX_CM = 220;
const uint16_t SONAR_TIMEOUT_US = 13000; // approximately 220 cm
const uint16_t PING_GAP_MS = 18;

const uint16_t SIDE_TURN_TRIGGER_CM = 68;
const uint16_t SIDE_WALL_VISIBLE_CM = 60;
// El giro derecho ya esta calibrado. El izquierdo acepta antes la pared nueva
// porque con la direccion a 10 grados cruza la esquina mucho mas rapidamente.
const uint16_t LEFT_WALL_REACQUIRED_CM  = 70;
const uint16_t RIGHT_WALL_REACQUIRED_CM = 50;
const uint16_t WALL_PRESENT_CM = 62;
const uint16_t MIN_USABLE_CM = 3;

const uint16_t FRONT_COUNT_CM = 65;

// One lateral opening starts the steering immediately. Duplicate counting is
// prevented by cornerArmed and by wall reacquisition after every turn.
// Una sola lectura lateral valida mayor de 100 cm inicia el giro inmediatamente.
const uint8_t OPEN_CONFIRM_READS = 1;
const uint8_t LEFT_WALL_CONFIRM_READS  = 1;
const uint8_t RIGHT_WALL_CONFIRM_READS = 2;
const uint8_t FRONT_COUNT_CONFIRM_READS = 2;

// Straight-line correction. Keep this intentionally small without an IMU.
const float STEERING_KP_DEG_PER_CM = 0.24f;
const int8_t MAX_STRAIGHT_CORRECTION_DEG = 10;
const uint8_t STEERING_DEADBAND_CM = 3;

// ============================================================================
// 5. MANEUVER TIMING
// ============================================================================
// No compartir los tiempos: el servo izquierdo (10 grados) gira mucho mas
// cerrado que el derecho (120 grados). Se conserva intacto el giro horario.
const uint16_t MIN_LEFT_TURN_MS  = 450;
const uint16_t MAX_LEFT_TURN_MS  = 2250;
const uint16_t MIN_RIGHT_TURN_MS = 450;
const uint16_t MAX_RIGHT_TURN_MS = 2300;
const uint16_t ALIGN_MS = 220;
const uint16_t EXIT_MS = 200;

const uint8_t CORNERS_TO_FINISH = 12; // 4 corners x 3 laps
// Distance after the 12th corner. Calibrate this to stop inside the start area.
const uint16_t FINAL_RUN_MS = 850;
const uint16_t FINAL_BRAKE_MS = 350;

const uint16_t DEBUG_INTERVAL_MS = 250;
const uint16_t INITIAL_TURN_GUARD_MS = 300;

// ============================================================================
// 6. TYPES AND STATE
// ============================================================================
enum Direction : int8_t {
  DIR_UNKNOWN = 0,
  DIR_LEFT = -1,
  DIR_RIGHT = 1
};

enum DriveState : uint8_t {
  STATE_STARTUP,
  STATE_STRAIGHT,
  STATE_TURN,
  STATE_ALIGN,
  STATE_EXIT,
  STATE_FINAL_RUN,
  STATE_FINAL_BRAKE,
  STATE_STOPPED
};

enum SensorId : uint8_t {
  SENSOR_LEFT,
  SENSOR_RIGHT,
  SENSOR_FRONT
};

struct SonarReading {
  uint16_t rawCm;
  float filteredCm;
  uint8_t openHits;
  uint8_t wallHits;
  uint8_t criticalHits;
  bool initialized;
  unsigned long updatedAt;
};

Servo steering;
SonarReading sonarLeft  = {SONAR_MAX_CM, SONAR_MAX_CM, 0, 0, 0, false, 0};
SonarReading sonarRight = {SONAR_MAX_CM, SONAR_MAX_CM, 0, 0, 0, false, 0};
SonarReading sonarFront = {SONAR_MAX_CM, SONAR_MAX_CM, 0, 0, 0, false, 0};

Direction direction = DIR_UNKNOWN;
DriveState state = STATE_STARTUP;

uint8_t completedCorners = 0;
bool cornerArmed = true;
bool turnSawOpenSide = false;
uint8_t frontCountHits = 0;
bool frontCornerSeen = false;

unsigned long stateStartedAt = 0;
unsigned long lastCornerCompletedAt = 0;
unsigned long nextPingAt = 0;
unsigned long lastDebugAt = 0;
unsigned long lastPwmRampAt = 0;
unsigned long raceStartedAt = 0;

uint8_t sensorSequenceIndex = 0;
uint8_t motorCurrentPwm = 0;
uint8_t motorTargetPwm = 0;
int16_t lastServoCommand = -999;
bool leftSensorSampled = false;
bool rightSensorSampled = false;

// ============================================================================
// 7. LOW-LEVEL MOTOR AND SERVO
// ============================================================================
int16_t clampServo(int16_t angle) {
  return constrain(angle, SERVO_MIN_DEG, SERVO_MAX_DEG);
}

void setSteering(int16_t angle) {
  int16_t command = clampServo(angle);
  // No reenviar el mismo angulo continuamente: el servo solo recibe una orden
  // al entrar al giro y otra al volver al centro.
  if (command == lastServoCommand) return;
  steering.write(command);
  lastServoCommand = command;
}

int16_t leftServoAngle(int16_t calibratedAngle) {
  if (SERVO_SMALL_ANGLE_IS_LEFT) return calibratedAngle;
  return SERVO_CENTER_DEG + (SERVO_CENTER_DEG - calibratedAngle);
}

int16_t rightServoAngle(int16_t calibratedAngle) {
  if (SERVO_SMALL_ANGLE_IS_LEFT) return calibratedAngle;
  return SERVO_CENTER_DEG - (calibratedAngle - SERVO_CENTER_DEG);
}

void setMotorTarget(uint8_t pwm) {
  motorTargetPwm = pwm;
}

void enableForwardDirection() {
  digitalWrite(PIN_IN1, MOTOR_FORWARD_IN1);
  digitalWrite(PIN_IN2, MOTOR_FORWARD_IN2);
}

void updateMotorRamp(unsigned long now) {
  if (now - lastPwmRampAt < PWM_RAMP_INTERVAL_MS) return;
  lastPwmRampAt = now;

  if (motorCurrentPwm < motorTargetPwm) {
    uint16_t next = motorCurrentPwm + PWM_RAMP_STEP;
    motorCurrentPwm = (next > motorTargetPwm) ? motorTargetPwm : next;
  } else if (motorCurrentPwm > motorTargetPwm) {
    int16_t next = motorCurrentPwm - PWM_RAMP_STEP;
    motorCurrentPwm = (next < motorTargetPwm) ? motorTargetPwm : next;
  }

  enableForwardDirection();
  analogWrite(PIN_ENA, motorCurrentPwm);
}

void motorCoastStop() {
  motorTargetPwm = 0;
  motorCurrentPwm = 0;
  analogWrite(PIN_ENA, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
}

void motorActiveBrake() {
  motorTargetPwm = 0;
  motorCurrentPwm = 0;
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENA, 255);
}

// ============================================================================
// 8. ULTRASONIC ACQUISITION
// ============================================================================
uint16_t readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, SONAR_TIMEOUT_US);
  // Un timeout no se usa para decidir: evita giros falsos por sensor sin eco.
  if (duration == 0) return 0;

  uint16_t cm = (uint16_t)(duration / 58UL);
  if (cm < MIN_USABLE_CM) return MIN_USABLE_CM;
  if (cm > SONAR_MAX_CM) return SONAR_MAX_CM;
  return cm;
}

void updateReading(SonarReading &reading, uint16_t cm, bool isFront) {
  reading.rawCm = cm;
  reading.updatedAt = millis();

  if (cm == 0) {
    reading.openHits = 0;
    reading.wallHits = 0;
    reading.criticalHits = 0;
    if (isFront && !frontCornerSeen) frontCountHits = 0;
    return;
  }

  if (!reading.initialized) {
    reading.filteredCm = cm;
    reading.initialized = true;
  } else {
    // Fast enough for openings, but calmer than a raw reading for steering.
    reading.filteredCm = (reading.filteredCm * 0.60f) + (cm * 0.40f);
  }

  if (!isFront) {
    reading.openHits = (cm > SIDE_TURN_TRIGGER_CM)
        ? (uint8_t)min(255, reading.openHits + 1) : 0;
    const uint16_t wallLimit = (&reading == &sonarLeft)
        ? LEFT_WALL_REACQUIRED_CM : RIGHT_WALL_REACQUIRED_CM;
    reading.wallHits = (cm >= MIN_USABLE_CM && cm <= wallLimit)
        ? (uint8_t)min(255, reading.wallHits + 1) : 0;
  } else {
    bool countWindow = (state == STATE_STRAIGHT && cornerArmed) ||
                       state == STATE_TURN;
    if (countWindow && cm <= FRONT_COUNT_CM) {
      frontCountHits = (uint8_t)min(255, frontCountHits + 1);
      if (frontCountHits >= FRONT_COUNT_CONFIRM_READS)
        frontCornerSeen = true;
    } else if (!frontCornerSeen) {
      frontCountHits = 0;
    }
  }
}

SensorId nextSensorToRead() {
  // En recta la apertura tiene prioridad absoluta: no intercalar el frontal.
  // Cada lateral se actualiza cada dos pings.
  if (state == STATE_STRAIGHT || state == STATE_STARTUP) {
    SensorId id = (sensorSequenceIndex % 2 == 0) ? SENSOR_LEFT : SENSOR_RIGHT;
    sensorSequenceIndex = (sensorSequenceIndex + 1) % 2;
    return id;
  }

  // Durante el giro antihorario se lee solamente el sensor interior. Asi la
  // pared nueva se detecta cada 18 ms y no cuando el robot ya esta encima.
  // El muestreo del giro horario se deja igual porque ya funciona bien.
  if (state == STATE_TURN && direction == DIR_LEFT)
    return SENSOR_LEFT;

  // Before direction is known, sample both sides equally and front sometimes.
  static const SensorId unknownSequence[5] = {
    SENSOR_LEFT, SENSOR_RIGHT, SENSOR_LEFT, SENSOR_RIGHT, SENSOR_FRONT
  };

  // Once known, prioritize the inside/turning side.
  static const SensorId rightSequence[4] = {
    SENSOR_RIGHT, SENSOR_LEFT, SENSOR_RIGHT, SENSOR_FRONT
  };
  static const SensorId leftSequence[4] = {
    SENSOR_LEFT, SENSOR_RIGHT, SENSOR_LEFT, SENSOR_FRONT
  };

  if (direction == DIR_UNKNOWN) {
    SensorId id = unknownSequence[sensorSequenceIndex % 5];
    sensorSequenceIndex = (sensorSequenceIndex + 1) % 5;
    return id;
  }

  const SensorId *sequence = (direction == DIR_RIGHT)
      ? rightSequence : leftSequence;
  SensorId id = sequence[sensorSequenceIndex % 4];
  sensorSequenceIndex = (sensorSequenceIndex + 1) % 4;
  return id;
}

void updateOneSonar(unsigned long now) {
  if ((long)(now - nextPingAt) < 0) return;
  nextPingAt = now + PING_GAP_MS;

  switch (nextSensorToRead()) {
    case SENSOR_LEFT:
      updateReading(sonarLeft,
          readUltrasonicCm(PIN_LEFT_TRIG, PIN_LEFT_ECHO), false);
      leftSensorSampled = true;
      break;
    case SENSOR_RIGHT:
      updateReading(sonarRight,
          readUltrasonicCm(PIN_RIGHT_TRIG, PIN_RIGHT_ECHO), false);
      rightSensorSampled = true;
      break;
    case SENSOR_FRONT:
      updateReading(sonarFront,
          readUltrasonicCm(PIN_FRONT_TRIG, PIN_FRONT_ECHO), true);
      break;
  }
}

SonarReading &turnSideReading() {
  return (direction == DIR_LEFT) ? sonarLeft : sonarRight;
}

// ============================================================================
// 9. DECISION AND STEERING LOGIC
// ============================================================================
bool readingFresh(const SonarReading &reading, unsigned long now,
                  uint16_t maxAgeMs = 260) {
  return reading.initialized && reading.rawCm >= MIN_USABLE_CM &&
         (now - reading.updatedAt <= maxAgeMs);
}

Direction detectedOpening(unsigned long now) {
  // Evita que el sensor izquierdo gane solo por ser el primero de la secuencia.
  // Esta espera ocurre una unica vez al encender (aprox. 40 ms).
  if (!leftSensorSampled || !rightSensorSampled) return DIR_UNKNOWN;
  if (direction == DIR_UNKNOWN && now - raceStartedAt < INITIAL_TURN_GUARD_MS)
    return DIR_UNKNOWN;

  bool leftOpen = readingFresh(sonarLeft, now) &&
                  sonarLeft.openHits >= OPEN_CONFIRM_READS;
  bool rightOpen = readingFresh(sonarRight, now) &&
                   sonarRight.openHits >= OPEN_CONFIRM_READS;

  // La primera apertura fija el sentido. Luego el sensor exterior no puede
  // cambiarlo accidentalmente durante una recta.
  if (direction == DIR_LEFT) return leftOpen ? DIR_LEFT : DIR_UNKNOWN;
  if (direction == DIR_RIGHT) return rightOpen ? DIR_RIGHT : DIR_UNKNOWN;

  if (leftOpen && !rightOpen) return DIR_LEFT;
  if (rightOpen && !leftOpen) return DIR_RIGHT;
  if (leftOpen && rightOpen) {
    if (direction != DIR_UNKNOWN) return direction;
    int16_t difference = (int16_t)sonarLeft.rawCm -
                         (int16_t)sonarRight.rawCm;
    if (difference >= 15) return DIR_LEFT;
    if (difference <= -15) return DIR_RIGHT;
  }
  return DIR_UNKNOWN;
}

void applyStraightCorrection(unsigned long now) {
  (void)now;
  setSteering(SERVO_CENTER_DEG);
}

void beginTurn(unsigned long now) {
  if (direction == DIR_UNKNOWN || state != STATE_STRAIGHT || !cornerArmed) return;

  state = STATE_TURN;
  stateStartedAt = now;
  cornerArmed = false;
  turnSawOpenSide = true;

  // Discard wall hits collected on the previous straight. A new wall must be
  // acquired after the turn has genuinely begun.
  turnSideReading().wallHits = 0;

  setMotorTarget(TURN_PWM);
  if (direction == DIR_LEFT)
    setSteering(leftServoAngle(SERVO_LEFT_DEG));
  else
    setSteering(rightServoAngle(SERVO_RIGHT_DEG));

  Serial.print(F("Corner start. Next count="));
  Serial.println(completedCorners + 1);
}

void finishTurn(unsigned long now) {
  state = STATE_ALIGN;
  stateStartedAt = now;
  completedCorners++;
  frontCornerSeen = false;
  frontCountHits = 0;

  setMotorTarget(ALIGN_PWM);
  setSteering(SERVO_CENTER_DEG);

  Serial.print(F("Corner completed: "));
  Serial.print(completedCorners);
  Serial.println(F("/12"));
  if ((completedCorners % 4) == 0) {
    Serial.print(F("Lap completed: "));
    Serial.println(completedCorners / 4);
  }
}

void handleStraight(unsigned long now) {
  Direction opening = detectedOpening(now);
  if (cornerArmed && opening != DIR_UNKNOWN) {
    direction = opening;
    sensorSequenceIndex = 0;
    beginTurn(now);
    return;
  }

  applyStraightCorrection(now);

  // Nunca detenerse por una lectura frontal durante la carrera.
  if (direction == DIR_UNKNOWN) {
    setMotorTarget(DECISION_PWM);
  } else {
    setMotorTarget(CRUISE_PWM);
  }
}

void handleTurn(unsigned long now) {
  unsigned long elapsed = now - stateStartedAt;

  if (turnSideReading().openHits >= OPEN_CONFIRM_READS)
    turnSawOpenSide = true;

  const uint16_t minTurnMs = (direction == DIR_LEFT)
      ? MIN_LEFT_TURN_MS : MIN_RIGHT_TURN_MS;
  const uint16_t maxTurnMs = (direction == DIR_LEFT)
      ? MAX_LEFT_TURN_MS : MAX_RIGHT_TURN_MS;
  const uint8_t wallConfirmReads = (direction == DIR_LEFT)
      ? LEFT_WALL_CONFIRM_READS : RIGHT_WALL_CONFIRM_READS;

  bool newWallConfirmed = turnSideReading().wallHits >= wallConfirmReads;
  if ((elapsed >= minTurnMs && turnSawOpenSide && newWallConfirmed) ||
      elapsed >= maxTurnMs) {
    finishTurn(now);
  }
}

void updateStateMachine(unsigned long now) {
  switch (state) {
    case STATE_STARTUP:
      setSteering(SERVO_CENTER_DEG);
      state = STATE_STRAIGHT;
      stateStartedAt = now;
      setMotorTarget(START_PWM);
      break;

    case STATE_STRAIGHT:
      handleStraight(now);
      break;

    case STATE_TURN:
      handleTurn(now);
      break;

    case STATE_ALIGN:
      if (now - stateStartedAt >= ALIGN_MS) {
        state = STATE_EXIT;
        stateStartedAt = now;
        setSteering(SERVO_CENTER_DEG);
        setMotorTarget(EXIT_PWM);
      }
      break;

    case STATE_EXIT:
      if (now - stateStartedAt >= EXIT_MS) {
        lastCornerCompletedAt = now;
        turnSideReading().wallHits = 0;

        if (completedCorners >= CORNERS_TO_FINISH) {
          state = STATE_FINAL_RUN;
          stateStartedAt = now;
          setSteering(SERVO_CENTER_DEG);
          setMotorTarget(FINAL_PWM);
        } else {
          // Rearme directo al terminar la salida; no exige pared ni confirmacion.
          cornerArmed = true;
          sonarLeft.openHits = 0;
          sonarRight.openHits = 0;
          state = STATE_STRAIGHT;
          stateStartedAt = now;
          setMotorTarget(CRUISE_PWM);
        }
      }
      break;

    case STATE_FINAL_RUN:
      // Keep a small centering correction during the final straight.
      applyStraightCorrection(now);
      if (now - stateStartedAt >= FINAL_RUN_MS) {
        state = STATE_FINAL_BRAKE;
        stateStartedAt = now;
        setSteering(SERVO_CENTER_DEG);
        motorActiveBrake();
      }
      break;

    case STATE_FINAL_BRAKE:
      if (now - stateStartedAt >= FINAL_BRAKE_MS) {
        motorCoastStop();
        state = STATE_STOPPED;
        stateStartedAt = now;
        Serial.println(F("FINISHED: 12 corners / 3 laps"));
      }
      break;

    case STATE_STOPPED:
      setSteering(SERVO_CENTER_DEG);
      motorCoastStop();
      break;
  }
}

// ============================================================================
// 10. SERIAL DIAGNOSTICS
// ============================================================================
const __FlashStringHelper *stateName(DriveState value) {
  switch (value) {
    case STATE_STARTUP: return F("STARTUP");
    case STATE_STRAIGHT: return F("STRAIGHT");
    case STATE_TURN: return F("TURN");
    case STATE_ALIGN: return F("ALIGN");
    case STATE_EXIT: return F("EXIT");
    case STATE_FINAL_RUN: return F("FINAL_RUN");
    case STATE_FINAL_BRAKE: return F("FINAL_BRAKE");
    case STATE_STOPPED: return F("STOPPED");
  }
  return F("?");
}

void printDebug(unsigned long now) {
  if (now - lastDebugAt < DEBUG_INTERVAL_MS) return;
  lastDebugAt = now;

  Serial.print(F("State=")); Serial.print(stateName(state));
  Serial.print(F(" Dir="));
  if (direction == DIR_LEFT) Serial.print(F("L"));
  else if (direction == DIR_RIGHT) Serial.print(F("R"));
  else Serial.print(F("?"));
  Serial.print(F(" Corners=")); Serial.print(completedCorners);
  Serial.print(F(" Laps=")); Serial.print(completedCorners / 4);
  Serial.print(F(" L=")); Serial.print(sonarLeft.rawCm);
  Serial.print(F(" R=")); Serial.print(sonarRight.rawCm);
  Serial.print(F(" F=")); Serial.print(sonarFront.rawCm);
  Serial.print(F(" PWM=")); Serial.print(motorCurrentPwm);
  Serial.print(F(" Target=")); Serial.print(motorTargetPwm);
  Serial.print(F(" Servo=")); Serial.print(lastServoCommand);
  Serial.print(F(" FrontSeen=")); Serial.print(frontCornerSeen ? F("YES") : F("NO"));
  Serial.print(F(" Armed=")); Serial.println(cornerArmed ? F("YES") : F("NO"));
}

// ============================================================================
// 11. ARDUINO ENTRY POINTS
// ============================================================================
void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  pinMode(PIN_FRONT_TRIG, OUTPUT);
  pinMode(PIN_FRONT_ECHO, INPUT);
  pinMode(PIN_RIGHT_TRIG, OUTPUT);
  pinMode(PIN_RIGHT_ECHO, INPUT);
  pinMode(PIN_LEFT_TRIG, OUTPUT);
  pinMode(PIN_LEFT_ECHO, INPUT);

  digitalWrite(PIN_FRONT_TRIG, LOW);
  digitalWrite(PIN_RIGHT_TRIG, LOW);
  digitalWrite(PIN_LEFT_TRIG, LOW);

  steering.attach(PIN_SERVO);
  setSteering(SERVO_CENTER_DEG);

  // Avance inmediato al encender: no espera y no arranca desde PWM cero.
  enableForwardDirection();
  motorCurrentPwm = START_PWM;
  motorTargetPwm = START_PWM;
  analogWrite(PIN_ENA, START_PWM);

  unsigned long now = millis();
  raceStartedAt = now;
  state = STATE_STRAIGHT;
  stateStartedAt = now;
  nextPingAt = now;
  lastPwmRampAt = now;

  Serial.println(F("Olibot high-speed profile - immediate start"));
  Serial.print(F("PWM straight=")); Serial.print(CRUISE_PWM);
  Serial.print(F(" turn=")); Serial.print(TURN_PWM);
  Serial.print(F(" align=")); Serial.print(ALIGN_PWM);
  Serial.print(F(" exit=")); Serial.println(EXIT_PWM);
}

void loop() {
  unsigned long now = millis();

  updateOneSonar(now);
  updateStateMachine(now);

  // Braking writes ENA directly; ramping must be suspended in brake/stop.
  if (state != STATE_FINAL_BRAKE && state != STATE_STOPPED)
    updateMotorRamp(now);

  printDebug(now);
}