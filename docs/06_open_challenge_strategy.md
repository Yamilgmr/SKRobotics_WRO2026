# 6. Open Challenge Strategy

## Goal

Complete three laps without touching walls and stop after the final lap. The current code is the team's final Open Challenge version for now. It counts 12 corners, advances through the final straight, and stops.

## Why Continuous Turns

A stop-and-turn approach is simpler, but it costs time and can make the robot unstable when acceleration restarts. The current strategy keeps the motor moving during turns and prevents new decisions while a turn is already in progress.

## Current Algorithm

1. Read right, left, and front ultrasonic sensors using a non-blocking scheduler.
2. Prioritize side sensors so wall-to-opening transitions are detected early.
3. Detect the track direction from the first reliable side opening.
4. Drive forward while checking side openings, front distance, and safety thresholds.
5. Start a turn when a side sensor changes from wall to opening.
6. Use the front sensor as a fallback if the side opening was not detected in time.
7. Execute the turn sequence: entry steering, main turn, fine alignment, countersteer, and recovery.
8. Count completed corners.
9. After corner 12, advance on the starting straight for `FINAL_ADVANCE_MS`.
10. Stop the robot.

## Tuning Method

Start with slow tests. Change one variable at a time:

| Test | Variable | Expected Observation |
| --- | --- | --- |
| Wall target | `WALL_TARGET_CM` | Higher value drives farther from the wall |
| Side opening detection | `SIDE_OPEN_CM` | Higher value requires more open space before a side-triggered turn |
| Front fallback | `FRONT_TURN_CM` | Higher value captures turns earlier from the front sensor |
| Turn exit | `TURN_EXIT_FRONT_CM`, `TURN_EXIT_WALL_MAX_CM`, `TURN_EXIT_SIDE_MAX_CM` | Controls when the robot accepts that the new straight is visible |
| Turn duration | `TURN_MIN_MS`, `TURN_MAX_MS` | Longer value turns more |
| Alignment | `ALIGN_MIN_MS`, `ALIGN_MAX_MS` | Controls second correction timing |
| Final stop | `FINAL_ADVANCE_MS` | Controls how far the robot drives after corner 12 |
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
