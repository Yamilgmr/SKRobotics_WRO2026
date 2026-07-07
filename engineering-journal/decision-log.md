# Decision Log

| Date | Decision | Alternatives | Reason | Evidence | Next Action |
| --- | --- | --- | --- | --- | --- |
| 2026-07-01 | Start with Arduino Mega 2560 as the first documented controller option | Arduino Uno, ESP32, Raspberry Pi | More pins and familiar Arduino workflow | Initial hardware available | Replaced by confirmed ESP32 wiring on 2026-07-04 |
| 2026-07-04 | Test ESP32 Acebott / ESP32 Dev Module as controller attempt | Arduino Mega 2560, Raspberry Pi | Matched the temporary ESP32 firmware, supported MPU6050 I2C, and provided PWM for servo and L298N control | Team confirmed a temporary ESP32 pin map | Archived after the 2026-07-05 return to Arduino Mega |
| 2026-07-05 | Return to Arduino Mega 2560 as active controller after urgent hardware constraints | Continue with ESP32, use Arduino Uno | Arduino Mega is available, 5 V logic is compatible with HC-SR04, and it has enough pins for the reduced sensor set | Team confirmed new component list: Mega, L298N, two ultrasonics, MG996R, breadboard, 7.4 V nominal battery, DC motor | Verify physical pin map and upload Mega firmware |
| 2026-07-06 | Use final two-sensor Arduino Mega code as active Open Challenge baseline | Previous Mega code with gyroscope/start button assumptions | Matches the actual wiring: servo D9, L298N D5/D6/D7, front sonar D22/D23, right sonar D24/D25 | Final code provided by team | Test on robot and add lap counting/final stop later |
| 2026-07-01 | Start with ultrasonic wall following | Camera-first, IMU-first | Uses existing sensors and gives a fast Open Challenge baseline | Current reduced setup has front and right ultrasonic sensors | Run static distance tests |
| 2026-07-04 | Use L298N for the first motor-control prototype | Replace with TB6612FNG, BTS7960, ESC, or MOSFET driver | The L298N is available and easy to test with Arduino PWM, even though it is less efficient | Current prototype photos show the L298N returned to the vehicle | Document wiring, PWM tests, and heat behavior |
| 2026-07-01 | Use prefire turns for Open Challenge | Stop-and-turn | Lower time loss if tuned well | Team strategy preference | Tune front threshold and turn time |
| 2026-07-06 | Select HuskyLens for Obstacle Challenge perception | RGB color sensor, PixyCam, geometry-only obstacle behavior | HuskyLens can classify red/green signs externally and send simplified data to Arduino Mega | Team confirmed HuskyLens camera plan | Mount HuskyLens, test communication, and collect red/green detection data |

