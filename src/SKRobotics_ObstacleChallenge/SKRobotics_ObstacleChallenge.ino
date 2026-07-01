#include <Servo.h>
#include <NewPing.h>

/*
  SKRobotics WRO 2026 - Obstacle Challenge architecture placeholder

  This sketch documents the intended architecture for the Obstacle Challenge.
  It is not a final competition solution because the current hardware cannot
  classify red and green traffic signs. Add a color or vision sensor before
  implementing the obstacle behavior.
*/

const bool MOTOR_OUTPUT_ENABLED = false;

const uint8_t SERVO_PIN = 6;
const uint8_t MOTOR_PWM_PIN = 5;
const uint8_t MOTOR_DIR_PIN = 4;
const uint8_t STATUS_LED_PIN = 13;

Servo steeringServo;

enum ObstacleColor {
  NO_OBSTACLE,
  RED_OBSTACLE,
  GREEN_OBSTACLE,
  UNKNOWN_OBSTACLE
};

enum RobotState {
  WAIT_FOR_SENSOR_UPGRADE,
  FOLLOW_LANE,
  CLASSIFY_OBSTACLE,
  EVADE_RED,
  EVADE_GREEN,
  RECOVER_LANE,
  SEARCH_PARKING,
  PARKING_MANEUVER,
  FINISHED
};

RobotState state = WAIT_FOR_SENSOR_UPGRADE;

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  steeringServo.attach(SERVO_PIN);
  steeringServo.write(90);
  stopMotor();

  Serial.println("Obstacle Challenge placeholder loaded.");
  Serial.println("Add a red/green vision or color sensor before competition use.");
}

void loop() {
  switch (state) {
    case WAIT_FOR_SENSOR_UPGRADE:
      digitalWrite(STATUS_LED_PIN, millis() / 500 % 2);
      stopMotor();
      break;
    case FOLLOW_LANE:
      followLane();
      break;
    case CLASSIFY_OBSTACLE:
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
  // TODO: Implement after selecting PixyCam, RGB camera, or another color sensor.
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
  // TODO: Reuse Open Challenge wall following or replace with IMU/vision lane logic.
}

void handleRedObstacle() {
  // WRO rule target: red traffic signs must be passed on the correct side.
  // TODO: Add steering path after red detection is calibrated.
  state = RECOVER_LANE;
}

void handleGreenObstacle() {
  // WRO rule target: green traffic signs must be passed on the correct side.
  // TODO: Add steering path after green detection is calibrated.
  state = RECOVER_LANE;
}

void recoverLane() {
  // TODO: Recenter after obstacle evasion.
  state = FOLLOW_LANE;
}

void searchParkingBox() {
  // TODO: Detect magenta parking markers or use another documented trigger.
  state = PARKING_MANEUVER;
}

void performParkingManeuver() {
  // TODO: Add encoder or timed parking routine after hardware is available.
  state = FINISHED;
}

void stopMotor() {
  analogWrite(MOTOR_PWM_PIN, 0);
  digitalWrite(MOTOR_DIR_PIN, LOW);
}

