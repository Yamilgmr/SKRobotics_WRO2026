# 9. Engineering Decisions

This document records current design reasoning. More detailed entries are stored in `engineering-journal/decision-log.md`.

## Decision: Return To Arduino Mega 2560 As Active Controller

Reason:

- The team had urgent practical constraints and needed a controller that could be wired and tested immediately.
- Arduino Mega 2560 provides many 5 V digital pins for sensors, servo signal, and L298N control.
- It is compatible with the current two-ultrasonic, MG996R, and L298N baseline.

Trade-off:

- The ESP32 pin map and level-converter work are no longer the active baseline.
- Arduino Mega has less processing capability for advanced vision.
- A future color/vision sensor must be selected for the Obstacle Challenge.

## Decision: Use Two Ultrasonic Sensors For The First Mega Baseline

Reason:

- The front sensor can detect upcoming walls.
- The right sensor can support right-wall following.
- This is enough to start Open Challenge tuning with the available hardware.

Trade-off:

- No left-side distance measurement.
- Recovery after corners is harder.
- The robot depends more on ultrasonic thresholds and careful timing.

## Decision: Use L298N For The First Motor-Control Prototype

Reason:

- The L298N is available to the team.
- It is simple to integrate with Arduino Mega PWM and direction pins.
- It allows the robot to begin motion testing before choosing a more efficient driver.

Trade-off:

- The L298N is less efficient than modern MOSFET-based drivers.
- Voltage drop and heat may reduce the robot's speed and consistency.
- The team must document PWM response and heat behavior to decide if it remains acceptable.

## Decision: Document Missing Obstacle Perception Honestly

Reason:

- The current component list does not include a color or vision sensor.
- WRO documentation rewards engineering reasoning, testing, and reproducibility.
- A clear risk list is more useful than pretending the robot is complete.

Trade-off:

- The repository shows incomplete items early in the season.
- The team must choose a red/green detection method before the Obstacle Challenge can be competitive.
