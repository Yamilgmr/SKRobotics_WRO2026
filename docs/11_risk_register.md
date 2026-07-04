# 11. Risk Register

| Risk | Impact | Likelihood | Mitigation |
| --- | --- | --- | --- |
| DC motor connected without driver | Damages electronics or creates unsafe current draw | Low if L298N is wired correctly | Keep the robot lifted during first motor tests and verify L298N wiring before driving |
| L298N voltage drop or overheating | Robot may be slower or inconsistent | Medium | Record PWM response, battery voltage, and heat during tests |
| 5 V echo signal connected directly to ESP32 | ESP32 GPIO damage | Low if level converter is wired correctly | Use the 8-channel level converter on ultrasonic echo lines and verify voltage with a multimeter |
| Ultrasonic readings fail at angles | Robot turns late or overcorrects | Medium | Use filtering, tune thresholds, and compare with MPU6050 yaw evidence |
| Servo center is wrong | Robot drifts into wall | High during setup | Run servo sweep and straight-line test |
| Battery voltage changes behavior | Lap times and turn radius vary | Medium | Record voltage and test at expected charge levels |
| HuskyLens color recognition fails under lighting changes | Robot may choose the wrong obstacle side | Medium | Test red/green detection under practice lighting and log false detections |
| Parking strategy not selected | Parking points cannot be targeted reliably | High | Decide whether parking uses HuskyLens, color sensing, distance geometry, encoder timing, or a combined method |
| Placeholders not replaced | Repository loses documentation points | Medium | Use checklist before submission deadline |
| Too many constants changed at once | Tuning becomes confusing | Medium | Change one variable per test and log results |

