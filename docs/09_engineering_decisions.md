# 9. Engineering Decisions

This document records current design reasoning. More detailed entries are stored in `engineering-journal/decision-log.md`.

## Decision: Return To Arduino Mega 2560 As Active Controller

Reason:

- The team had urgent practical constraints and needed a controller that could be wired and tested immediately.
- Arduino Mega 2560 provides many 5 V digital pins for sensors, servo signal, and L298N control.
- It is compatible with the current three-ultrasonic, MG996R, L298N, and HuskyLens-ready baseline.

Trade-off:

- The ESP32 pin map and level-converter work are no longer the active baseline.
- Arduino Mega has less processing capability for advanced vision, so HuskyLens will handle the camera-side recognition.
- HuskyLens communication and calibration still need to be integrated.

## Decision: Use Three Ultrasonic Sensors For The Current Open Challenge Baseline

Reason:

- The front sensor can support safety, fallback corner capture, and turn-exit validation.
- The right and left sensors can detect wall-to-opening transitions before the front wall becomes dangerous.
- The three-sensor layout makes left and right turn decisions more explainable than the earlier reduced two-sensor baseline.

Trade-off:

- No gyroscope or encoder is used, so the robot still depends on ultrasonic thresholds and timing.
- Three HC-SR04 sensors require careful sequencing and short timeouts to control latency and cross-talk.
- The final pin map must be checked physically against the robot before judging it as verified wiring.

## Decision: Use L298N For The First Motor-Control Prototype

Reason:

- The L298N is available to the team.
- It is simple to integrate with Arduino Mega PWM and direction pins.
- It allows the robot to begin motion testing before choosing a more efficient driver.

Trade-off:

- The L298N is less efficient than modern MOSFET-based drivers.
- Voltage drop and heat may reduce the robot's speed and consistency.
- The team must document PWM response and heat behavior to decide if it remains acceptable.

## Decision: Use HuskyLens For Obstacle Perception

Reason:

- The team selected and installed HuskyLens as the planned red/green traffic sign camera.
- HuskyLens can provide simplified recognition data to the Arduino Mega.
- The selected parking concept also uses HuskyLens to detect the parking area before reversing into the box.
- WRO documentation rewards engineering reasoning, testing, and reproducibility.
- A clear risk list is more useful than pretending the camera is already integrated.

Trade-off:

- The Obstacle Challenge strategy is now selected, but not proven.
- The team must test Arduino communication and collect real red/green and parking-wall detection data before the Obstacle Challenge can be competitive.
