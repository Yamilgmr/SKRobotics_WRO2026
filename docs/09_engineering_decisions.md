# 9. Engineering Decisions

This document records current design reasoning. More detailed entries are stored in `engineering-journal/decision-log.md`.

## Decision: Use Arduino Mega 2560

Reason:

- Many pins for sensors and actuators.
- Familiar Arduino development workflow.
- Enough resources for ultrasonic sensing and servo control.

Trade-off:

- Limited processing for advanced computer vision.
- Future vision may require a dedicated sensor or SBC.

## Decision: Start With Ultrasonic Wall Following

Reason:

- Hardware is already available.
- Simple to test.
- Good baseline for Open Challenge movement.

Trade-off:

- Ultrasonic readings are noisy on angled surfaces.
- No color classification.
- No direct yaw measurement.

## Decision: Use L298N For The First Motor-Control Prototype

Reason:

- The L298N is available to the team.
- It is simple to integrate with Arduino Mega using PWM and direction pins.
- It allows the robot to begin motion testing before choosing a more efficient driver.

Trade-off:

- The L298N is less efficient than modern MOSFET-based drivers.
- Voltage drop and heat may reduce the robot's speed and consistency.
- The team must document PWM response and heat behavior to decide if it remains acceptable.

## Decision: Use HuskyLens For Obstacle Color Detection

Reason:

- Ultrasonic sensors can measure distance but cannot classify red and green traffic signs.
- HuskyLens can detect visual features and send compact information to the Arduino.
- This keeps the Arduino Mega as the main controller while adding color/object perception.

Trade-off:

- The camera must be mounted rigidly and tested under competition lighting.
- The team must decide the communication mode between HuskyLens and Arduino.
- Parking detection is still unresolved and needs a separate decision.

## Decision: Document Missing Systems Honestly

Reason:

- WRO documentation rewards engineering reasoning, testing, and reproducibility.
- A clear risk list is more useful than pretending the robot is complete.

Trade-off:

- The repository shows incomplete items early in the season.
- The team must keep updating evidence as the robot improves.

