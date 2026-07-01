# Decision Log

| Date | Decision | Alternatives | Reason | Evidence | Next Action |
| --- | --- | --- | --- | --- | --- |
| 2026-07-01 | Use Arduino Mega 2560 as main controller | Arduino Uno, ESP32, Raspberry Pi | More pins and familiar Arduino workflow | Current hardware available | Confirm final pin map |
| 2026-07-01 | Start with ultrasonic wall following | Camera-first, IMU-first | Uses existing sensors and gives a fast Open Challenge baseline | Three ultrasonic sensors available | Run static distance tests |
| 2026-07-01 | Remove L298N from final design | Keep L298N | L298N is inefficient and loses voltage | Team already removed it | Select replacement motor driver |
| 2026-07-01 | Use prefire turns for Open Challenge | Stop-and-turn | Lower time loss if tuned well | Team strategy preference | Tune front threshold and turn time |
| 2026-07-01 | Mark Obstacle Challenge as pending sensor choice | Guess with ultrasonics | Ultrasonics cannot identify red/green colors | Hardware limitation | Choose color/vision sensor |

