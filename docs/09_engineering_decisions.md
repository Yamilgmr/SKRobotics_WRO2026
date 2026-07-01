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

## Decision: Remove L298N But Require Replacement

Reason:

- L298N is inefficient and can reduce motor voltage.

Trade-off:

- Removing it creates a missing motor control subsystem.
- The team must choose a new driver before safe powered testing.

## Decision: Document Missing Systems Honestly

Reason:

- WRO documentation rewards engineering reasoning, testing, and reproducibility.
- A clear risk list is more useful than pretending the robot is complete.

Trade-off:

- The repository shows incomplete items early in the season.
- The team must keep updating evidence as the robot improves.

