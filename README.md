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

Our current vehicle is in an active prototype stage. After an urgent controller change, the active baseline is now Arduino Mega 2560 with three ultrasonic sensors and a HuskyLens installed for future Obstacle Challenge perception. This README reflects the current robot, not the older ESP32 wiring attempt.

## Current Hardware Baseline

The current prototype uses the following parts:

| Subsystem | Current Part | Purpose | Status |
| --- | --- | --- | --- |
| Main controller | Arduino Mega 2560 | Reads sensors, controls actuators, and runs the state machine | Selected |
| Distance sensing | 3 HC-SR04 ultrasonic sensors | Front, right, and left distance sensing | Selected |
| Steering | MG996R servo | Front axle steering | Selected |
| Propulsion | DC motor | Drive motor | Selected |
| Motor driver | L298N | DC motor direction and PWM speed control | Selected |
| Vision | HuskyLens | Planned Obstacle Challenge sign and parking-area detection | Installed, manual tests completed |
| Power | 2 x 3.7 V cells | 7.4 V nominal battery pack | Selected |
| Prototyping | Breadboard | Temporary signal and power distribution | Selected |

Important safety note: a DC motor must not be powered directly from Arduino pins. The L298N is required between the Arduino Mega and the DC motor. All electronics must share common ground, and the MG996R servo should not be powered from a weak Arduino 5 V pin if it draws high current under steering load.

## Current Pin Map

This pin map comes from the final Arduino Mega code currently used by the team. If any cable is connected differently on the physical robot, update the constants in the code and this table together.

| Component | Arduino Mega Pin | Notes |
| --- | --- | --- |
| MG996R steering servo signal | D9 | Servo PWM signal |
| L298N ENA | D5 | Motor speed PWM |
| L298N IN1 | D6 | Motor direction |
| L298N IN2 | D7 | Motor direction |
| Front HC-SR04 TRIG | D42 | Front ultrasonic trigger |
| Front HC-SR04 ECHO | D43 | Front ultrasonic echo |
| Right HC-SR04 TRIG | D46 | Right ultrasonic trigger |
| Right HC-SR04 ECHO | D47 | Right ultrasonic echo |
| Left HC-SR04 TRIG | D52 | Left ultrasonic trigger |
| Left HC-SR04 ECHO | D53 | Left ultrasonic echo |

## Technical Direction

For the Open Challenge, the current strategy is side-opening detection with predictive corner steering. The robot reads the left and right ultrasonic sensors more often than the front sensor so that a wall-to-opening transition can trigger the turn early. The front sensor is used for safety, exit validation, and corner counting support.

The planned behavior is:

1. Start driving after power-on and sensor warmup.
2. Drive forward while reading front, right, and left ultrasonic sensors.
3. Detect the driving direction from the first reliable side opening.
4. Start turns from side wall-to-opening transitions when possible.
5. Use the front ultrasonic sensor for safety and turn-exit validation.
6. Apply turn, fine-align, and countersteer phases.
7. Count 12 corners, advance through the final straight, and stop.

This is a practical approach for the hardware currently available. The Open Challenge code now uses three ultrasonic sensors and includes 12-corner counting plus final stop behavior. The HuskyLens is installed and ready for Obstacle Challenge development, but it is not integrated into the active code yet.

## Current Limitations

- HuskyLens is installed for Obstacle Challenge red/green recognition, but it is not integrated into Arduino code yet.
- Parking strategy is selected at concept level but not implemented in code yet.
- No gyroscope, encoder, start button, or status LED is used in the current code.
- The wiring diagram image is available, but physical wire-by-wire verification is still pending.
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

## Current Visual Evidence

- [Open Challenge YouTube Short](https://youtube.com/shorts/AXeQFjmq5FQ?feature=share) - team-provided autonomous driving clip. The team should confirm that the autonomous-driving portion is at least 30 seconds before treating it as the final WRO video submission.
- [Hardware close-up photos](assets/hardware_photos/README.md) - current Arduino Mega, L298N, HuskyLens, ultrasonic mount, and DC motor wiring evidence.

## Firmware Philosophy

The starter firmware is designed around a finite state machine. That makes the robot behavior easier to explain, test, and improve. The Open Challenge sketch separates these responsibilities:

- `STATE_FIND_DIRECTION`: center and detect the first reliable side opening.
- `STATE_CRUISE`: follow the track and watch side openings.
- `STATE_CONFIRM_SIDES`: confirm new side readings before committing to a turn.
- `STATE_TURNING`: execute the main turn phase.
- `STATE_FINE_ALIGN`: apply the second steering correction.
- `STATE_EXIT_TURN`: countersteer and recover into the next straight.
- `STATE_FINAL_ADVANCE`: advance after the twelfth corner.
- `STATE_STOPPED`: stop after three laps.

The current Open Challenge version uses the Arduino Mega, three ultrasonic sensors, the MG996R steering servo, and the L298N motor driver. Constants are grouped at the top of the sketch so testing can be done methodically.

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
2. Build a repeatable side-opening detection baseline.
3. Tune side-opening and front fallback thresholds for early corner entry.
4. Tune turn timing, fine alignment, and countersteer so the robot exits corners consistently.
5. Reduce oscillation by limiting lateral correction.
6. Use 12-corner counting and final advance/stop behavior from the current firmware.

The team goal is to make the robot complete laps reliably before increasing speed. Once reliability is stable, the prefire turn can be made more aggressive to reduce lap time.

## Obstacle Challenge Scoring Focus

HuskyLens is installed as the camera for Obstacle Challenge red/green recognition. The team has performed manual HuskyLens tests without Arduino code integration yet. The robot still needs reliable HuskyLens communication, mounting validation, and calibration before it can make correct evasion decisions.

Recommended next steps:

1. Stabilize Open Challenge movement first.
2. Mount the HuskyLens where it can see traffic signs before the car reaches them.
3. Add a test sketch for HuskyLens red/green recognition.
4. Add obstacle states to the finite state machine.
5. Implement the selected parking strategy: use HuskyLens to detect the parking area, drive forward until both parking walls leave the camera view, then reverse and align into the parking box.

This prevents the team from pretending an untested capability exists. Judges usually reward honest engineering reasoning more than undocumented claims.

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
- Corner prefire threshold test.
- Three lap consistency test.
- Turn counting and final stop test with the current firmware.
- Future HuskyLens red/green detection test after Arduino communication is added.

## Reproducibility Checklist

- [x] Public-ready WRO folder structure.
- [x] README in English with more than 5000 characters.
- [x] Team and season information.
- [x] Current hardware baseline.
- [x] Arduino Mega final code pin map.
- [x] Initial software architecture.
- [x] Open Challenge starter code.
- [x] Calibration sketches.
- [x] Initial real Open Challenge serial test evidence.
- [ ] Real measured calibration and test data.
- [x] Engineering journal templates.
- [x] Final vehicle photos from all required angles.
- [x] Electrical wiring diagram image.
- [ ] Physical wire-by-wire verification.
- [x] Team photo files stored directly in `t-photos/`.
- [x] Driving video link provided.
- [ ] CAD or mechanical drawings.
- [ ] L298N wiring and PWM test data.
- [x] Turn counting and automatic final stop.
- [x] Obstacle color detection hardware selected: HuskyLens.
- [ ] HuskyLens Arduino integration and red/green test data.
- [x] Parking strategy selected.
- [x] Current Open Challenge code marked as final for now.
- [ ] Obstacle and parking competition code.

## Attribution

This repository was organized with reference to the official WRO Future Engineers repository template and with inspiration from strong public WRO repositories from previous seasons. The content, strategy notes, and starter code in this repository are written for SKRobotics and should be updated with the team's own test evidence throughout the season.

## Contact

Team SKRobotics  
Colegio Ingeniero Tomas Guardia  
Panama
