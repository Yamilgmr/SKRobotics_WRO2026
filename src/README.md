# Source Code

This folder contains Arduino Mega sketches for the SKRobotics WRO 2026 vehicle.

## Required Arduino Setup

- Arduino IDE.
- Board: `Arduino Mega or Mega 2560`.
- Processor: `ATmega2560`.
- `Servo`, included with the Arduino IDE.

The Open Challenge firmware uses `Servo` for the MG996R, `analogWrite()` for the L298N ENA pin, and timed `pulseIn()` ultrasonic reads for the three HC-SR04 sensors.

## Sketches

| Path | Purpose |
| --- | --- |
| `SKRobotics_OpenChallenge/SKRobotics_OpenChallenge.ino` | Current Arduino Mega Open Challenge firmware using front/right/left ultrasonic sensing, high-speed side-opening turn detection, 12-corner counting, final run, active brake, and final stop behavior |
| `SKRobotics_ObstacleChallenge/SKRobotics_ObstacleChallenge.ino` | Placeholder architecture for future HuskyLens red/green obstacle and parking strategy |
| `calibration/ultrasonic_sensor_test/ultrasonic_sensor_test.ino` | Tests front, right, and left ultrasonic readings |
| `calibration/servo_sweep_test/servo_sweep_test.ino` | Finds safe MG996R steering center and limits |
| `calibration/motor_driver_test/motor_driver_test.ino` | Arduino Mega L298N motor driver test, disabled by default |

## Safety

The DC motor must not be connected directly to Arduino pins. The current Open Challenge firmware uses the L298N on D5, D6, and D7 and starts moving immediately after power-on.

Keep the wheels lifted during first upload and motor tests. The calibration motor test remains disabled by default until the team intentionally enables it.
