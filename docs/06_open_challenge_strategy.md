# 6. Open Challenge Strategy

## Goal

Complete three laps without touching walls and stop after the final lap. The current code is the team's final Open Challenge version for now. It counts 12 corners, runs a calibrated final straight, actively brakes, and stops.

## Why Continuous Turns

A stop-and-turn approach is simpler, but it costs time and can make the robot unstable when acceleration restarts. The current strategy keeps the motor moving during turns and prevents new decisions while a turn is already in progress.

## Current Algorithm

1. Read left, right, and front ultrasonic sensors with timed `pulseIn()` calls.
2. Prioritize side sensors in straight sections so wall-to-opening transitions are detected early.
3. Detect the track direction from the first reliable side opening.
4. Drive forward while checking side openings and the secondary front corner cue.
5. Start a turn when a side sensor changes from wall to opening.
6. Hold the calibrated fixed steering angle until the new wall is reacquired or the turn timeout is reached.
7. Center the wheels, exit the corner, and rearm the next turn.
8. Count completed corners.
9. After corner 12, run the starting straight for `FINAL_RUN_MS`.
10. Apply active braking for `FINAL_BRAKE_MS`, then stop the robot.

## Tuning Method

Start with slow tests. Change one variable at a time:

| Test | Variable | Expected Observation |
| --- | --- | --- |
| Side opening detection | `SIDE_TURN_TRIGGER_CM` | Higher value requires more open space before a side-triggered turn |
| Front corner cue | `FRONT_COUNT_CM` | Higher value counts front corner evidence earlier |
| Left turn duration | `MIN_LEFT_TURN_MS`, `MAX_LEFT_TURN_MS` | Controls left turn minimum and maximum duration |
| Right turn duration | `MIN_RIGHT_TURN_MS`, `MAX_RIGHT_TURN_MS` | Controls right turn minimum and maximum duration |
| Wall reacquisition | `LEFT_WALL_REACQUIRED_CM`, `RIGHT_WALL_REACQUIRED_CM` | Controls when each turn accepts that the new wall was found |
| Alignment and exit | `ALIGN_MS`, `EXIT_MS` | Controls centered-wheel recovery after each turn |
| Final stop | `FINAL_RUN_MS`, `FINAL_BRAKE_MS` | Controls final straight distance and braking time after corner 12 |
| Speed | `CRUISE_PWM`, `TURN_PWM`, `ALIGN_PWM`, `FINAL_PWM` | Higher speed needs earlier steering and stronger recovery |

## Success Metrics

- Completes one lap without touching walls.
- Completes three laps without manual rescue.
- Counts 12 corners consistently.
- Stops after the final advance.
- Does not chain false turns from noisy ultrasonic readings.
- Turn exit timing is repeatable at expected battery voltage.

## Remaining Open Challenge Evidence

- More measured calibration data for ultrasonic readings.
- L298N PWM and heat observations.
- Video of a full run.
- Physical wire-by-wire verification against the final pin map.
