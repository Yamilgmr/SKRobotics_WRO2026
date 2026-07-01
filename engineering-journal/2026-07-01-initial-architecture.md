# 2026-07-01 Initial Architecture

## Context

The team is preparing for WRO 2026 Future Engineers. The current robot hardware includes an Arduino Mega 2560, three ultrasonic sensors, an AD002 steering servo, a DC motor, and a 3 x 3.7 V battery holder. The L298N motor driver was removed.

## Design Direction

The first goal is an Open Challenge baseline using continuous wall following and predictive turns. The robot should not stop before every curve. Instead, it should detect an upcoming wall with the front ultrasonic sensor and start steering before reaching the corner.

## Main Constraint

The DC motor cannot be controlled safely until a replacement motor driver is selected. The starter firmware includes motor control functions, but motor output is disabled by default.

## Next Tests

1. Verify ultrasonic sensor readings at fixed distances.
2. Find servo center and safe steering limits.
3. Select motor driver.
4. Test low-speed motion with robot lifted.
5. Tune first corner prefire behavior.

