# Source Code

This folder contains ESP32 Arduino sketches for the SKRobotics WRO 2026 vehicle. The active controller target is an ESP32 Acebott / ESP32 Dev Module.

## Required Arduino/ESP32 Setup

- ESP32 board support for Arduino IDE.
- `Wire`, included with the Arduino core.

The Open Challenge firmware uses ESP32 LEDC PWM directly for the MG996R steering servo and the L298N motor driver. It uses `pulseIn()` for ultrasonic timing and `Wire` for the MPU6050.

## Sketches

| Path | Purpose |
| --- | --- |
| `SKRobotics_OpenChallenge/SKRobotics_OpenChallenge.ino` | ESP32 Open Challenge base firmware using ultrasonic opening detection, MPU6050 yaw feedback, and prefire turns |
| `SKRobotics_ObstacleChallenge/SKRobotics_ObstacleChallenge.ino` | Placeholder architecture for HuskyLens-based red/green obstacle strategy |
| `calibration/ultrasonic_sensor_test/ultrasonic_sensor_test.ino` | Prints filtered and raw-style ultrasonic readings |
| `calibration/servo_sweep_test/servo_sweep_test.ino` | Finds safe steering center and limits |
| `calibration/motor_driver_test/motor_driver_test.ino` | ESP32 L298N motor driver test, disabled by default |

## Safety

The DC motor must not be connected directly to ESP32 pins. The current Open Challenge firmware uses the L298N on GPIO 14, GPIO 32, and GPIO 33, and waits for the start button before driving:

```cpp
static const bool WAIT_FOR_START_BUTTON = true;
```

Keep the wheels lifted during first upload and motor tests. The calibration motor test remains disabled by default until the team intentionally enables it.

