# 6. Open Challenge Strategy

## Goal

Complete three laps without touching walls and stop after the final lap. The current strategy prioritizes continuous motion through corners using prefire steering.

## Why Prefire Turns

A stop-and-turn approach is simpler, but it costs time and can make the robot unstable when acceleration restarts. Prefire turning begins the steering action before the robot reaches the wall. This can produce smoother laps if the trigger distance is tuned correctly.

## Algorithm

1. Choose a wall to follow.
2. Drive forward at a conservative PWM value.
3. Read side distance and compute steering correction.
4. If front distance is below `FRONT_PREFIRE_CM`, enter turn state.
5. Hold the steering angle for `TURN_HOLD_MS`.
6. Count the corner.
7. Recenter for `RECENTER_MS`.
8. Repeat until 12 turns are counted.

## Tuning Method

Start with slow speed. Change one variable at a time:

| Test | Variable | Expected Observation |
| --- | --- | --- |
| Corner entry | `FRONT_PREFIRE_CM` | Higher value turns earlier |
| Turn radius | `TURN_HOLD_MS` | Longer value turns more |
| Wall following | `SIDE_CORRECTION_GAIN` | Higher value reacts more strongly |
| Stability | `BASE_SPEED_PWM` | Higher speed needs earlier turns |

## Success Metrics

- Completes one lap without touching walls.
- Completes three laps without manual rescue.
- Stops after the twelfth turn.
- Turn count matches actual corners.
- Distance corrections do not oscillate.

## Next Improvements

The prefire strategy will become much stronger with an IMU or encoder. With yaw feedback, turns can stop at an actual 90 degree target instead of relying only on time. With an encoder, lap distance and parking maneuvers can be measured more reliably.

