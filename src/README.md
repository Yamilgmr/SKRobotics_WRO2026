# Source Code

This folder contains Arduino sketches for the SKRobotics WRO 2026 vehicle.

## Required Arduino Libraries

- `Servo`
- `NewPing`

`Servo` is included with the Arduino IDE. `NewPing` can be installed from the Arduino Library Manager.

## Sketches

| Path | Purpose |
| --- | --- |
| `SKRobotics_OpenChallenge/SKRobotics_OpenChallenge.ino` | Starter Open Challenge firmware using ultrasonic wall following and prefire turns |
| `SKRobotics_ObstacleChallenge/SKRobotics_ObstacleChallenge.ino` | Placeholder architecture for future color/vision obstacle strategy |
| `calibration/ultrasonic_sensor_test/ultrasonic_sensor_test.ino` | Prints filtered and raw-style ultrasonic readings |
| `calibration/servo_sweep_test/servo_sweep_test.ino` | Finds safe steering center and limits |
| `calibration/motor_driver_test/motor_driver_test.ino` | Placeholder motor driver test, disabled by default |

## Safety

The DC motor must not be connected directly to Arduino pins. The code keeps motor output disabled by default:

```cpp
const bool MOTOR_OUTPUT_ENABLED = false;
```

Only set it to `true` after a proper motor driver is installed and the wiring has been reviewed.

