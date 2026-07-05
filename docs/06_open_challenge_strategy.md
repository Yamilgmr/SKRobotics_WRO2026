# 6. Open Challenge Strategy

## Goal

Complete three laps without touching walls and stop after the final lap. The current strategy prioritizes continuous motion through corners using a front ultrasonic sensor, right-wall distance, and gyroscope-supported turns.

## Why Prefire Turns

A stop-and-turn approach is simpler, but it costs time and can make the robot unstable when acceleration restarts. Prefire turning begins the steering action before the robot reaches the wall. This can produce smoother laps if the trigger distance is tuned correctly.

## Current Algorithm

1. Read front and right ultrasonic sensors.
2. Follow the right wall during straight sections.
3. Use right-distance error to apply small steering corrections.
4. If the front distance drops below `FRONT_TURN_CM`, enter the turn state.
5. Keep the motor moving during the turn.
6. Use gyroscope yaw and timing limits to exit the turn.
7. Recenter the steering and count the corner.
8. Repeat until the target corner count is reached.

## Tuning Method

Start with slow speed. Change one variable at a time:

| Test | Variable | Expected Observation |
| --- | --- | --- |
| Right-wall distance | `TARGET_RIGHT_DISTANCE_CM` | Higher value drives farther from the right wall |
| Wall correction | `RIGHT_CORRECTION_GAIN` | Higher value reacts more strongly |
| Corner entry | `FRONT_TURN_CM` | Higher value turns earlier |
| Emergency safety | `FRONT_DANGER_CM` | Higher value reacts sooner to close walls |
| Turn radius | `MIN_TURN_MS`, `MAX_TURN_MS` | Longer time turns more |
| Gyro exit | `TURN_EXIT_YAW_DEG` | Higher value exits closer to a full 90 degree turn |
| Speed | `SPEED_FAST`, `SPEED_TURN` | Higher speed needs earlier steering |

## Success Metrics

- Completes one lap without touching walls.
- Completes three laps without manual rescue.
- Stops after the target corner count.
- Turn count matches actual corners.
- Right-wall corrections do not oscillate.
- Gyroscope yaw does not drift too much during a run.

## Next Improvements

The current two-ultrasonic setup is enough for a first Open Challenge baseline, but it has limited recovery information. Adding either a left ultrasonic sensor, encoder, or stronger yaw correction later would improve repeatability. For now, the best path is to collect real test data and tune the constants carefully.
