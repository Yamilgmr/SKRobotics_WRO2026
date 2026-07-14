#include <Servo.h>

// -----------------------------------------------------------------------------
// PIN MAP CONFIRMADO
// -----------------------------------------------------------------------------

const uint8_t PIN_SERVO      = 9;
const uint8_t PIN_ENA        = 5;   // PWM del motor: analogWrite()
const uint8_t PIN_IN1        = 6;   // Direccion L298N
const uint8_t PIN_IN2        = 7;   // Direccion L298N

const uint8_t PIN_FRONT_TRIG = 42;
const uint8_t PIN_FRONT_ECHO = 43;
const uint8_t PIN_RIGHT_TRIG = 46;
const uint8_t PIN_RIGHT_ECHO = 47;
const uint8_t PIN_LEFT_TRIG  = 52;
const uint8_t PIN_LEFT_ECHO  = 53;

// -----------------------------------------------------------------------------
// CALIBRACION FISICA - AJUSTAR EN ESTE ORDEN
// -----------------------------------------------------------------------------

// 1) Direccion del motor y del servo.
const bool MOTOR_INVERTED    = false;
const bool STEERING_INVERTED = false;

// 2) Angulos reales medidos en el mecanismo Ackermann.
// Al detectar abertura aplica inmediatamente el angulo de giro configurado.
const int16_t SERVO_LEFT_ENTRY_DEG  = 45;
const int16_t SERVO_LEFT_DEG        = 35;
const int16_t SERVO_CENTER_DEG = 90;
const int16_t SERVO_RIGHT_ENTRY_DEG = 90;
const int16_t SERVO_RIGHT_DEG       = 110;
const int16_t SERVO_LEFT_ALIGN_DEG  = 45;
const int16_t SERVO_RIGHT_ALIGN_DEG = 118;
const int16_t SERVO_MIN_DEG    = SERVO_LEFT_DEG;
const int16_t SERVO_MAX_DEG    = SERVO_RIGHT_DEG;

// 3) Velocidades PWM. Empieza bajo y aumenta solo despues de estabilizar giros.
const uint8_t START_PWM         = 200;
const uint8_t CRUISE_PWM        = 195;
const uint8_t APPROACH_PWM      = 170;
const uint8_t DECISION_PWM      = 145;
const uint8_t TURN_PWM          = 255;
const uint8_t ALIGN_PWM         = 180;
const uint8_t EXIT_PWM          = 170;
const uint8_t FINAL_PWM         = 185;

// 4) Geometria de la pista y posicion de los sensores, en centimetros.
const uint16_t WALL_TARGET_CM          = 28;
const uint16_t WALL_SEEN_MAX_CM        = 78;
const uint16_t SIDE_OPEN_CM            = 110;
const uint16_t FRONT_APPROACH_CM       = 130;
// Respaldo si ningun lateral alcanzo a detectar antes la abertura.
const uint16_t FRONT_TURN_CM           = 100;
const uint16_t FRONT_DECISION_STOP_CM  = 42;
const uint16_t FRONT_HARD_STOP_CM      = 10;
// El giro conserva la secuencia estable anterior; el frontal solo interviene
// para reconocer que el carro ya mira hacia la nueva recta.
const uint16_t TURN_EXIT_FRONT_CM      = 100;
const uint16_t TURN_EXIT_WALL_MAX_CM   = 105;
const uint16_t TURN_EXIT_SIDE_MAX_CM   = 130;
const uint16_t ALIGN_FRONT_CLEAR_CM    = 145;
const uint16_t ALIGN_SIDE_MAX_CM       = 130;
const uint16_t WALL_FOLLOW_MAX_CM      = 95;

// Separacion medida entre las caras de los dos sensores laterales: 10.1 cm.
// Se conserva en milimetros para no perder el milimetro de la medicion.
const uint16_t LATERAL_SENSOR_SPAN_MM  = 101;

// 5) Tiempo de giro. Son limites de seguridad, no delays bloqueantes.
const uint16_t TURN_MIN_MS             = 520;
const uint16_t TURN_MAX_MS             = 900;
// Antirrepeticion: ademas se exige volver a ver pared en ambos laterales.
const uint16_t MIN_CORNER_INTERVAL_MS  = 450;
const uint16_t TURN_ENTRY_STEERING_MS  = 0;
const uint16_t ALIGN_MIN_MS            = 260;
const uint16_t ALIGN_MAX_MS            = 750;
const uint16_t EXIT_COUNTERSTEER_MS    = 120;
const uint16_t EXIT_RECOVERY_MS        = 420;
const int16_t  EXIT_COUNTERSTEER_DEG   = 8;

// Tiempo que avanza en la recta inicial despues de completar la esquina 12.
const uint16_t FINAL_ADVANCE_MS        = 1600;

// Control PD lateral. Kp corrige distancia; Kd reduce oscilacion.
const float WALL_KP                    = 0.42f;
const float WALL_KD                    = 0.85f;
const int16_t WALL_MAX_STEERING_DEG    = 13;

// Centrado antes de conocer el sentido de la pista.
const float PRE_DIRECTION_KP           = 0.32f;
const int16_t PRE_DIRECTION_MAX_DEG    = 9;

// Monitor serie. Desactivalo para la corrida final si no lo necesitas.
const bool SERIAL_DEBUG                = true;
const uint16_t DEBUG_INTERVAL_MS       = 200;

// -----------------------------------------------------------------------------
// CONSTANTES INTERNAS
// -----------------------------------------------------------------------------

const uint8_t TOTAL_TURNS              = 12;
const uint16_t MAX_DISTANCE_CM         = 240;
const uint16_t MIN_DISTANCE_CM         = 3;
const uint8_t FILTER_SAMPLES           = 5;

const uint32_t ECHO_RISE_TIMEOUT_US    = 3000UL;
const uint32_t ECHO_MAX_US             = 14500UL;
const uint32_t INTER_SENSOR_GAP_US     = 18000UL;

const uint8_t WALL_CONFIRM_SAMPLES     = 2;
// Una sola lectura cruda de abertura dispara el giro inmediatamente.
const uint8_t OPEN_CONFIRM_SAMPLES     = 1;
const uint8_t SIDE_WALL_CONFIRM_SAMPLES = 2;
const uint8_t FRONT_CONFIRM_SAMPLES    = 2;
const uint8_t TURN_EXIT_CONFIRM_SAMPLES = 2;
const uint8_t ALIGN_CONFIRM_SAMPLES    = 2;

const uint16_t SENSOR_FRESH_MS         = 550;
const uint16_t CONTROL_INTERVAL_MS     = 20;
const uint16_t STARTUP_RAMP_MS         = 450;

// -----------------------------------------------------------------------------
// TIPOS Y ESTADO GLOBAL
// -----------------------------------------------------------------------------

enum SensorId : uint8_t {
  SENSOR_FRONT = 0,
  SENSOR_RIGHT = 1,
  SENSOR_LEFT  = 2,
  SENSOR_COUNT = 3
};

enum PingPhase : uint8_t {
  PING_IDLE,
  PING_TRIGGER_HIGH,
  PING_WAIT_RISE,
  PING_WAIT_FALL
};

enum TrackDirection : int8_t {
  DIRECTION_UNKNOWN = 0,
  DIRECTION_LEFT    = -1,
  DIRECTION_RIGHT   = 1
};

enum RobotState : uint8_t {
  STATE_FIND_DIRECTION,
  STATE_CRUISE,
  STATE_CONFIRM_SIDES,
  STATE_TURNING,
  STATE_FINE_ALIGN,
  STATE_EXIT_TURN,
  STATE_FINAL_ADVANCE,
  STATE_STOPPED,
  STATE_SAFETY_STOP
};

struct SonarData {
  uint8_t trigPin;
  uint8_t echoPin;
  uint16_t rawCm;
  uint16_t filteredCm;
  uint16_t history[FILTER_SAMPLES];
  uint8_t historyCount;
  uint8_t historyIndex;
  uint8_t wallStreak;
  uint8_t openStreak;
  bool wallSeen;
  uint32_t lastSampleMs;
  uint32_t sequence;
};

Servo steeringServo;

SonarData sonar[SENSOR_COUNT] = {
  {PIN_FRONT_TRIG, PIN_FRONT_ECHO, MAX_DISTANCE_CM, MAX_DISTANCE_CM,
   {MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM},
   0, 0, 0, 0, false, 0, 0},
  {PIN_RIGHT_TRIG, PIN_RIGHT_ECHO, MAX_DISTANCE_CM, MAX_DISTANCE_CM,
   {MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM},
   0, 0, 0, 0, false, 0, 0},
  {PIN_LEFT_TRIG, PIN_LEFT_ECHO, MAX_DISTANCE_CM, MAX_DISTANCE_CM,
   {MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM, MAX_DISTANCE_CM},
   0, 0, 0, 0, false, 0, 0}
};

// Se consultan los laterales mas veces que el frontal para reaccionar antes.
// Cada lateral aparece dos veces y el frontal una vez por ciclo.
const SensorId PING_ORDER[] = {
  SENSOR_RIGHT, SENSOR_LEFT, SENSOR_FRONT, SENSOR_RIGHT, SENSOR_LEFT
};
const uint8_t PING_ORDER_COUNT = sizeof(PING_ORDER) / sizeof(PING_ORDER[0]);

PingPhase pingPhase = PING_IDLE;
uint8_t pingOrderIndex = 0;
uint8_t activeSensor = PING_ORDER[0];
uint32_t pingPhaseAtUs = 0;
uint32_t echoRiseAtUs = 0;
uint32_t nextPingAtUs = 0;

TrackDirection trackDirection = DIRECTION_UNKNOWN;
RobotState robotState = STATE_FIND_DIRECTION;

uint8_t completedTurns = 0;
uint8_t frontCloseStreak = 0;
uint8_t turnExitStreak = 0;
uint8_t alignmentStreak = 0;
uint8_t decisionLeftOpenStreak = 0;
uint8_t decisionRightOpenStreak = 0;
uint8_t decisionLeftWallStreak = 0;
uint8_t decisionRightWallStreak = 0;
uint32_t lastProcessedFrontSequence = 0;
uint32_t lastAlignmentFrontSequence = 0;
uint32_t decisionLeftStartSequence = 0;
uint32_t decisionRightStartSequence = 0;
bool nextCornerArmed = false;

uint32_t bootAtMs = 0;
uint32_t stateAtMs = 0;
uint32_t lastTurnCompletedMs = 0;
uint32_t lastControlMs = 0;
uint32_t lastDebugMs = 0;

float previousWallError = 0.0f;
bool previousWallErrorValid = false;

uint8_t targetMotorPwm = 0;
uint8_t currentMotorPwm = 0;
uint32_t lastMotorRampMs = 0;
bool brakeActive = false;
int16_t lastServoAngle = SERVO_CENTER_DEG;

// -----------------------------------------------------------------------------
// UTILIDADES
// -----------------------------------------------------------------------------

bool timeReachedUs(uint32_t now, uint32_t target) {
  return (int32_t)(now - target) >= 0;
}

uint8_t saturatingIncrement(uint8_t value) {
  return (value < 255) ? (uint8_t)(value + 1) : value;
}

const __FlashStringHelper* stateName(RobotState state) {
  switch (state) {
    case STATE_FIND_DIRECTION: return F("BUSCANDO_SENTIDO");
    case STATE_CRUISE:         return F("RECTA");
    case STATE_CONFIRM_SIDES:  return F("CONFIRMANDO_LADOS");
    case STATE_TURNING:        return F("GIRANDO");
    case STATE_FINE_ALIGN:      return F("AJUSTE_GIRO");
    case STATE_EXIT_TURN:      return F("SALIDA_GIRO");
    case STATE_FINAL_ADVANCE:  return F("AVANCE_FINAL");
    case STATE_STOPPED:        return F("DETENIDO");
    case STATE_SAFETY_STOP:    return F("PARADA_SEGURIDAD");
  }
  return F("DESCONOCIDO");
}

const __FlashStringHelper* directionName(TrackDirection direction) {
  if (direction == DIRECTION_LEFT) return F("IZQUIERDA");
  if (direction == DIRECTION_RIGHT) return F("DERECHA");
  return F("SIN_DEFINIR");
}

bool sensorFresh(SensorId id) {
  return sonar[id].sequence > 0 &&
         (uint32_t)(millis() - sonar[id].lastSampleMs) <= SENSOR_FRESH_MS;
}

SensorId outerWallSensor() {
  return (trackDirection == DIRECTION_RIGHT) ? SENSOR_LEFT : SENSOR_RIGHT;
}

// -----------------------------------------------------------------------------
// MOTOR Y DIRECCION
// -----------------------------------------------------------------------------

void applyForwardPwm(uint8_t pwm) {
  if (MOTOR_INVERTED) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
  } else {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  }
  analogWrite(PIN_ENA, pwm);
}

void setMotorTarget(uint8_t pwm) {
  targetMotorPwm = pwm;
  brakeActive = false;
}

void brakeMotor() {
  targetMotorPwm = 0;
  currentMotorPwm = 0;
  brakeActive = true;
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
  analogWrite(PIN_ENA, 255);
}

void updateMotorRamp() {
  if (brakeActive) return;

  uint32_t now = millis();
  if ((uint32_t)(now - lastMotorRampMs) < 10) return;
  lastMotorRampMs = now;

  if (currentMotorPwm < targetMotorPwm) {
    uint16_t next = (uint16_t)currentMotorPwm + 4;
    currentMotorPwm = (next > targetMotorPwm) ? targetMotorPwm : (uint8_t)next;
  } else if (currentMotorPwm > targetMotorPwm) {
    int16_t next = (int16_t)currentMotorPwm - 8;
    currentMotorPwm = (next < targetMotorPwm) ? targetMotorPwm : (uint8_t)next;
  }

  applyForwardPwm(currentMotorPwm);
}

void writeServoAngle(int16_t angle) {
  if (angle < SERVO_MIN_DEG) angle = SERVO_MIN_DEG;
  if (angle > SERVO_MAX_DEG) angle = SERVO_MAX_DEG;
  lastServoAngle = angle;
  steeringServo.write(angle);
}

void setSteeringOffset(int16_t logicalOffsetDeg) {
  const int16_t minOffset = SERVO_MIN_DEG - SERVO_CENTER_DEG;
  const int16_t maxOffset = SERVO_MAX_DEG - SERVO_CENTER_DEG;
  if (logicalOffsetDeg < minOffset) logicalOffsetDeg = minOffset;
  if (logicalOffsetDeg > maxOffset) logicalOffsetDeg = maxOffset;

  int16_t physicalOffset = STEERING_INVERTED ? -logicalOffsetDeg : logicalOffsetDeg;
  int16_t angle = SERVO_CENTER_DEG + physicalOffset;
  writeServoAngle(angle);
}

void setTurnSteering() {
  bool commandRight = (trackDirection == DIRECTION_RIGHT);
  if (STEERING_INVERTED) commandRight = !commandRight;

  bool entryPhase = (uint32_t)(millis() - stateAtMs) < TURN_ENTRY_STEERING_MS;
  if (commandRight) {
    writeServoAngle(entryPhase ? SERVO_RIGHT_ENTRY_DEG : SERVO_RIGHT_DEG);
  } else {
    writeServoAngle(entryPhase ? SERVO_LEFT_ENTRY_DEG : SERVO_LEFT_DEG);
  }
}

void setFineAlignmentSteering() {
  bool commandRight = (trackDirection == DIRECTION_RIGHT);
  if (STEERING_INVERTED) commandRight = !commandRight;
  writeServoAngle(commandRight ? SERVO_RIGHT_ALIGN_DEG : SERVO_LEFT_ALIGN_DEG);
}

// -----------------------------------------------------------------------------
// FILTRO Y ESCANER ASINCRONO PARA HC-SR04
// -----------------------------------------------------------------------------

uint16_t medianDistance(const SonarData &s) {
  uint8_t count = s.historyCount;
  if (count == 0) return MAX_DISTANCE_CM;

  uint16_t ordered[FILTER_SAMPLES];
  for (uint8_t i = 0; i < count; i++) ordered[i] = s.history[i];

  for (uint8_t i = 1; i < count; i++) {
    uint16_t value = ordered[i];
    int8_t j = (int8_t)i - 1;
    while (j >= 0 && ordered[j] > value) {
      ordered[j + 1] = ordered[j];
      j--;
    }
    ordered[j + 1] = value;
  }

  return ordered[count / 2];
}

void updateSonarEvidence(SensorId id) {
  SonarData &s = sonar[id];

  if (id == SENSOR_FRONT) {
    bool closeNow = (s.rawCm <= FRONT_TURN_CM) ||
                    (s.filteredCm <= FRONT_TURN_CM);
    frontCloseStreak = closeNow ? saturatingIncrement(frontCloseStreak) : 0;
    return;
  }

  if (s.rawCm >= MIN_DISTANCE_CM && s.rawCm <= WALL_SEEN_MAX_CM) {
    s.wallStreak = saturatingIncrement(s.wallStreak);
    if (s.wallStreak >= WALL_CONFIRM_SAMPLES) s.wallSeen = true;
  } else {
    s.wallStreak = 0;
  }

  bool openingNow = s.wallSeen &&
                    (s.rawCm >= SIDE_OPEN_CM || s.filteredCm >= SIDE_OPEN_CM);
  s.openStreak = openingNow ? saturatingIncrement(s.openStreak) : 0;
}

void recordSonarSample(SensorId id, uint16_t distanceCm) {
  SonarData &s = sonar[id];
  if (distanceCm < MIN_DISTANCE_CM) distanceCm = MIN_DISTANCE_CM;
  if (distanceCm > MAX_DISTANCE_CM) distanceCm = MAX_DISTANCE_CM;

  s.rawCm = distanceCm;
  s.history[s.historyIndex] = distanceCm;
  s.historyIndex = (uint8_t)((s.historyIndex + 1) % FILTER_SAMPLES);
  if (s.historyCount < FILTER_SAMPLES) s.historyCount++;
  s.filteredCm = medianDistance(s);
  s.lastSampleMs = millis();
  s.sequence++;

  updateSonarEvidence(id);
}

void finishActivePing(uint16_t distanceCm, uint32_t nowUs) {
  digitalWrite(sonar[activeSensor].trigPin, LOW);
  recordSonarSample((SensorId)activeSensor, distanceCm);

  pingOrderIndex = (uint8_t)((pingOrderIndex + 1) % PING_ORDER_COUNT);
  activeSensor = (uint8_t)PING_ORDER[pingOrderIndex];
  pingPhase = PING_IDLE;
  nextPingAtUs = nowUs + INTER_SENSOR_GAP_US;
}

void updateSonarScanner() {
  uint32_t nowUs = micros();
  SonarData &s = sonar[activeSensor];

  switch (pingPhase) {
    case PING_IDLE:
      if (!timeReachedUs(nowUs, nextPingAtUs)) return;
      digitalWrite(s.trigPin, HIGH);
      pingPhaseAtUs = nowUs;
      pingPhase = PING_TRIGGER_HIGH;
      break;

    case PING_TRIGGER_HIGH:
      if ((uint32_t)(nowUs - pingPhaseAtUs) < 10UL) return;
      digitalWrite(s.trigPin, LOW);
      pingPhaseAtUs = nowUs;
      pingPhase = PING_WAIT_RISE;
      break;

    case PING_WAIT_RISE:
      if (digitalRead(s.echoPin) == HIGH) {
        echoRiseAtUs = nowUs;
        pingPhase = PING_WAIT_FALL;
      } else if ((uint32_t)(nowUs - pingPhaseAtUs) >= ECHO_RISE_TIMEOUT_US) {
        // Sin eco significa espacio libre o distancia mayor al rango util.
        finishActivePing(MAX_DISTANCE_CM, nowUs);
      }
      break;

    case PING_WAIT_FALL:
      if (digitalRead(s.echoPin) == LOW) {
        uint32_t echoDurationUs = nowUs - echoRiseAtUs;
        uint16_t cm = (uint16_t)(echoDurationUs / 58UL);
        if (cm < MIN_DISTANCE_CM || cm > MAX_DISTANCE_CM) cm = MAX_DISTANCE_CM;
        finishActivePing(cm, nowUs);
      } else if ((uint32_t)(nowUs - echoRiseAtUs) >= ECHO_MAX_US) {
        finishActivePing(MAX_DISTANCE_CM, nowUs);
      }
      break;
  }
}

// -----------------------------------------------------------------------------
// LOGICA DE NAVEGACION
// -----------------------------------------------------------------------------

void changeState(RobotState nextState) {
  robotState = nextState;
  stateAtMs = millis();

  if (SERIAL_DEBUG) {
    Serial.print(F("EVENTO | Estado -> "));
    Serial.println(stateName(nextState));
  }
}

void safetyStop(const __FlashStringHelper *reason) {
  setSteeringOffset(0);
  brakeMotor();
  changeState(STATE_SAFETY_STOP);
  if (SERIAL_DEBUG) {
    Serial.print(F("PARADA DE SEGURIDAD | "));
    Serial.println(reason);
  }
}

int16_t preDirectionSteering() {
  if (!sensorFresh(SENSOR_LEFT) || !sensorFresh(SENSOR_RIGHT)) return 0;

  uint16_t left = sonar[SENSOR_LEFT].filteredCm;
  uint16_t right = sonar[SENSOR_RIGHT].filteredCm;
  if (left > WALL_FOLLOW_MAX_CM || right > WALL_FOLLOW_MAX_CM) return 0;

  // Cada HC-SR04 mide desde su propia cara. Sumamos media separacion para
  // comparar la distancia de cada pared al centro real del carro.
  const float halfSensorSpanCm = (float)LATERAL_SENSOR_SPAN_MM / 20.0f;
  float leftFromCenterCm = (float)left + halfSensorSpanCm;
  float rightFromCenterCm = (float)right + halfSensorSpanCm;
  float correction = (rightFromCenterCm - leftFromCenterCm) * PRE_DIRECTION_KP;
  return (int16_t)constrain(correction,
                            (float)-PRE_DIRECTION_MAX_DEG,
                            (float)PRE_DIRECTION_MAX_DEG);
}

int16_t wallFollowingSteering() {
  if (trackDirection == DIRECTION_UNKNOWN) return 0;

  SensorId wallSensor = outerWallSensor();
  if (!sensorFresh(wallSensor)) {
    previousWallErrorValid = false;
    return 0;
  }

  uint16_t distance = sonar[wallSensor].filteredCm;
  if (distance > WALL_FOLLOW_MAX_CM) {
    previousWallErrorValid = false;
    return 0;
  }

  float error = (float)distance - (float)WALL_TARGET_CM;
  float derivative = previousWallErrorValid ? (error - previousWallError) : 0.0f;
  previousWallError = error;
  previousWallErrorValid = true;

  // La pared exterior esta al lado opuesto del sentido de giro.
  float command = -(float)trackDirection * (WALL_KP * error + WALL_KD * derivative);
  return (int16_t)constrain(command,
                            (float)-WALL_MAX_STEERING_DEG,
                            (float)WALL_MAX_STEERING_DEG);
}

uint16_t closestFrontDistance() {
  uint16_t raw = sonar[SENSOR_FRONT].rawCm;
  uint16_t filtered = sonar[SENSOR_FRONT].filteredCm;
  return (raw < filtered) ? raw : filtered;
}

bool frontTurnConfirmed() {
  return sensorFresh(SENSOR_FRONT) &&
         frontCloseStreak >= FRONT_CONFIRM_SAMPLES;
}

bool updateCornerRearm() {
  if (nextCornerArmed) return true;

  if ((uint32_t)(millis() - lastTurnCompletedMs) < MIN_CORNER_INTERVAL_MS) {
    return false;
  }

  // Se rearma solo despues de abandonar la abertura y recuperar las dos
  // paredes laterales. El frontal no bloquea la siguiente esquina.
  bool corridorRecovered = sensorFresh(SENSOR_LEFT) &&
                           sensorFresh(SENSOR_RIGHT) &&
                           sonar[SENSOR_LEFT].wallSeen &&
                           sonar[SENSOR_RIGHT].wallSeen;
  if (corridorRecovered) {
    nextCornerArmed = true;
    frontCloseStreak = 0;
    if (SERIAL_DEBUG) {
      Serial.println(F("EVENTO | Siguiente esquina armada por paredes laterales"));
    }
  }

  return nextCornerArmed;
}

void beginTurn();

void resetSideEdgeEvidence() {
  sonar[SENSOR_LEFT].wallStreak = 0;
  sonar[SENSOR_LEFT].openStreak = 0;
  sonar[SENSOR_LEFT].wallSeen = false;
  sonar[SENSOR_RIGHT].wallStreak = 0;
  sonar[SENSOR_RIGHT].openStreak = 0;
  sonar[SENSOR_RIGHT].wallSeen = false;
}

TrackDirection immediateOpeningDirection() {
  bool leftOpened = sensorFresh(SENSOR_LEFT) &&
                    sonar[SENSOR_LEFT].wallSeen &&
                    sonar[SENSOR_LEFT].openStreak >= OPEN_CONFIRM_SAMPLES;
  bool rightOpened = sensorFresh(SENSOR_RIGHT) &&
                     sonar[SENSOR_RIGHT].wallSeen &&
                     sonar[SENSOR_RIGHT].openStreak >= OPEN_CONFIRM_SAMPLES;

  // Cada sensor trabaja de forma independiente: el izquierdo abre giro a la
  // izquierda y el derecho abre giro a la derecha. wallSeen obliga a que antes
  // haya existido pared, evitando decidir solo porque el carro arranco pegado
  // al lado contrario.
  if (leftOpened == rightOpened) return DIRECTION_UNKNOWN;
  return leftOpened ? DIRECTION_LEFT : DIRECTION_RIGHT;
}

bool tryImmediateOpeningTurn() {
  if (!nextCornerArmed ||
      (robotState != STATE_FIND_DIRECTION && robotState != STATE_CRUISE)) {
    return false;
  }

  TrackDirection detected = immediateOpeningDirection();
  if (detected == DIRECTION_UNKNOWN) return false;

  // La abertura observada manda en esta esquina. Asi funciona tanto a la
  // izquierda como a la derecha sin bloquearse al sentido de un giro anterior.
  trackDirection = detected;

  if (SERIAL_DEBUG) {
    Serial.print(F("EVENTO | Apertura lateral; giro inmediato a "));
    Serial.println(directionName(detected));
  }

  changeState(STATE_CONFIRM_SIDES);
  beginTurn();
  return true;
}

void beginSideConfirmation() {
  if (robotState != STATE_FIND_DIRECTION && robotState != STATE_CRUISE) return;

  frontCloseStreak = 0;
  decisionLeftOpenStreak = 0;
  decisionRightOpenStreak = 0;
  decisionLeftWallStreak = 0;
  decisionRightWallStreak = 0;
  decisionLeftStartSequence = sonar[SENSOR_LEFT].sequence;
  decisionRightStartSequence = sonar[SENSOR_RIGHT].sequence;

  changeState(STATE_CONFIRM_SIDES);
  setSteeringOffset(0);
  setMotorTarget(DECISION_PWM);

  if (SERIAL_DEBUG) {
    Serial.println(F("EVENTO | F<=100 confirmado; capturando lados NUEVOS"));
  }
}

void updateOneDecisionSide(SensorId id, uint32_t &lastSequence,
                           uint8_t &openStreak, uint8_t &wallStreak) {
  if (!sensorFresh(id) || sonar[id].sequence == lastSequence) return;
  lastSequence = sonar[id].sequence;

  uint16_t distance = sonar[id].rawCm;
  if (distance >= SIDE_OPEN_CM) {
    openStreak = saturatingIncrement(openStreak);
    wallStreak = 0;
  } else if (distance <= WALL_SEEN_MAX_CM) {
    wallStreak = saturatingIncrement(wallStreak);
    openStreak = 0;
  } else {
    // Zona ambigua 79..109 cm: no confirma ni pared ni abertura.
    openStreak = 0;
    wallStreak = 0;
  }
}

TrackDirection confirmedSideDirection() {
  bool turnLeft = decisionLeftOpenStreak >= OPEN_CONFIRM_SAMPLES &&
                  decisionRightWallStreak >= SIDE_WALL_CONFIRM_SAMPLES;
  bool turnRight = decisionRightOpenStreak >= OPEN_CONFIRM_SAMPLES &&
                   decisionLeftWallStreak >= SIDE_WALL_CONFIRM_SAMPLES;

  if (turnLeft == turnRight) return DIRECTION_UNKNOWN;
  return turnLeft ? DIRECTION_LEFT : DIRECTION_RIGHT;
}

void handleSideConfirmation() {
  setSteeringOffset(0);

  uint16_t front = closestFrontDistance();
  setMotorTarget((sensorFresh(SENSOR_FRONT) &&
                  front <= FRONT_DECISION_STOP_CM) ? 0 : DECISION_PWM);

  updateOneDecisionSide(SENSOR_LEFT,
                        decisionLeftStartSequence,
                        decisionLeftOpenStreak,
                        decisionLeftWallStreak);
  updateOneDecisionSide(SENSOR_RIGHT,
                        decisionRightStartSequence,
                        decisionRightOpenStreak,
                        decisionRightWallStreak);

  TrackDirection detected = confirmedSideDirection();
  if (detected != DIRECTION_UNKNOWN) {
    trackDirection = detected;
    if (SERIAL_DEBUG) {
      Serial.print(F("EVENTO | Respaldo frontal confirma giro a "));
      Serial.println(directionName(trackDirection));
    }
    beginTurn();
    return;
  }

  if (sensorFresh(SENSOR_FRONT) && front <= FRONT_HARD_STOP_CM) {
    safetyStop(F("Lados ambiguos o contradictorios en la esquina"));
  }
}

void beginTurn() {
  if (robotState != STATE_CONFIRM_SIDES ||
      trackDirection == DIRECTION_UNKNOWN) return;

  frontCloseStreak = 0;
  turnExitStreak = 0;
  alignmentStreak = 0;
  nextCornerArmed = false;
  lastProcessedFrontSequence = sonar[SENSOR_FRONT].sequence;
  previousWallErrorValid = false;

  changeState(STATE_TURNING);
  setTurnSteering();
  setMotorTarget(TURN_PWM);

  if (SERIAL_DEBUG) {
    Serial.print(F("EVENTO | Inicia giro a "));
    Serial.println(directionName(trackDirection));
  }
}

void beginFineAlignment() {
  alignmentStreak = 0;
  lastAlignmentFrontSequence = sonar[SENSOR_FRONT].sequence;
  changeState(STATE_FINE_ALIGN);
  setFineAlignmentSteering();
  setMotorTarget(ALIGN_PWM);

  if (SERIAL_DEBUG) {
    Serial.println(F("EVENTO | Inicia segunda correccion del giro"));
  }
}

void completeTurn() {
  completedTurns++;
  lastTurnCompletedMs = millis();
  turnExitStreak = 0;
  nextCornerArmed = false;

  setMotorTarget(EXIT_PWM);
  setSteeringOffset((int16_t)-trackDirection * EXIT_COUNTERSTEER_DEG);
  changeState(STATE_EXIT_TURN);

  if (SERIAL_DEBUG) {
    Serial.print(F("EVENTO | Esquina completada: "));
    Serial.print(completedTurns);
    Serial.print(F("/"));
    Serial.print(TOTAL_TURNS);
    Serial.print(F(" | Vueltas: "));
    Serial.println(completedTurns / 4);
  }
}

void updateTurnCompletion() {
  uint32_t elapsed = millis() - stateAtMs;
  if (elapsed < TURN_MIN_MS) return;

  if (sonar[SENSOR_FRONT].sequence != lastProcessedFrontSequence) {
    lastProcessedFrontSequence = sonar[SENSOR_FRONT].sequence;

    bool frontClear = sensorFresh(SENSOR_FRONT) &&
                      sonar[SENSOR_FRONT].filteredCm >= TURN_EXIT_FRONT_CM;
    SensorId wallId = outerWallSensor();
    bool wallRecovered = sensorFresh(wallId) &&
                         sonar[wallId].filteredCm <= TURN_EXIT_WALL_MAX_CM;
    bool bothSidesAligned = sensorFresh(SENSOR_LEFT) &&
                            sensorFresh(SENSOR_RIGHT) &&
                            sonar[SENSOR_LEFT].filteredCm <= TURN_EXIT_SIDE_MAX_CM &&
                            sonar[SENSOR_RIGHT].filteredCm <= TURN_EXIT_SIDE_MAX_CM;

    turnExitStreak = (frontClear && wallRecovered && bothSidesAligned)
                     ? saturatingIncrement(turnExitStreak) : 0;
  }

  if (turnExitStreak >= TURN_EXIT_CONFIRM_SAMPLES || elapsed >= TURN_MAX_MS) {
    if (elapsed >= TURN_MAX_MS && SERIAL_DEBUG) {
      Serial.println(F("AVISO | Giro terminado por limite de tiempo"));
    }
    beginFineAlignment();
  }
}

void updateFineAlignment() {
  uint32_t elapsed = millis() - stateAtMs;

  if (sonar[SENSOR_FRONT].sequence != lastAlignmentFrontSequence) {
    lastAlignmentFrontSequence = sonar[SENSOR_FRONT].sequence;

    bool frontClear = sensorFresh(SENSOR_FRONT) &&
                      sonar[SENSOR_FRONT].filteredCm >= ALIGN_FRONT_CLEAR_CM;
    bool sidesShowCorridor = sensorFresh(SENSOR_LEFT) &&
                             sensorFresh(SENSOR_RIGHT) &&
                             sonar[SENSOR_LEFT].filteredCm <= ALIGN_SIDE_MAX_CM &&
                             sonar[SENSOR_RIGHT].filteredCm <= ALIGN_SIDE_MAX_CM;

    alignmentStreak = (frontClear && sidesShowCorridor)
                      ? saturatingIncrement(alignmentStreak) : 0;
  }

  bool confirmed = elapsed >= ALIGN_MIN_MS &&
                   alignmentStreak >= ALIGN_CONFIRM_SAMPLES;
  if (confirmed || elapsed >= ALIGN_MAX_MS) {
    if (elapsed >= ALIGN_MAX_MS && SERIAL_DEBUG) {
      Serial.println(F("AVISO | Ajuste del giro terminado por limite de tiempo"));
    }
    completeTurn();
  }
}

void handleFindDirection() {
  uint32_t sinceBoot = millis() - bootAtMs;
  setMotorTarget((sinceBoot < STARTUP_RAMP_MS) ? START_PWM : CRUISE_PWM);

  uint16_t front = closestFrontDistance();
  if (sensorFresh(SENSOR_FRONT) && front <= FRONT_APPROACH_CM) {
    setMotorTarget(APPROACH_PWM);
  }

  // Mientras avanza centra el carro; la decision usa transiciones laterales,
  // no la diferencia instantanea causada por empezar pegado a una pared.
  setSteeringOffset(preDirectionSteering());

  // Ruta principal: no espera al frontal si ya ocurrio pared -> abertura.
  if (tryImmediateOpeningTurn()) return;

  // Respaldo: si la geometria oculto la abertura lateral, decide desde F<=100.
  if (nextCornerArmed && frontTurnConfirmed()) {
    beginSideConfirmation();
  }
}

void handleCruise() {
  uint16_t front = closestFrontDistance();
  setMotorTarget((sensorFresh(SENSOR_FRONT) && front <= FRONT_APPROACH_CM)
                 ? APPROACH_PWM : CRUISE_PWM);
  setSteeringOffset(wallFollowingSteering());

  // Rearma al recuperar paredes laterales; no espera al sensor frontal.
  if (!updateCornerRearm()) return;

  // La abertura lateral tiene prioridad y ordena el giro inmediatamente.
  if (tryImmediateOpeningTurn()) return;

  // El frontal a 100 cm solo respalda una abertura que no pudo verse antes.
  if (frontTurnConfirmed()) beginSideConfirmation();
}

void handleExitTurn() {
  uint32_t elapsed = millis() - stateAtMs;
  setMotorTarget(EXIT_PWM);

  if (elapsed < EXIT_COUNTERSTEER_MS) {
    setSteeringOffset((int16_t)-trackDirection * EXIT_COUNTERSTEER_DEG);
  } else {
    setSteeringOffset(wallFollowingSteering());
  }

  if (elapsed < EXIT_RECOVERY_MS) return;

  frontCloseStreak = 0;
  if (completedTurns >= TOTAL_TURNS) {
    changeState(STATE_FINAL_ADVANCE);
    setMotorTarget(FINAL_PWM);
  } else {
    resetSideEdgeEvidence();
    changeState(STATE_CRUISE);
    setMotorTarget(CRUISE_PWM);
  }
}

void handleFinalAdvance() {
  setMotorTarget(FINAL_PWM);
  setSteeringOffset(wallFollowingSteering());

  if ((uint32_t)(millis() - stateAtMs) >= FINAL_ADVANCE_MS) {
    setSteeringOffset(0);
    brakeMotor();
    changeState(STATE_STOPPED);
    if (SERIAL_DEBUG) Serial.println(F("META | 3 vueltas completadas"));
  }
}

void updateNavigation() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastControlMs) < CONTROL_INTERVAL_MS) return;
  lastControlMs = now;

  switch (robotState) {
    case STATE_FIND_DIRECTION:
      handleFindDirection();
      break;

    case STATE_CRUISE:
      handleCruise();
      break;

    case STATE_CONFIRM_SIDES:
      handleSideConfirmation();
      break;

    case STATE_TURNING:
      setMotorTarget(TURN_PWM);
      setTurnSteering();
      updateTurnCompletion();
      break;

    case STATE_FINE_ALIGN:
      setMotorTarget(ALIGN_PWM);
      setFineAlignmentSteering();
      updateFineAlignment();
      break;

    case STATE_EXIT_TURN:
      handleExitTurn();
      break;

    case STATE_FINAL_ADVANCE:
      handleFinalAdvance();
      break;

    case STATE_STOPPED:
    case STATE_SAFETY_STOP:
      // Estados terminales. Solo se reinician apagando y encendiendo el switch.
      break;
  }
}

// -----------------------------------------------------------------------------
// DIAGNOSTICO
// -----------------------------------------------------------------------------

void printDebug() {
  if (!SERIAL_DEBUG) return;
  uint32_t now = millis();
  if ((uint32_t)(now - lastDebugMs) < DEBUG_INTERVAL_MS) return;
  lastDebugMs = now;

  Serial.print(F("Estado="));
  Serial.print(stateName(robotState));
  Serial.print(F(" | Sentido="));
  Serial.print(directionName(trackDirection));
  Serial.print(F(" | F="));
  Serial.print(sonar[SENSOR_FRONT].rawCm);
  Serial.print('/');
  Serial.print(sonar[SENSOR_FRONT].filteredCm);
  Serial.print(F(" | L="));
  Serial.print(sonar[SENSOR_LEFT].rawCm);
  Serial.print('/');
  Serial.print(sonar[SENSOR_LEFT].filteredCm);
  Serial.print(F(" | R="));
  Serial.print(sonar[SENSOR_RIGHT].rawCm);
  Serial.print('/');
  Serial.print(sonar[SENSOR_RIGHT].filteredCm);
  Serial.print(F(" | ConfL="));
  Serial.print(decisionLeftOpenStreak);
  Serial.print('/');
  Serial.print(decisionLeftWallStreak);
  Serial.print(F(" | ConfR="));
  Serial.print(decisionRightOpenStreak);
  Serial.print('/');
  Serial.print(decisionRightWallStreak);
  Serial.print(F(" | Servo="));
  Serial.print(lastServoAngle);
  Serial.print(F(" | PWM="));
  Serial.print(currentMotorPwm);
  Serial.print('/');
  Serial.print(targetMotorPwm);
  Serial.print(F(" | Armado="));
  Serial.print(nextCornerArmed ? F("SI") : F("NO"));
  Serial.print(F(" | Esquinas="));
  Serial.print(completedTurns);
  Serial.print('/');
  Serial.println(TOTAL_TURNS);
}

// -----------------------------------------------------------------------------
// SETUP Y LOOP
// -----------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  applyForwardPwm(0);

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sonar[i].trigPin, OUTPUT);
    pinMode(sonar[i].echoPin, INPUT);
    digitalWrite(sonar[i].trigPin, LOW);
  }

  steeringServo.attach(PIN_SERVO, 500, 2500);
  setSteeringOffset(0);

  bootAtMs = millis();
  stateAtMs = bootAtMs;
  lastMotorRampMs = bootAtMs;
  nextPingAtUs = micros();
  nextCornerArmed = true; // La primera esquina queda habilitada al arrancar.

  // Arranque inmediato: el motor comienza una rampa suave sin esperar boton.
  setMotorTarget(START_PWM);

  if (SERIAL_DEBUG) {
    Serial.println(F("WRO OPEN | Inicio automatico"));
    Serial.println(F("Mega 2560 | 3x HC-SR04 | L298N | Servo Ackermann"));
    Serial.println(F("Formato sonar: lectura_cruda/mediana_filtrada en cm"));
    Serial.println(F("Giro: 1x pared->abertura; cada lateral manda su sentido"));
    Serial.println(F("Maniobra: giro anterior + ajuste + contravolante"));
  }
}

void loop() {
  updateSonarScanner();
  updateNavigation();
  updateMotorRamp();
  printDebug();
}
