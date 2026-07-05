/*
  SKRobotics WRO Future Engineers 2026 - Open Challenge base firmware
  Target: Arduino Mega 2560 + L298N + 2 HC-SR04 + MG996R + gyroscope/IMU

  Current strategy:
  1. Follow the right wall using the right ultrasonic sensor.
  2. Monitor the front ultrasonic sensor for upcoming corners.
  3. Start the turn before stopping or hitting the wall.
  4. Use gyroscope yaw when available, with timer fallback.
  5. Count corners and stop after the target count.
*/

#include <Servo.h>
#include <Wire.h>
#include <math.h>

static const uint8_t PIN_SERVO = 6;

static const uint8_t PIN_MOTOR_ENA = 5;
static const uint8_t PIN_MOTOR_IN1 = 4;
static const uint8_t PIN_MOTOR_IN2 = 3;

static const uint8_t PIN_TRIG_FRONT = 22;
static const uint8_t PIN_ECHO_FRONT = 23;

static const uint8_t PIN_TRIG_RIGHT = 24;
static const uint8_t PIN_ECHO_RIGHT = 25;

static const uint8_t PIN_START_BUTTON = 30;

static const bool WAIT_FOR_START_BUTTON = true;
static const bool MOTOR_INVERTED = false;
// Default gyro implementation assumes an MPU6050-compatible module at I2C 0x68.
// If the installed gyroscope is different, set this to false until its driver is added.
static const bool GYRO_ENABLED = true;

enum TurnDirection {
  TURN_LEFT = -1,
  TURN_RIGHT = 1
};

static const TurnDirection TURN_DIRECTION = TURN_LEFT;

static const int SERVO_CENTER = 90;
static const int SERVO_LEFT_LIMIT = 55;
static const int SERVO_RIGHT_LIMIT = 125;

static const int SPEED_STOP = 0;
static const int SPEED_SLOW = 95;
static const int SPEED_MEDIUM = 125;
static const int SPEED_FAST = 150;
static const int SPEED_TURN = 115;
static const int SPEED_FINISH = 90;

static const float TARGET_RIGHT_DISTANCE_CM = 28.0;
static const float RIGHT_CORRECTION_GAIN = 0.9;
static const float FRONT_CLEAR_CM = 35.0;
static const float FRONT_TURN_CM = 24.0;
static const float FRONT_DANGER_CM = 14.0;
static const float MAX_DISTANCE_CM = 200.0;

static const uint32_t SONAR_TIMEOUT_US = 12000;
static const uint8_t SENSOR_WARMUP_READS = 8;

static const uint32_t MIN_TURN_MS = 550;
static const uint32_t MAX_TURN_MS = 1800;
static const uint32_t REALIGN_MS = 450;
static const uint32_t FINISH_DRIVE_MS = 700;
static const uint32_t MIN_CORNER_INTERVAL_MS = 800;

static const float TURN_EXIT_YAW_DEG = 75.0;
static const uint8_t TARGET_CORNERS = 12;

static const uint8_t MPU6050_ADDR = 0x68;

enum RobotState {
  WAITING_FOR_START,
  DRIVING,
  TURNING,
  REALIGNING,
  FINISH_DRIVE,
  STOPPED
};

struct Distances {
  float front;
  float right;
};

Servo steeringServo;

static RobotState state = WAITING_FOR_START;
static Distances cm = {0.0, 0.0};
static bool gyroReady = false;
static float gyroZOffsetRaw = 0.0;
static float yawDeg = 0.0;
static float turnStartYawDeg = 0.0;
static uint32_t lastGyroUs = 0;
static uint8_t cornersDone = 0;
static uint32_t stateStartedMs = 0;
static uint32_t lastCornerCountMs = 0;
static uint32_t lastDebugMs = 0;

static void setState(RobotState next);
static void writeServoAngle(int angle);
static void motorStop();
static void motorForward(int speedPwm);
static float readUltrasonicCm(uint8_t trigPin, uint8_t echoPin);
static float filterDistance(float previous, float raw);
static void readDistances();
static void warmupDistances();
static bool writeMpuRegister(uint8_t reg, uint8_t value);
static int16_t readMpu16(uint8_t reg);
static bool initGyro();
static void updateGyro();
static float absAngleDelta(float a, float b);
static int drivingServoAngle();
static void beginTurn();
static void handleWaitingForStart();
static void handleDriving();
static void handleTurning();
static void handleRealigning();
static void handleFinishDrive();
static void printDebug();

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(PIN_MOTOR_ENA, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  pinMode(PIN_TRIG_FRONT, OUTPUT);
  pinMode(PIN_TRIG_RIGHT, OUTPUT);
  pinMode(PIN_ECHO_FRONT, INPUT);
  pinMode(PIN_ECHO_RIGHT, INPUT);

  pinMode(PIN_START_BUTTON, INPUT_PULLUP);

  steeringServo.attach(PIN_SERVO);
  writeServoAngle(SERVO_CENTER);
  motorStop();

  if (GYRO_ENABLED) {
    gyroReady = initGyro();
  }

  warmupDistances();
  lastCornerCountMs = millis();
  setState(WAITING_FOR_START);

  Serial.println("SKRobotics Open Challenge firmware loaded for Arduino Mega 2560.");
  Serial.println(gyroReady ? "Gyroscope ready." : "Gyroscope not ready; using timer fallback.");
}

void loop() {
  readDistances();
  updateGyro();

  switch (state) {
    case WAITING_FOR_START:
      handleWaitingForStart();
      break;
    case DRIVING:
      handleDriving();
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
      writeServoAngle(SERVO_CENTER);
      break;
  }

  printDebug();
}

static void setState(RobotState next) {
  state = next;
  stateStartedMs = millis();
}

static void writeServoAngle(int angle) {
  angle = constrain(angle, SERVO_LEFT_LIMIT, SERVO_RIGHT_LIMIT);
  steeringServo.write(angle);
}

static void motorStop() {
  analogWrite(PIN_MOTOR_ENA, SPEED_STOP);
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
}

static void motorForward(int speedPwm) {
  speedPwm = constrain(speedPwm, 0, 255);

  if (!MOTOR_INVERTED) {
    digitalWrite(PIN_MOTOR_IN1, HIGH);
    digitalWrite(PIN_MOTOR_IN2, LOW);
  } else {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, HIGH);
  }

  analogWrite(PIN_MOTOR_ENA, speedPwm);
}

static float readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, SONAR_TIMEOUT_US);
  if (duration == 0) {
    return MAX_DISTANCE_CM;
  }

  float distance = (duration * 0.0343f) / 2.0f;
  if (distance < 2.0f || distance > MAX_DISTANCE_CM) {
    return MAX_DISTANCE_CM;
  }

  return distance;
}

static float filterDistance(float previous, float raw) {
  if (previous <= 0.1f) {
    return raw;
  }

  if (raw > previous + 70.0f) {
    return previous * 0.80f + raw * 0.20f;
  }

  return previous * 0.65f + raw * 0.35f;
}

static void readDistances() {
  float frontRaw = readUltrasonicCm(PIN_TRIG_FRONT, PIN_ECHO_FRONT);
  delay(6);
  float rightRaw = readUltrasonicCm(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT);

  cm.front = filterDistance(cm.front, frontRaw);
  cm.right = filterDistance(cm.right, rightRaw);
}

static void warmupDistances() {
  cm.front = 0.0f;
  cm.right = 0.0f;

  for (uint8_t i = 0; i < SENSOR_WARMUP_READS; i++) {
    readDistances();
    delay(20);
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
  if (Wire.endTransmission(false) != 0) {
    return 0;
  }

  int bytesRead = Wire.requestFrom(MPU6050_ADDR, (uint8_t)2);
  if (bytesRead != 2) {
    return 0;
  }

  uint8_t high = Wire.read();
  uint8_t low = Wire.read();
  return (int16_t)((high << 8) | low);
}

static bool initGyro() {
  Wire.begin();
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
  if (!gyroReady) {
    return;
  }

  uint32_t now = micros();
  float dt = (now - lastGyroUs) / 1000000.0f;
  lastGyroUs = now;

  int16_t rawZ = readMpu16(0x47);
  float zDps = ((float)rawZ - gyroZOffsetRaw) / 131.0f;
  yawDeg += zDps * dt;

  while (yawDeg > 180.0f) yawDeg -= 360.0f;
  while (yawDeg < -180.0f) yawDeg += 360.0f;
}

static float absAngleDelta(float a, float b) {
  float d = a - b;
  while (d > 180.0f) d -= 360.0f;
  while (d < -180.0f) d += 360.0f;
  return fabs(d);
}

static int drivingServoAngle() {
  float error = cm.right - TARGET_RIGHT_DISTANCE_CM;
  float correction = constrain(error * RIGHT_CORRECTION_GAIN, -18.0f, 18.0f);
  int angle = SERVO_CENTER + (int)correction;
  return constrain(angle, SERVO_LEFT_LIMIT, SERVO_RIGHT_LIMIT);
}

static void beginTurn() {
  turnStartYawDeg = yawDeg;
  setState(TURNING);
}

static void handleWaitingForStart() {
  motorStop();
  writeServoAngle(SERVO_CENTER);

  if (!WAIT_FOR_START_BUTTON) {
    setState(DRIVING);
    return;
  }

  if (digitalRead(PIN_START_BUTTON) == LOW) {
    delay(40);
    if (digitalRead(PIN_START_BUTTON) == LOW) {
      while (digitalRead(PIN_START_BUTTON) == LOW) {
        delay(5);
      }
      setState(DRIVING);
    }
  }
}

static void handleDriving() {
  bool frontTurn = cm.front < FRONT_TURN_CM;
  bool frontDanger = cm.front < FRONT_DANGER_CM;
  bool cornerIntervalOk = millis() - lastCornerCountMs >= MIN_CORNER_INTERVAL_MS;

  if ((frontTurn || frontDanger) && cornerIntervalOk) {
    beginTurn();
    return;
  }

  writeServoAngle(drivingServoAngle());
  motorForward(cm.front > FRONT_CLEAR_CM ? SPEED_FAST : SPEED_MEDIUM);
}

static void handleTurning() {
  uint32_t elapsed = millis() - stateStartedMs;

  if (TURN_DIRECTION == TURN_RIGHT) {
    writeServoAngle(SERVO_RIGHT_LIMIT);
  } else {
    writeServoAngle(SERVO_LEFT_LIMIT);
  }

  motorForward(SPEED_TURN);

  bool minTimeOk = elapsed >= MIN_TURN_MS;
  bool maxTime = elapsed >= MAX_TURN_MS;
  bool yawOk = gyroReady && absAngleDelta(yawDeg, turnStartYawDeg) >= TURN_EXIT_YAW_DEG;
  bool timerFallbackOk = !gyroReady && elapsed >= ((MIN_TURN_MS + MAX_TURN_MS) / 2);
  bool frontSafe = cm.front > FRONT_DANGER_CM;

  if ((minTimeOk && frontSafe && (yawOk || timerFallbackOk)) || maxTime) {
    setState(REALIGNING);
  }
}

static void handleRealigning() {
  writeServoAngle(SERVO_CENTER);
  motorForward(SPEED_MEDIUM);

  uint32_t elapsed = millis() - stateStartedMs;
  bool cornerIntervalOk = millis() - lastCornerCountMs >= MIN_CORNER_INTERVAL_MS;

  if (elapsed >= REALIGN_MS && cornerIntervalOk) {
    cornersDone++;
    lastCornerCountMs = millis();

    if (cornersDone >= TARGET_CORNERS) {
      setState(FINISH_DRIVE);
    } else {
      setState(DRIVING);
    }
  }
}

static void handleFinishDrive() {
  writeServoAngle(SERVO_CENTER);
  motorForward(SPEED_FINISH);

  if (millis() - stateStartedMs >= FINISH_DRIVE_MS) {
    motorStop();
    writeServoAngle(SERVO_CENTER);
    setState(STOPPED);
  }
}

static void printDebug() {
  if (millis() - lastDebugMs < 250) {
    return;
  }

  lastDebugMs = millis();
  Serial.print("state=");
  Serial.print(state);
  Serial.print(" front=");
  Serial.print(cm.front, 1);
  Serial.print(" right=");
  Serial.print(cm.right, 1);
  Serial.print(" yaw=");
  Serial.print(yawDeg, 1);
  Serial.print(" corners=");
  Serial.println(cornersDone);
}
