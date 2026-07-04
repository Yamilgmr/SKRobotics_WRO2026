# 6. Open Challenge Strategy

## Goal

Complete three laps without touching walls and stop after the final lap. The current strategy prioritizes continuous motion through corners using prefire steering.

## Why Prefire Turns

A stop-and-turn approach is simpler, but it costs time and can make the robot unstable when acceleration restarts. Prefire turning begins the steering action before the robot reaches the wall. This can produce smoother laps if the trigger distance is tuned correctly.

## Algorithm

1. Read front, left, and right ultrasonic sensors.
2. Drive straight while the front path is clear.
3. Watch for a confirmed opening when a side wall disappears.
4. Enter the turn state toward the confirmed opening.
5. Keep moving through the turn instead of braking.
6. Use MPU6050 yaw, side-wall evidence, and timing limits to decide when the turn is complete.
7. Recenter the steering and count the corner.
8. Repeat until 12 corners are counted, then drive a short finish segment and stop.

## Tuning Method

Start with slow speed. Change one variable at a time:

| Test | Variable | Expected Observation |
| --- | --- | --- |
| Front safety | `FRONT_DANGER_CM` | Higher value starts emergency turns earlier |
| Opening detection | `WALL_PRESENT_CM`, `WALL_LOST_CM` | Wider gap reduces false openings but may detect later |
| Turn radius | `MIN_TURN_MS`, `MAX_TURN_MS` | Longer time turns more |
| Yaw exit | `TURN_EXIT_YAW_DEG` | Higher value exits closer to a full 90 degree turn |
| Stability | `SPEED_FAST`, `SPEED_TURN` | Higher speed needs earlier and smoother steering |

## Success Metrics

- Completes one lap without touching walls.
- Completes three laps without manual rescue.
- Stops after the twelfth turn.
- Turn count matches actual corners.
- Distance corrections do not oscillate.

## Next Improvements

The prefire strategy already uses MPU6050 yaw as one turn-exit signal. The next improvement is to compare yaw-based exits against real lap behavior and decide whether an encoder is needed for lap distance and parking maneuvers.

