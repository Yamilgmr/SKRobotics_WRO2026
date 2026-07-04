# 9. Engineering Decisions

This document records current design reasoning. More detailed entries are stored in `engineering-journal/decision-log.md`.

## Decision: Use ESP32 Acebott / ESP32 Dev Module

Reason:

- Supports the confirmed current wiring and ESP32 base firmware.
- Provides PWM channels for the MG996R steering servo and L298N motor speed.
- Provides I2C for the MPU6050 and possible future HuskyLens integration.
- Still keeps an Arduino IDE workflow that the team can test quickly.

Trade-off:

- ESP32 GPIO uses 3.3 V logic, so 5 V sensor signals need level conversion.
- Some GPIO pins are input-only, so the pin map must be followed carefully.
- The team must document board selection and exact upload settings.

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
- It is simple to integrate with ESP32 PWM and direction pins.
- It allows the robot to begin motion testing before choosing a more efficient driver.

Trade-off:

- The L298N is less efficient than modern MOSFET-based drivers.
- Voltage drop and heat may reduce the robot's speed and consistency.
- The team must document PWM response and heat behavior to decide if it remains acceptable.

## Decision: Use HuskyLens For Obstacle Color Detection

Reason:

- Ultrasonic sensors can measure distance but cannot classify red and green traffic signs.
- HuskyLens can detect visual features and send compact information to the ESP32.
- This keeps the ESP32 as the main controller while adding color/object perception.

Trade-off:

- The camera must be mounted rigidly and tested under competition lighting.
- The team must decide the communication mode between HuskyLens and ESP32.
- Parking detection is still unresolved and needs a separate decision.

## Decision: Document Missing Systems Honestly

Reason:

- WRO documentation rewards engineering reasoning, testing, and reproducibility.
- A clear risk list is more useful than pretending the robot is complete.

Trade-off:

- The repository shows incomplete items early in the season.
- The team must keep updating evidence as the robot improves.

