# 2026-07-01 Initial Architecture

## Context

The team is preparing for WRO 2026 Future Engineers. At this point in the design process, the current robot hardware included an Arduino Mega 2560, three ultrasonic sensors, an AD002 steering servo, a DC motor, and a 3 x 3.7 V battery holder. The L298N motor driver had been removed, so motor control was treated as an unresolved subsystem.

## Design Direction

The first goal is an Open Challenge baseline using continuous wall following and predictive turns. The robot should not stop before every curve. Instead, it should detect an upcoming wall with the front ultrasonic sensor and start steering before reaching the corner.

## Main Constraint

The DC motor cannot be controlled safely until a motor driver is selected and wired correctly. The starter firmware includes motor control functions, but motor output is disabled by default.

## Update On 2026-07-04

The team decided to use the L298N again for the first motor-control prototype. This is not the most efficient motor driver, but it is available and simple to test with Arduino. The next evidence needed is the L298N wiring diagram, PWM response, and heat behavior during repeated runs.

## Next Tests

1. Verify ultrasonic sensor readings at fixed distances.
2. Find servo center and safe steering limits.
3. Verify L298N wiring and common ground.
4. Test low-speed motion with robot lifted.
5. Tune first corner prefire behavior.

