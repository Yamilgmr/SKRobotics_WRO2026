# 11. Risk Register

| Risk | Impact | Likelihood | Mitigation |
| --- | --- | --- | --- |
| DC motor connected without driver | Damages electronics or creates unsafe current draw | Low if L298N is wired correctly | Keep the robot lifted during first motor tests and verify L298N wiring before driving |
| L298N voltage drop or overheating | Robot may be slower or inconsistent | Medium | Record PWM response, battery voltage, and heat during tests |
| Breadboard wire becomes loose | Intermittent failures during runs | Medium | Secure jumpers and document final wiring diagram |
| Ultrasonic sensor cross-talk or stale readings | Robot may detect openings or exits incorrectly | Medium | Use short pulse timeouts, side-prioritized sensor sequencing, filtering, and repeated turn tests |
| Ultrasonic readings fail at angles | Robot turns late or overcorrects | Medium | Use filtering and tune thresholds through repeated tests |
| Servo center is wrong | Robot drifts into wall | High during setup | Run servo sweep and straight-line test |
| No direct turn-angle sensor | Turn exit depends on timing and ultrasonic readings | Medium | Tune turn timing at expected battery voltage and record repeated runs |
| Corner counter or final stop miscounts | Robot may stop too early or too late | Medium | Test the 12-corner counter across repeated full runs |
| Battery voltage changes behavior | Lap times and turn radius vary | Medium | Record voltage and test at expected charge levels |
| HuskyLens not integrated or calibrated | Obstacle Challenge cannot be solved reliably | High | Mount HuskyLens, test communication, and collect red/green detection data |
| Parking concept not implemented in code | Parking points cannot be targeted reliably | High | Use HuskyLens to detect the parking area, drive until both walls leave view, then reverse and align after real tests |
| Too many constants changed at once | Tuning becomes confusing | Medium | Change one variable per test and log results |
