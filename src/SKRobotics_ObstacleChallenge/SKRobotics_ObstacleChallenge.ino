/*
  SKRobotics WRO 2026 - Obstacle Challenge architecture placeholder

  Target controller: Arduino Mega 2560

  Current status:
  - HuskyLens is selected as the planned camera for red/green sign detection.
  - HuskyLens communication, mounting, and calibration are not implemented yet.
  - This sketch documents the future state structure without pretending the
    obstacle and parking behavior has already been tested.
*/

enum TrafficSignColor {
  NO_SIGN,
  RED_SIGN,
  GREEN_SIGN,
  UNKNOWN_SIGN
};

enum RobotState {
  WAIT_FOR_HUSKYLENS_INTEGRATION,
  FOLLOW_LANE,
  READ_TRAFFIC_SIGN,
  EVADE_RED,
  EVADE_GREEN,
  RECOVER_LANE,
  SEARCH_PARKING,
  PARKING_MANEUVER,
  FINISHED
};

RobotState state = WAIT_FOR_HUSKYLENS_INTEGRATION;

TrafficSignColor readHuskyLensColor();
void classifyAndTransition();
void followLane();
void handleRedObstacle();
void handleGreenObstacle();
void recoverLane();
void searchParkingBox();
void performParkingManeuver();
void stopRobot();

void setup() {
  Serial.begin(115200);
  stopRobot();

  Serial.println("Obstacle Challenge placeholder loaded for Arduino Mega.");
  Serial.println("HuskyLens selected. Integrate communication and calibration before driving obstacles.");
}

void loop() {
  switch (state) {
    case WAIT_FOR_HUSKYLENS_INTEGRATION:
      stopRobot();
      break;
    case FOLLOW_LANE:
      followLane();
      break;
    case READ_TRAFFIC_SIGN:
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
      stopRobot();
      break;
  }
}

TrafficSignColor readHuskyLensColor() {
  // TODO: Implement after HuskyLens mounting, communication, and color recognition are tested.
  return NO_SIGN;
}

void classifyAndTransition() {
  TrafficSignColor color = readHuskyLensColor();

  if (color == RED_SIGN) {
    state = EVADE_RED;
  } else if (color == GREEN_SIGN) {
    state = EVADE_GREEN;
  } else {
    state = FOLLOW_LANE;
  }
}

void followLane() {
  // TODO: Reuse the Open Challenge lane-following baseline.
}

void handleRedObstacle() {
  // TODO: Add red sign evasion path after detection is calibrated.
  state = RECOVER_LANE;
}

void handleGreenObstacle() {
  // TODO: Add green sign evasion path after detection is calibrated.
  state = RECOVER_LANE;
}

void recoverLane() {
  // TODO: Recenter after obstacle evasion.
  state = FOLLOW_LANE;
}

void searchParkingBox() {
  // TODO: Decide whether parking detection uses HuskyLens, distance geometry, timing, or a combined method.
  state = PARKING_MANEUVER;
}

void performParkingManeuver() {
  // TODO: Add parking motion after the detection method is selected.
  state = FINISHED;
}

void stopRobot() {
  // TODO: Connect this to the final motor/servo output helpers after obstacle code is implemented.
}
