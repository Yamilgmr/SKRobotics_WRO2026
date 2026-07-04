# Decision Log

| Date | Decision | Alternatives | Reason | Evidence | Next Action |
| --- | --- | --- | --- | --- | --- |
| 2026-07-01 | Use Arduino Mega 2560 as main controller | Arduino Uno, ESP32, Raspberry Pi | More pins and familiar Arduino workflow | Current hardware available | Confirm final pin map |
| 2026-07-01 | Start with ultrasonic wall following | Camera-first, IMU-first | Uses existing sensors and gives a fast Open Challenge baseline | Three ultrasonic sensors available | Run static distance tests |
| 2026-07-04 | Use L298N for the first motor-control prototype | Replace with TB6612FNG, BTS7960, ESC, or MOSFET driver | The L298N is available and easy to test with Arduino, even though it is less efficient | Current prototype photos show the L298N returned to the vehicle | Document wiring, PWM tests, and heat behavior |
| 2026-07-01 | Use prefire turns for Open Challenge | Stop-and-turn | Lower time loss if tuned well | Team strategy preference | Tune front threshold and turn time |
| 2026-07-04 | Use HuskyLens for Obstacle Challenge color detection | Guess with ultrasonics, PixyCam, RGB sensors | Ultrasonics cannot identify red/green colors; HuskyLens can provide object/color information to Arduino | Team has selected the HuskyLens direction | Mount camera, choose communication mode, and collect red/green test data |

