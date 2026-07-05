/*
  SKRobotics WRO 2026 - Obstacle Challenge architecture placeholder

  Target controller: Arduino Mega 2560

  Current status:
  - The current hardware list does not include a camera or color sensor.
  - Red/green traffic-sign detection cannot be implemented reliably yet.
  - This sketch documents the future state structure without pretending the
    missing perception hardware exists.
*/

enum TrafficSignColor {
  NO_SIGN,
  RED_SIGN,
  GREEN_SIGN,
  UNKNOWN_SIGN
};

enum RobotState {
  WAIT_FOR_SENSOR_SELECTION,
  FOLLOW_LANE,
  READ_TRAFFIC_SIGN,
  EVADE_RED,
  EVADE_GREEN,
  RECOVER_LANE,
  SEARCH_PARKING,
  PARKING_MANEUVER,
  FINISHED
};

RobotState state = WAIT_FOR_SENSOR_SELECTION;

TrafficSignColor detectTrafficSignColor();
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
  Serial.println("Select and test a color/vision sensor before implementing obstacle behavior.");
}

void loop() {
  switch (state) {
    case WAIT_FOR_SENSOR_SELECTION:
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

TrafficSignColor detectTrafficSignColor() {
  // TODO: Implement after the team selects and tests color/vision hardware.
  return NO_SIGN;
}

void classifyAndTransition() {
  TrafficSignColor color = detectTrafficSignColor();

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
  // TODO: Decide whether parking detection uses color sensing, geometry, or timing.
  state = PARKING_MANEUVER;
}

void performParkingManeuver() {
  // TODO: Add parking motion after the detection method is selected.
  state = FINISHED;
}

void stopRobot() {
  // TODO: Connect this to the final motor/servo output helpers after obstacle code is implemented.
}
