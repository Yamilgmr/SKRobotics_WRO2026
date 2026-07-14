/*
  SKRobotics WRO 2026 - Obstacle Challenge architecture placeholder

  Target controller: Arduino Mega 2560

  Current status:
  - HuskyLens is installed and manually tested without Arduino code yet.
  - HuskyLens is selected for red/green sign detection and parking-area detection.
  - HuskyLens communication and calibration are not implemented yet.
  - Selected parking concept: detect the parking area, drive forward until both
    parking walls leave the camera view, then reverse and align into the box.
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
  APPROACH_PARKING_UNTIL_WALLS_LEAVE_VIEW,
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
bool parkingWallsVisible();
void approachUntilParkingWallsLeaveView();
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
    case APPROACH_PARKING_UNTIL_WALLS_LEAVE_VIEW:
      approachUntilParkingWallsLeaveView();
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
  // TODO: Use HuskyLens to detect the parking area after communication is tested.
  if (parkingWallsVisible()) {
    state = APPROACH_PARKING_UNTIL_WALLS_LEAVE_VIEW;
  }
}

bool parkingWallsVisible() {
  // TODO: Return true when HuskyLens sees the two parking walls/markers.
  return false;
}

void approachUntilParkingWallsLeaveView() {
  // TODO: Drive forward until both parking walls leave the HuskyLens point of view.
  // This should place the robot so the next step can reverse and align into the box.
  state = PARKING_MANEUVER;
}

void performParkingManeuver() {
  // TODO: Reverse and align into the parking box after the visual trigger is validated.
  state = FINISHED;
}

void stopRobot() {
  // TODO: Connect this to the final motor/servo output helpers after obstacle code is implemented.
}
