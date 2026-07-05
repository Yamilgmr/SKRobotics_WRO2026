# 11. Risk Register

| Risk | Impact | Likelihood | Mitigation |
| --- | --- | --- | --- |
| DC motor connected without driver | Damages electronics or creates unsafe current draw | Low if L298N is wired correctly | Keep the robot lifted during first motor tests and verify L298N wiring before driving |
| L298N voltage drop or overheating | Robot may be slower or inconsistent | Medium | Record PWM response, battery voltage, and heat during tests |
| Breadboard wire becomes loose | Intermittent failures during runs | Medium | Secure jumpers and document final wiring diagram |
| Only one side ultrasonic sensor | Robot has weaker lane recovery | Medium | Use conservative speed, right-wall filtering, and gyroscope-supported turns |
| Ultrasonic readings fail at angles | Robot turns late or overcorrects | Medium | Use filtering and tune thresholds through repeated tests |
| Servo center is wrong | Robot drifts into wall | High during setup | Run servo sweep and straight-line test |
| Gyroscope drift | Turn exit becomes inconsistent | Medium | Calibrate at startup and compare yaw with real corner behavior |
| Battery voltage changes behavior | Lap times and turn radius vary | Medium | Record voltage and test at expected charge levels |
| No obstacle color sensor | Obstacle Challenge cannot be solved reliably | High | Select and test a red/green detection sensor |
| Parking strategy not selected | Parking points cannot be targeted reliably | High | Decide whether parking uses color sensing, distance geometry, encoder timing, or a combined method |
| Too many constants changed at once | Tuning becomes confusing | Medium | Change one variable per test and log results |
