# SKRobotics WRO 2026 Future Engineers

Engineering repository for the SKRobotics self-driving vehicle for the World Robot Olympiad 2026 Future Engineers category.

Team: SKRobotics  
Country: Panama  
School: Colegio Ingeniero Tomas Guardia  
Members: Yamil Lopez and Carlo Yau  
Coach: Carolina Gamarra  
Season: WRO 2026  
Category: Future Engineers

## Repository Purpose

This repository is the main engineering documentation and software workspace for our WRO 2026 Future Engineers vehicle. It is organized to make the robot reproducible, to explain the reasoning behind our design decisions, and to keep evidence of testing, tuning, failures, and improvements during the season.

The WRO 2026 rules evaluate not only final vehicle performance, but also the quality of the engineering process. For that reason, this repository is structured around the five documentation criteria used in the engineering journal and GitHub assessment:

1. Mobility and mechanical design.
2. Power and sensor architecture.
3. Software architecture and obstacle strategy.
4. System thinking and engineering decisions.
5. Reproducibility and GitHub repository quality.

Our current vehicle is in an active prototype stage. After an urgent controller change, the active baseline is now Arduino Mega 2560 with a reduced sensor set. This README reflects the current robot, not the older ESP32 wiring attempt.

## Current Hardware Baseline

The current prototype uses the following parts:

| Subsystem | Current Part | Purpose | Status |
| --- | --- | --- | --- |
| Main controller | Arduino Mega 2560 | Reads sensors, controls actuators, and runs the state machine | Selected |
| Distance sensing | 2 HC-SR04 ultrasonic sensors | Front distance and right-wall distance | Selected |
| Orientation sensing | Gyroscope / IMU, model to confirm | Yaw estimate for turn validation | Selected |
| Steering | MG996R servo | Front axle steering | Selected |
| Propulsion | DC motor | Drive motor | Selected |
| Motor driver | L298N | DC motor direction and PWM speed control | Selected |
| Power | 2 x 3.7 V cells | 7.4 V nominal battery pack | Selected |
| Prototyping | Breadboard | Temporary signal and power distribution | Selected |

Important safety note: a DC motor must not be powered directly from Arduino pins. The L298N is required between the Arduino Mega and the DC motor. All electronics must share common ground, and the MG996R servo should not be powered from a weak Arduino 5 V pin if it draws high current under steering load.

## Draft Pin Map

The team changed hardware quickly, so this pin map is a clean draft for the current Arduino Mega baseline. It must be compared with the real wiring before upload. If any cable is already connected differently, update the constants in the code and this table together.

| Component | Arduino Mega Pin | Notes |
| --- | --- | --- |
| MG996R steering servo signal | D6 | Servo PWM signal |
| L298N ENA | D5 | Motor speed PWM |
| L298N IN1 | D4 | Motor direction |
| L298N IN2 | D3 | Motor direction |
| Front HC-SR04 TRIG | D22 | Front ultrasonic trigger |
| Front HC-SR04 ECHO | D23 | Front ultrasonic echo |
| Right HC-SR04 TRIG | D24 | Right ultrasonic trigger |
| Right HC-SR04 ECHO | D25 | Right ultrasonic echo |
| Gyroscope SDA | D20 / SDA | Arduino Mega I2C data |
| Gyroscope SCL | D21 / SCL | Arduino Mega I2C clock |
| Start button | D30 | Uses internal pull-up |

## Technical Direction

For the Open Challenge, the current strategy is right-wall following with predictive corner steering. The robot uses the right ultrasonic sensor to estimate lateral distance from the wall and the front ultrasonic sensor to detect upcoming walls. Instead of stopping before every corner, the robot should begin steering before the front wall becomes dangerous.

The planned behavior is:

1. Wait for the start button.
2. Drive forward while reading the front and right ultrasonic sensors.
3. Use the right ultrasonic sensor to correct steering during straight sections.
4. Start a continuous turn when the front ultrasonic sensor detects an upcoming corner.
5. Use the gyroscope and timing limits to decide when the turn is complete.
6. Recenter the steering after each turn.
7. Count corners until three laps are estimated, then stop.

This is a practical first approach for the hardware currently available. It is weaker than a full sensor suite because the robot no longer has a left ultrasonic sensor or a color/vision sensor. The repository documents that limitation honestly so the next engineering decisions are clear.

## Current Limitations

- Only two ultrasonic sensors are installed: front and right.
- There is no current camera or color sensor for Obstacle Challenge red/green recognition.
- Parking strategy is not selected yet.
- The exact gyroscope model and calibration constants must be confirmed.
- The final wiring diagram and real pin verification are still pending.
- Battery behavior at 7.4 V nominal must be measured under motor load.

## WRO Repository Structure

This repository follows the public WRO Future Engineers template structure and expands it with engineering documents:

| Path | Purpose |
| --- | --- |
| `t-photos/` | Official and informal team photos. |
| `v-photos/` | Six required vehicle photos: front, back, left, right, top, and bottom. |
| `video/` | Links to driving demonstration videos for each challenge. |
| `schemes/` | Wiring diagrams, electromechanical diagrams, and power architecture. |
| `src/` | Arduino Mega firmware and calibration sketches. |
| `models/` | CAD, STL, laser-cut, or 3D-print files. |
| `docs/` | Technical documentation by subsystem and strategy. |
| `engineering-journal/` | Decision log, iteration notes, and test records. |
| `data/` | Calibration data, sensor readings, and test measurements. |
| `other/` | Build notes, operation manuals, references, and additional resources. |

## Documentation Index

- [Project Overview](docs/01_project_overview.md)
- [WRO 2026 Requirements Map](docs/02_wro_2026_requirements_map.md)
- [Mechanical Mobility](docs/03_mechanical_mobility.md)
- [Power and Sensors](docs/04_power_and_sensors.md)
- [Software Architecture](docs/05_software_architecture.md)
- [Open Challenge Strategy](docs/06_open_challenge_strategy.md)
- [Obstacle Challenge Strategy](docs/07_obstacle_challenge_strategy.md)
- [Testing and Tuning Plan](docs/08_testing_and_tuning_plan.md)
- [Engineering Decisions](docs/09_engineering_decisions.md)
- [Build and Operation Manual](docs/10_build_and_operation_manual.md)
- [Risk Register](docs/11_risk_register.md)

## Source Code Index

- [Open Challenge Arduino Mega firmware](src/SKRobotics_OpenChallenge/SKRobotics_OpenChallenge.ino)
- [Obstacle Challenge placeholder firmware](src/SKRobotics_ObstacleChallenge/SKRobotics_ObstacleChallenge.ino)
- [Ultrasonic sensor test](src/calibration/ultrasonic_sensor_test/ultrasonic_sensor_test.ino)
- [Servo sweep test](src/calibration/servo_sweep_test/servo_sweep_test.ino)
- [Motor driver test](src/calibration/motor_driver_test/motor_driver_test.ino)
- [Source code notes](src/README.md)

## Firmware Philosophy

The starter firmware is designed around a finite state machine. That makes the robot behavior easier to explain, test, and improve. The Open Challenge sketch separates these responsibilities:

- `WAITING_FOR_START`: keep the robot still until the start button is pressed.
- `DRIVING`: follow the right wall and monitor front distance.
- `TURNING`: steer through the corner without stopping.
- `REALIGNING`: center the steering and stabilize after the corner.
- `FINISH_DRIVE`: move a short final distance after the target corner count.
- `STOPPED`: stop the motor and hold the steering centered.

The first version avoids depending on unfinished calibration data. It uses the Arduino Mega, two ultrasonic sensors, the MG996R steering servo, the L298N motor driver, and gyroscope yaw feedback when available. Constants are grouped at the top of the sketch so testing can be done methodically.

## Motor Driver Status

The L298N remains the first motor driver because it is available and simple to wire with Arduino Mega PWM and direction pins. The engineering trade-off is that the L298N is inefficient compared with modern MOSFET-based drivers and can waste voltage as heat.

The L298N setup must be documented with:

- Motor voltage and expected current.
- Voltage compatibility with the 7.4 V nominal battery pack.
- PWM input behavior from Arduino Mega pins.
- Heat observations during repeated runs.
- Wiring diagram, common ground, and battery connection.
- Test results at low, medium, and high PWM.

## Open Challenge Scoring Focus

The Open Challenge rewards completing three laps and stopping correctly. Our first score-focused plan is:

1. Verify the Arduino Mega pin map.
2. Build a repeatable right-wall-following baseline.
3. Tune the front distance threshold for early corner entry.
4. Tune turn timing and gyroscope threshold so the robot exits corners consistently.
5. Reduce oscillation by limiting steering correction.
6. Count turns and stop after the target corner count.

The team goal is to make the robot complete laps reliably before increasing speed. Once reliability is stable, the prefire turn can be made more aggressive to reduce lap time.

## Obstacle Challenge Scoring Focus

The current hardware does not include a camera or color sensor, so the Obstacle Challenge strategy is not complete yet. The robot needs a reliable way to identify red and green traffic signs before it can make correct evasion decisions. Parking is also still an open design problem.

Recommended next steps:

1. Stabilize Open Challenge movement first.
2. Decide what sensor will identify red and green objects.
3. Add a test sketch for the selected color/vision sensor.
4. Add obstacle states to the finite state machine.
5. Choose and document a parking detection method for the magenta parking box.

This prevents the team from pretending a missing capability exists. Judges usually reward honest engineering reasoning more than undocumented claims.

## Testing Evidence Plan

The repository includes templates for repeatable testing. Each test should record:

- Date.
- Hardware version.
- Battery voltage.
- Code version or commit hash.
- Track setup.
- Sensor thresholds.
- Result.
- Failure mode.
- Next change.

Priority tests:

- Confirm every Arduino Mega pin in the draft pin map.
- Ultrasonic sensor stability at 10 cm, 20 cm, 30 cm, 40 cm, and 60 cm.
- Servo center and maximum safe left/right angles.
- Motor PWM response with the L298N.
- Gyroscope yaw stability and turn-exit repeatability.
- Corner prefire threshold test.
- Three lap consistency test.
- Future obstacle color detection test after the sensor is selected.

## Reproducibility Checklist

- [x] Public-ready WRO folder structure.
- [x] README in English with more than 5000 characters.
- [x] Team and season information.
- [x] Current hardware baseline.
- [x] Arduino Mega draft pin map.
- [x] Initial software architecture.
- [x] Open Challenge starter code.
- [x] Calibration sketches.
- [x] Engineering journal templates.
- [x] Current vehicle photos from all required angles.
- [ ] Verified final wiring diagram.
- [ ] Final competition vehicle photos after wiring and mounts are finished.
- [ ] Team photo files stored directly in `t-photos/`.
- [ ] Driving video links.
- [ ] CAD or mechanical drawings.
- [ ] L298N wiring and PWM test data.
- [ ] Gyroscope calibration data.
- [ ] Obstacle color detection hardware and strategy.
- [ ] Parking strategy.
- [ ] Final competition code.

## Attribution

This repository was organized with reference to the official WRO Future Engineers repository template and with inspiration from strong public WRO repositories from previous seasons. The content, strategy notes, and starter code in this repository are written for SKRobotics and should be updated with the team's own test evidence throughout the season.

## Contact

Team SKRobotics  
Colegio Ingeniero Tomas Guardia  
Panama
