# 11. Risk Register

| Risk | Impact | Likelihood | Mitigation |
| --- | --- | --- | --- |
| DC motor connected without driver | Damages Arduino or unsafe current draw | High until driver selected | Keep motor disabled in code; select driver before testing |
| Ultrasonic readings fail at angles | Robot turns late or overcorrects | Medium | Use filtering, tune thresholds, add IMU later |
| Servo center is wrong | Robot drifts into wall | High during setup | Run servo sweep and straight-line test |
| Battery voltage changes behavior | Lap times and turn radius vary | Medium | Record voltage and test at expected charge levels |
| No color sensing for obstacles | Obstacle Challenge cannot be solved | High | Choose PixyCam/color sensor and document calibration |
| Placeholders not replaced | Repository loses documentation points | Medium | Use checklist before submission deadline |
| Too many constants changed at once | Tuning becomes confusing | Medium | Change one variable per test and log results |

