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

The WRO 2026 rules evaluate not only the final vehicle performance, but also the quality of the engineering process. For that reason, this repository is structured around the five documentation criteria used in the engineering journal and GitHub assessment:

1. Mobility and mechanical design.
2. Power and sensor architecture.
3. Software architecture and obstacle strategy.
4. System thinking and engineering decisions.
5. Reproducibility and GitHub repository quality.

Our current vehicle is in an early development stage. The repository already contains the required WRO folder structure, starter Arduino code, proposed test routines, wiring placeholders, strategy documents, engineering journal templates, and the current six-view vehicle photo set. CAD files, videos, exact measurements, diagrams, and final calibration data will be added as the physical robot evolves.

## Current Hardware Baseline

The current prototype uses the following parts:

| Subsystem | Current Part | Purpose | Status |
| --- | --- | --- | --- |
| Main controller | Arduino Mega 2560 | Reads sensors and controls actuators | Selected |
| Distance sensing | 3 ultrasonic sensors | Front, left, and right wall distance estimation | Selected |
| Steering | AD002 servo | Front axle steering | Selected |
| Propulsion | DC motor | Rear drive motor | Selected |
| Battery | 3 x 3.7 V battery holder | Main power source | Selected |
| Motor driver | L298N | DC motor direction and PWM control | Selected for first prototype |
| Vision | HuskyLens AI camera | Planned red/green obstacle color recognition | Selected for Obstacle Challenge |

Important safety note: a DC motor must not be powered directly from Arduino pins. The team will use an L298N motor driver for the first working prototype because it is available and simple to wire with Arduino. The L298N is not the most efficient driver, so its voltage drop, heat, and current limits must be documented during testing. Until final wiring is checked, the firmware keeps motor output disabled by default.

## Technical Direction

For the Open Challenge, our first strategy is ultrasonic wall following with predictive corner steering. Instead of stopping at each corner, the robot should detect the front wall early and prefire the steering angle while continuing to move. The goal is to reduce time lost in turns and keep the vehicle fluid through the lap.

The planned behavior is:

1. Follow the selected side wall using left or right ultrasonic distance.
2. Monitor the front ultrasonic sensor for an upcoming corner.
3. Start the steering command before the robot reaches the wall.
4. Hold the turn for a calibrated time window.
5. Recenter using side distance after the corner.
6. Count turns to estimate laps.

This is a practical first approach for the hardware currently available. It is not the final competition strategy because three ultrasonic sensors alone cannot reliably solve all WRO 2026 requirements. To reach a stronger score, we plan to add or evaluate:

- An IMU for yaw-based 90 degree turns and drift correction.
- An encoder for repeatable distance and parking movement.
- HuskyLens color/block recognition for obstacle challenge red and green pillar classification.
- A tested L298N wiring and power setup with repeatable speed control.

For the Obstacle Challenge, the planned strategy is to use the HuskyLens AI camera to detect the red and green blocks and send the classification result to the Arduino Mega. The Arduino will use that information to decide the correct evasion side. The final parking strategy is still under research and will be documented after the team chooses how to detect or approach the magenta parking box.

## WRO Repository Structure

This repository follows the public WRO Future Engineers template structure and expands it with engineering documents:

| Path | Purpose |
| --- | --- |
| `t-photos/` | Official and informal team photos. |
| `v-photos/` | Six required vehicle photos: front, back, left, right, top, and bottom. |
| `video/` | Links to driving demonstration videos for each challenge. |
| `schemes/` | Wiring diagrams, electromechanical diagrams, and power architecture. |
| `src/` | Arduino firmware and calibration sketches. |
| `models/` | CAD, STL, laser-cut, or 3D-print files. |
| `docs/` | Technical documentation by subsystem and strategy. |
| `engineering-journal/` | Decision log, iteration notes, and test records. |
| `data/` | Calibration data, sensor readings, and test measurements. |
| `other/` | Build notes, operation manuals, references, and additional resources. |

The structure is intentionally explicit. A judge or another team should be able to start from the README, open the relevant subsystem document, find the related code or diagram, and understand both what we built and why we built it that way.

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

- [Open Challenge starter firmware](src/SKRobotics_OpenChallenge/SKRobotics_OpenChallenge.ino)
- [Obstacle Challenge placeholder firmware](src/SKRobotics_ObstacleChallenge/SKRobotics_ObstacleChallenge.ino)
- [Ultrasonic sensor test](src/calibration/ultrasonic_sensor_test/ultrasonic_sensor_test.ino)
- [Servo sweep test](src/calibration/servo_sweep_test/servo_sweep_test.ino)
- [Motor driver test placeholder](src/calibration/motor_driver_test/motor_driver_test.ino)
- [Source code notes](src/README.md)

## Firmware Philosophy

The starter firmware is designed around a finite state machine. That makes the robot behavior easier to explain, test, and improve. The Open Challenge sketch separates these responsibilities:

- `WAIT_FOR_START`: keep the robot still until the start button is pressed.
- `FOLLOW_WALL`: drive forward and use side distance to stay near the target lane.
- `PREFIRE_TURN`: start the turn before hitting the front wall.
- `RECENTER`: return the steering toward the lane after a corner.
- `FINISHED`: stop after the estimated number of turns for three laps.

The first version avoids depending on unfinished calibration data. It uses the Arduino Mega, three ultrasonic sensors, steering servo, and a motor driver interface that will be wired to the L298N after the final pin map is confirmed. Constants are grouped at the top of the sketch so testing can be done methodically.

The firmware also includes measurement filtering for ultrasonic sensors. Each distance reading is sampled several times, invalid readings are rejected, and the remaining values are averaged. This is necessary because ultrasonic sensors can be noisy near angled walls, soft surfaces, or curved corners.

## Motor Driver Status

The team will use the L298N for the first prototype. This is a practical choice because it is available, familiar, and compatible with Arduino PWM/direction control. The engineering trade-off is that the L298N is inefficient compared with modern MOSFET-based drivers and can waste voltage as heat.

The L298N setup must be documented with:

- Motor voltage and expected current.
- Voltage compatibility with the battery pack.
- PWM input compatibility with Arduino Mega 5 V logic.
- Heat observations during repeated runs.
- Wiring diagram, common ground, and battery connection.
- Test results at low, medium, and high PWM.

Until this is solved, the code uses:

```cpp
const bool MOTOR_OUTPUT_ENABLED = false;
```

Set this to `true` only after the L298N is wired, the common ground is verified, and the robot has been tested safely with the wheels lifted.

## Open Challenge Scoring Focus

The Open Challenge rewards completing three laps and stopping correctly. Our first score-focused plan is:

1. Build a repeatable wall-following baseline.
2. Tune the front distance threshold for early corner entry.
3. Tune the turn duration so the robot exits close to parallel with the next wall.
4. Reduce oscillation by limiting how aggressive the side correction can be.
5. Add lap counting by turn count.
6. Add a final stop routine after twelve corners.

The team goal is to make the robot complete laps reliably before increasing speed. Once reliability is stable, the prefire turn can be made more aggressive to reduce lap time.

## Obstacle Challenge Scoring Focus

The Obstacle Challenge requires the robot to pass red and green traffic signs on the correct side and later perform parking. The team plans to use the HuskyLens AI camera for red/green block recognition and send the detected color to the Arduino Mega for decision-making. Parking is still an open design problem.

Recommended next steps:

1. Mount the HuskyLens with a stable view of the traffic signs.
2. Train and test red and green block recognition under competition lighting.
3. Define the serial/I2C communication method between HuskyLens and Arduino.
4. Add an obstacle classifier module to the Arduino firmware.
5. Add evasion states to the finite state machine.
6. Choose and document a parking detection method for the magenta parking box.

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

- Ultrasonic sensor stability at 10 cm, 20 cm, 30 cm, 40 cm, and 60 cm.
- Servo center and maximum safe left/right angles.
- Motor PWM response with the selected driver.
- Corner prefire threshold test.
- Three lap consistency test.
- Obstacle color detection test after the sensor is selected.

## Reproducibility Checklist

- [x] Public-ready WRO folder structure.
- [x] README in English with more than 5000 characters.
- [x] Team and season information.
- [x] Hardware baseline.
- [x] Initial software architecture.
- [x] Open Challenge starter code.
- [x] Calibration sketches.
- [x] Wiring and power placeholders.
- [x] Engineering journal templates.
- [x] Current vehicle photos from all required angles.
- [ ] Final wiring diagram photo/export.
- [ ] Final competition vehicle photos after wiring and mounts are finished.
- [ ] Team photo files stored directly in `t-photos/`.
- [ ] Driving video links.
- [ ] CAD or mechanical drawings.
- [ ] L298N wiring and PWM test data.
- [ ] HuskyLens color detection data.
- [ ] Parking strategy.
- [ ] Final competition code.

## Attribution

This repository was organized with reference to the official WRO Future Engineers repository template and with inspiration from strong public WRO repositories from previous seasons. The content, strategy notes, and starter code in this repository are written for SKRobotics and should be updated with the team's own test evidence throughout the season.

## Contact

Team SKRobotics  
Colegio Ingeniero Tomas Guardia  
Panama

