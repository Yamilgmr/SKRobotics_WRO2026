#include <Arduino.h>
#include <Wire.h>

/*
  SKRobotics WRO 2026 - Obstacle Challenge architecture placeholder

  Target hardware:
  - ESP32 Acebott / ESP32 Dev Module
  - MG996R steering servo on GPIO 13
  - L298N motor driver on GPIO 14, GPIO 32, GPIO 33
  - MPU6050 on I2C GPIO 21 / GPIO 22
  - HuskyLens selected for red/green traffic-sign detection, not integrated yet

  This is not a final competition solution. It documents the intended software
  structure while the camera mount, communication method, color IDs, and parking
  strategy are still being tested.
*/

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
static const int PIN_I2C_SDA = 21;
static const int PIN_I2C_SCL = 22;

static const int PWM_SERVO_CH = 0;
static const int PWM_MOTOR_CH = 1;
static const int PWM_SERVO_BITS = 16;
static const int PWM_MOTOR_BITS = 8;
static const int PWM_SERVO_FREQ = 50;
static const int PWM_MOTOR_FREQ = 5000;

static const int SERVO_CENTER_US = 1500;

enum ObstacleColor {
  NO_OBSTACLE,
  RED_OBSTACLE,
  GREEN_OBSTACLE,
  UNKNOWN_OBSTACLE
};

enum RobotState {
  WAIT_FOR_HUSKYLENS_TESTS,
  FOLLOW_LANE,
  READ_HUSKYLENS,
  EVADE_RED,
  EVADE_GREEN,
  RECOVER_LANE,
  SEARCH_PARKING,
  PARKING_MANEUVER,
  FINISHED
};

static RobotState state = WAIT_FOR_HUSKYLENS_TESTS;

static void pwmAttachCompat(int pin, int channel, int freq, int bits);
static void pwmWriteCompat(int pin, int channel, int duty);
static void writeServoPulse(int microseconds);
static void stopMotor();
ObstacleColor detectObstacleColor();
void classifyAndTransition();
void followLane();
void handleRedObstacle();
void handleGreenObstacle();
void recoverLane();
void searchParkingBox();
void performParkingManeuver();

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

static void writeServoPulse(int microseconds) {
  int duty = (int)((uint32_t)microseconds * ((1UL << PWM_SERVO_BITS) - 1) / 20000UL);
  pwmWriteCompat(PIN_SERVO, PWM_SERVO_CH, duty);
}

static void stopMotor() {
  digitalWrite(PIN_MOTOR_IN1, LOW);
  digitalWrite(PIN_MOTOR_IN2, LOW);
  pwmWriteCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, 0);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  pwmAttachCompat(PIN_SERVO, PWM_SERVO_CH, PWM_SERVO_FREQ, PWM_SERVO_BITS);
  pwmAttachCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, PWM_MOTOR_FREQ, PWM_MOTOR_BITS);

  writeServoPulse(SERVO_CENTER_US);
  stopMotor();

  Serial.println("Obstacle Challenge placeholder loaded.");
  Serial.println("HuskyLens integration, obstacle color IDs, and parking logic are still TODO.");
}

void loop() {
  switch (state) {
    case WAIT_FOR_HUSKYLENS_TESTS:
      stopMotor();
      break;
    case FOLLOW_LANE:
      followLane();
      break;
    case READ_HUSKYLENS:
      classifyAndTransition();
      break;
    case EVADE_RED:
      handleRedObstacle();
      break;
    case EVADE_GREEN:
      handleGreenObstacle();
      break;
    case RECOVER_LANE:
      recoverLane();
      break;
    case SEARCH_PARKING:
      searchParkingBox();
      break;
    case PARKING_MANEUVER:
      performParkingManeuver();
      break;
    case FINISHED:
      stopMotor();
      break;
  }
}

ObstacleColor detectObstacleColor() {
  // TODO: Implement after HuskyLens communication mode and color IDs are tested.
  return NO_OBSTACLE;
}

void classifyAndTransition() {
  ObstacleColor color = detectObstacleColor();

  if (color == RED_OBSTACLE) {
    state = EVADE_RED;
  } else if (color == GREEN_OBSTACLE) {
    state = EVADE_GREEN;
  } else {
    state = FOLLOW_LANE;
  }
}

void followLane() {
  // TODO: Reuse Open Challenge wall following and add obstacle look-ahead.
}

void handleRedObstacle() {
  // TODO: Add red obstacle steering path after HuskyLens detection is calibrated.
  state = RECOVER_LANE;
}

void handleGreenObstacle() {
  // TODO: Add green obstacle steering path after HuskyLens detection is calibrated.
  state = RECOVER_LANE;
}

void recoverLane() {
  // TODO: Recenter after obstacle evasion.
  state = FOLLOW_LANE;
}

void searchParkingBox() {
  // TODO: Decide whether parking detection uses HuskyLens, geometry, or encoder timing.
  state = PARKING_MANEUVER;
}

void performParkingManeuver() {
  // TODO: Add parking motion after the detection method is selected.
  state = FINISHED;
}
