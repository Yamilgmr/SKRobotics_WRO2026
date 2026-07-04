/*
  SKRobotics WRO Future Engineers 2026 - Open Challenge base firmware
  Robot: ESP32 Acebott / ESP32 Dev Module + MPU6050 + 3 HC-SR04 + MG996R + L298N + DC motor

  Strategy:
  1. Drive straight while the front path is clear.
  2. Detect a corner when a side wall disappears and an opening is confirmed.
  3. Steer into the opening without stopping.
  4. Realign after the turn.
  5. Count 12 corners for 3 laps and stop in the starting quadrant.
*/

#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#if __has_include(<esp_arduino_version.h>)
#include <esp_arduino_version.h>
#endif

#ifndef ESP_ARDUINO_VERSION_MAJOR
#define ESP_ARDUINO_VERSION_MAJOR 2
#endif

static const int PIN_SERVO = 13;

static const int PIN_MOTOR_ENA = 14;
static const int PIN_MOTOR_IN1 = 32;
static const int PIN_MOTOR_IN2 = 33;

static const int PIN_TRIG_FRONT = 25;
static const int PIN_ECHO_FRONT = 34;

static const int PIN_TRIG_LEFT = 26;
static const int PIN_ECHO_LEFT = 35;

static const int PIN_TRIG_RIGHT = 27;
static const int PIN_ECHO_RIGHT = 36;

static const int PIN_I2C_SDA = 21;
static const int PIN_I2C_SCL = 22;

static const int PIN_START_BUTTON = 23;

static const bool WAIT_FOR_START_BUTTON = true;
static const uint32_t AUTO_START_DELAY_MS = 0;

static const bool MOTOR_INVERTED = false;

enum TurnSide {
  SIDE_UNKNOWN,
  SIDE_LEFT,
  SIDE_RIGHT
};

static const TurnSide FORCED_TURN_SIDE = SIDE_UNKNOWN;

static const int SERVO_LEFT_ANGLE = 0;
static const int SERVO_CENTER_ANGLE = 45;
static const int SERVO_RIGHT_ANGLE = 90;

static const int SERVO_US_LEFT = 1000;
static const int SERVO_US_CENTER = 1500;
static const int SERVO_US_RIGHT = 2000;

static const int SPEED_STOP = 0;
static const int SPEED_SLOW = 95;
static const int SPEED_MEDIUM = 125;
static const int SPEED_FAST = 155;
static const int SPEED_TURN = 115;
static const int SPEED_FINISH = 90;

static const float FRONT_CLEAR_CM = 22.0;
static const float FRONT_DANGER_CM = 12.0;
static const float WALL_PRESENT_CM = 55.0;
static const float WALL_LOST_CM = 85.0;
static const float MAX_DISTANCE_CM = 200.0;

static const int OPEN_CONFIRM_READS = 3;
static const uint32_t SONAR_TIMEOUT_US = 12000;
static const uint8_t SENSOR_WARMUP_READS = 8;

static const uint32_t MIN_TURN_MS = 650;
static const uint32_t MAX_TURN_MS = 2200;
static const uint32_t TURN_SENSOR_BACKUP_MS = 1050;
static const uint32_t REALIGN_MS = 550;
static const uint32_t FINISH_DRIVE_MS = 700;
static const uint32_t MIN_CORNER_INTERVAL_MS = 900;

static const float TURN_EXIT_YAW_DEG = 65.0;

static const int PWM_SERVO_CH = 0;
static const int PWM_MOTOR_CH = 1;
static const int PWM_SERVO_BITS = 16;
static const int PWM_MOTOR_BITS = 8;
static const int PWM_SERVO_FREQ = 50;
static const int PWM_MOTOR_FREQ = 5000;

static const uint8_t MPU6050_ADDR = 0x68;
static bool gyroReady = false;
static float gyroZOffsetRaw = 0.0;
static float yawDeg = 0.0;
static uint32_t lastGyroUs = 0;

enum RobotState {
  WAITING_FOR_START,
  STRAIGHT,
  TURNING,
  REALIGNING,
  FINISH_DRIVE,
  STOPPED
};

struct Distances {
  float front;
  float left;
  float right;
};

static RobotState state = WAITING_FOR_START;
static TurnSide trackTurnSide = SIDE_UNKNOWN;
static TurnSide activeTurnSide = SIDE_UNKNOWN;

static Distances cm = {0.0, 0.0, 0.0};
static int leftOpenCount = 0;
static int rightOpenCount = 0;
static bool leftWallSeen = false;
static bool rightWallSeen = false;

static int cornersDone = 0;
static uint32_t stateStartedMs = 0;
static uint32_t lastDebugMs = 0;
static uint32_t lastCornerCountMs = 0;
static float turnStartYawDeg = 0.0;

static void pwmAttachCompat(int pin, int channel, int freq, int bits) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  (void)channel;
  ledcAttach(pin, freq, bits);
#else
  ledcSetup(channel, freq, bits);
  ledcAttachPin(pin, channel);
#endif
}

static void pwmWriteCompat(int pin, int channel, int duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  (void)channel;
  ledcWrite(pin, duty);
#else
  ledcWrite(channel, duty);
#endif
}

static int clampInt(int value, int low, int high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

static float clampFloat(float value, float low, float high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}

static float absAngleDelta(float a, float b) {
  float d = a - b;
  while (d > 180.0f) d -= 360.0f;
  while (d < -180.0f) d += 360.0f;
  return fabs(d);
}

static void setState(RobotState next) {
  state = next;
  stateStartedMs = millis();
}

static int servoAngleToMicros(int robotAngle) {
  robotAngle = clampInt(robotAngle, SERVO_LEFT_ANGLE, SERVO_RIGHT_ANGLE);

  if (robotAngle <= SERVO_CENTER_ANGLE) {
    return map(robotAngle, SERVO_LEFT_ANGLE, SERVO_CENTER_ANGLE, SERVO_US_LEFT, SERVO_US_CENTER);
  }

  return map(robotAngle, SERVO_CENTER_ANGLE, SERVO_RIGHT_ANGLE, SERVO_US_CENTER, SERVO_US_RIGHT);
}

static void writeServoAngle(int robotAngle) {
  int us = servoAngleToMicros(robotAngle);
  int duty = (int)((uint32_t)us * ((1UL << PWM_SERVO_BITS) - 1) / 20000UL);
  pwmWriteCompat(PIN_SERVO, PWM_SERVO_CH, duty);
}

static void motorStop() {
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  pwmWriteCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, SPEED_STOP);
}

static void motorForward(int speedPwm) {
  speedPwm = clampInt(speedPwm, 0, 255);

  if (!MOTOR_INVERTED) {
    digitalWrite(PIN_MOTOR_IN1, HIGH);
    digitalWrite(PIN_MOTOR_IN2, LOW);
  } else {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, HIGH);
  }

  pwmWriteCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, speedPwm);
}

static float readUltrasonicCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, SONAR_TIMEOUT_US);
  if (duration == 0) return MAX_DISTANCE_CM;

  float distance = (duration * 0.0343f) / 2.0f;
  if (distance < 2.0f || distance > MAX_DISTANCE_CM) return MAX_DISTANCE_CM;
  return distance;
}

static float filterDistance(float previous, float raw) {
  if (previous <= 0.1f) return raw;

  if (raw > previous + 70.0f) {
    return previous * 0.80f + raw * 0.20f;
  }

  return previous * 0.65f + raw * 0.35f;
}

static void readDistances() {
  float f = readUltrasonicCm(PIN_TRIG_FRONT, PIN_ECHO_FRONT);
  delay(6);
  float l = readUltrasonicCm(PIN_TRIG_LEFT, PIN_ECHO_LEFT);
  delay(6);
  float r = readUltrasonicCm(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT);

  cm.front = filterDistance(cm.front, f);
  cm.left = filterDistance(cm.left, l);
  cm.right = filterDistance(cm.right, r);
}

static void warmupDistances() {
  cm.front = 0.0f;
  cm.left = 0.0f;
  cm.right = 0.0f;

  for (uint8_t i = 0; i < SENSOR_WARMUP_READS; i++) {
    readDistances();
    delay(20);
  }

  leftWallSeen = cm.left < WALL_PRESENT_CM;
  rightWallSeen = cm.right < WALL_PRESENT_CM;
  leftOpenCount = 0;
  rightOpenCount = 0;
}

static void updateOpeningDetectors() {
  if (cm.left < WALL_PRESENT_CM) {
    leftWallSeen = true;
    leftOpenCount = 0;
  } else if (leftWallSeen && cm.left > WALL_LOST_CM) {
    leftOpenCount++;
  } else {
    leftOpenCount = 0;
  }

  if (cm.right < WALL_PRESENT_CM) {
    rightWallSeen = true;
    rightOpenCount = 0;
  } else if (rightWallSeen && cm.right > WALL_LOST_CM) {
    rightOpenCount++;
  } else {
    rightOpenCount = 0;
  }
}

static bool writeMpuRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

static int16_t readMpu16(uint8_t reg) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return 0;

  int bytesRead = Wire.requestFrom(MPU6050_ADDR, (uint8_t)2);
  if (bytesRead != 2) return 0;

  uint8_t high = Wire.read();
  uint8_t low = Wire.read();
  return (int16_t)((high << 8) | low);
}

static bool initGyro() {
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  Wire.setClock(400000);

  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() != 0) {
    return false;
  }

  writeMpuRegister(0x6B, 0x00);
  delay(100);
  writeMpuRegister(0x1B, 0x00);

  long sum = 0;
  const int samples = 600;
  for (int i = 0; i < samples; i++) {
    sum += readMpu16(0x47);
    delay(2);
  }

  gyroZOffsetRaw = (float)sum / (float)samples;
  yawDeg = 0.0f;
  lastGyroUs = micros();
  return true;
}

static void updateGyro() {
  if (!gyroReady) return;

  uint32_t now = micros();
  float dt = (now - lastGyroUs) / 1000000.0f;
  lastGyroUs = now;

  int16_t rawZ = readMpu16(0x47);
  float zDps = ((float)rawZ - gyroZOffsetRaw) / 131.0f;
  yawDeg += zDps * dt;

  while (yawDeg > 180.0f) yawDeg -= 360.0f;
  while (yawDeg < -180.0f) yawDeg += 360.0f;
}

static void beginTurn(TurnSide side) {
  if (FORCED_TURN_SIDE != SIDE_UNKNOWN) {
    side = FORCED_TURN_SIDE;
  }

  activeTurnSide = side;

  if (trackTurnSide == SIDE_UNKNOWN) {
    trackTurnSide = side;
  }

  turnStartYawDeg = yawDeg;
  leftOpenCount = 0;
  rightOpenCount = 0;
  setState(TURNING);
}

static int straightServoAngle() {
  int angle = SERVO_CENTER_ANGLE;

  bool bothWalls = cm.left < WALL_LOST_CM && cm.right < WALL_LOST_CM;
  if (bothWalls) {
    float error = cm.right - cm.left;
    float correction = clampFloat(error * 0.10f, -10.0f, 10.0f);
    angle = SERVO_CENTER_ANGLE + (int)correction;
  }

  return clampInt(angle, 32, 58);
}

static void handleStraight() {
  updateOpeningDetectors();

  bool leftConfirmed = leftOpenCount >= OPEN_CONFIRM_READS;
  bool rightConfirmed = rightOpenCount >= OPEN_CONFIRM_READS;

  if (trackTurnSide == SIDE_LEFT) {
    rightConfirmed = false;
  } else if (trackTurnSide == SIDE_RIGHT) {
    leftConfirmed = false;
  }

  if (trackTurnSide == SIDE_UNKNOWN && leftConfirmed && rightConfirmed) {
    if (cm.left > cm.right) {
      rightConfirmed = false;
    } else {
      leftConfirmed = false;
    }
  }

  if (rightConfirmed) {
    beginTurn(SIDE_RIGHT);
    return;
  }

  if (leftConfirmed) {
    beginTurn(SIDE_LEFT);
    return;
  }

  if (cm.front < FRONT_DANGER_CM) {
    TurnSide emergencySide = trackTurnSide;
    if (emergencySide == SIDE_UNKNOWN) {
      emergencySide = (cm.right > cm.left) ? SIDE_RIGHT : SIDE_LEFT;
    }
    beginTurn(emergencySide);
    return;
  }

  writeServoAngle(straightServoAngle());
  motorForward(cm.front > FRONT_CLEAR_CM ? SPEED_FAST : SPEED_MEDIUM);
}

static void handleTurning() {
  uint32_t elapsed = millis() - stateStartedMs;

  if (activeTurnSide == SIDE_RIGHT) {
    writeServoAngle(SERVO_RIGHT_ANGLE);
  } else {
    writeServoAngle(SERVO_LEFT_ANGLE);
  }

  motorForward(SPEED_TURN);

  bool minTimeOk = elapsed >= MIN_TURN_MS;
  bool maxTime = elapsed >= MAX_TURN_MS;
  bool yawOk = gyroReady && absAngleDelta(yawDeg, turnStartYawDeg) >= TURN_EXIT_YAW_DEG;
  bool frontSafe = cm.front > FRONT_DANGER_CM;
  bool seesAnyWall = cm.left < WALL_PRESENT_CM || cm.right < WALL_PRESENT_CM;
  bool sensorBackupOk = elapsed >= TURN_SENSOR_BACKUP_MS && seesAnyWall;
  bool turnExitEvidence = yawOk || sensorBackupOk || (!gyroReady && seesAnyWall);

  if ((minTimeOk && frontSafe && turnExitEvidence) || maxTime) {
    setState(REALIGNING);
  }
}

static void handleRealigning() {
  writeServoAngle(SERVO_CENTER_ANGLE);
  motorForward(SPEED_MEDIUM);

  uint32_t elapsed = millis() - stateStartedMs;
  bool frontSafe = cm.front > FRONT_DANGER_CM;
  bool seesWall = cm.left < WALL_LOST_CM || cm.right < WALL_LOST_CM;
  bool cornerIntervalOk = millis() - lastCornerCountMs >= MIN_CORNER_INTERVAL_MS;

  if (elapsed >= REALIGN_MS && frontSafe && seesWall && cornerIntervalOk) {
    cornersDone++;
    lastCornerCountMs = millis();

    leftWallSeen = cm.left < WALL_PRESENT_CM;
    rightWallSeen = cm.right < WALL_PRESENT_CM;
    leftOpenCount = 0;
    rightOpenCount = 0;

    if (cornersDone >= 12) {
      setState(FINISH_DRIVE);
    } else {
      setState(STRAIGHT);
    }
  }
}

static void handleFinishDrive() {
  writeServoAngle(SERVO_CENTER_ANGLE);
  motorForward(SPEED_FINISH);

  if (millis() - stateStartedMs >= FINISH_DRIVE_MS) {
    motorStop();
    writeServoAngle(SERVO_CENTER_ANGLE);
    setState(STOPPED);
  }
}

static void waitForStart() {
  writeServoAngle(SERVO_CENTER_ANGLE);
  motorStop();

  if (!WAIT_FOR_START_BUTTON) {
    if (millis() - stateStartedMs >= AUTO_START_DELAY_MS) {
      setState(STRAIGHT);
    }
    return;
  }

  if (digitalRead(PIN_START_BUTTON) == LOW) {
    delay(40);
    if (digitalRead(PIN_START_BUTTON) == LOW) {
      while (digitalRead(PIN_START_BUTTON) == LOW) {
        delay(5);
      }
      setState(STRAIGHT);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  pinMode(PIN_TRIG_FRONT, OUTPUT);
  pinMode(PIN_TRIG_LEFT, OUTPUT);
  pinMode(PIN_TRIG_RIGHT, OUTPUT);

  pinMode(PIN_ECHO_FRONT, INPUT);
  pinMode(PIN_ECHO_LEFT, INPUT);
  pinMode(PIN_ECHO_RIGHT, INPUT);

  pinMode(PIN_START_BUTTON, INPUT_PULLUP);

  pwmAttachCompat(PIN_SERVO, PWM_SERVO_CH, PWM_SERVO_FREQ, PWM_SERVO_BITS);
  pwmAttachCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, PWM_MOTOR_FREQ, PWM_MOTOR_BITS);

  writeServoAngle(SERVO_CENTER_ANGLE);
  motorStop();

  gyroReady = initGyro();

  warmupDistances();

  if (FORCED_TURN_SIDE != SIDE_UNKNOWN) {
    trackTurnSide = FORCED_TURN_SIDE;
  }

  lastCornerCountMs = millis();
  setState(WAITING_FOR_START);
}

void loop() {
  readDistances();
  updateGyro();

  switch (state) {
    case WAITING_FOR_START:
      waitForStart();
      break;

    case STRAIGHT:
      handleStraight();
      break;

    case TURNING:
      handleTurning();
      break;

    case REALIGNING:
      handleRealigning();
      break;

    case FINISH_DRIVE:
      handleFinishDrive();
      break;

    case STOPPED:
      motorStop();
      writeServoAngle(SERVO_CENTER_ANGLE);
      break;
  }
}
