# 6. Open Challenge Strategy

## Goal

Complete three laps without touching walls and later stop after the final lap. The current code is a movement baseline: it focuses on right-wall following and reliable left/right turn decisions, but it does not yet implement lap counting or the final stop.

## Why Continuous Turns

A stop-and-turn approach is simpler, but it costs time and can make the robot unstable when acceleration restarts. The current strategy keeps the motor moving during turns and prevents new decisions while a turn is already in progress.

## Current Algorithm

1. Read front and right ultrasonic sensors.
2. Filter readings to reduce large sudden jumps.
3. While in `STRAIGHT`, evaluate only the front and right distances.
4. If the front is blocked and the right side is free, turn right.
5. If the front is blocked and the right side is not free, turn left.
6. If the right side is free for multiple readings and was previously blocked, turn right.
7. During a turn, do not re-decide the maneuver.
8. After the turn exit condition, drive straight for a short post-turn period.

## Tuning Method

Start with slow tests. Change one variable at a time:

| Test | Variable | Expected Observation |
| --- | --- | --- |
| Right-wall distance | `RIGHT_TARGET_CM` | Higher value drives farther from the right wall |
| Right opening detection | `RIGHT_FREE_CM` | Higher value requires more open space before turning right |
| Front corner entry | `FRONT_TURN_CM` | Higher value turns earlier |
| Turn exit distance | `FRONT_CLEAR_AFTER_TURN_CM` | Higher value waits for clearer front space |
| Turn duration | `MIN_TURN_MS`, `MAX_TURN_MS` | Longer value turns more |
| Post-turn stability | `POST_TURN_STRAIGHT_MS` | Longer value prevents chained turns |
| Speed | `SPEED_NORMAL`, `SPEED_TURN` | Higher speed needs earlier steering |

## Success Metrics

- Completes one lap without touching walls.
- Completes three laps without manual rescue.
- Does not chain repeated right turns when the right side remains open.
- Right-wall corrections do not oscillate.
- Turn exit timing is repeatable at expected battery voltage.

## Missing For Full Open Challenge Scoring

- Lap counting.
- Automatic stop after three laps.
- Test evidence for the chosen turn timing.
- Final tuning at the real competition battery voltage.

## Next Improvements

The current two-ultrasonic setup is enough for a first Open Challenge movement baseline, but it has limited recovery information. Adding lap counting and a final stop is the most important next software step. Later, adding an encoder or gyroscope would make turns and stopping more repeatable, but those devices are not part of the current code.
