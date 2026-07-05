# 8. Testing and Tuning Plan

## Test Philosophy

Every important change should produce evidence. The team should avoid changing multiple constants at the same time unless the change is documented as a new version.

## Test Log Fields

- Date.
- Robot version.
- Code commit.
- Battery voltage.
- Track configuration.
- Test objective.
- Constants changed.
- Result.
- Failure mode.
- Next action.

## Initial Tests

| ID | Test | Pass Condition |
| --- | --- | --- |
| T01 | Arduino Mega pin verification | Every pin in the table controls or reads the expected device |
| T02 | Servo center test | Robot rolls straight with center value |
| T03 | Servo limit test | No mechanical binding at left/right limits |
| T04 | Ultrasonic static test | Front and right readings are within acceptable error at fixed distances |
| T05 | Gyroscope yaw test | Yaw changes consistently during manual 90 degree turns |
| T06 | L298N low PWM test | Motor starts smoothly without overheating driver |
| T07 | Right-wall following test | Robot follows one straight section without oscillation |
| T08 | Prefire corner test | Robot turns without stopping and avoids front wall |
| T09 | One lap test | Robot completes one lap |
| T10 | Three lap test | Robot completes three laps and stops |
| T11 | Obstacle color sensor test | Future sensor identifies red/green reliably after selection |

## Metrics

- Lap completion rate.
- Average lap time.
- Wall touches per run.
- Turn count accuracy.
- Ultrasonic invalid reading percentage.
- Gyroscope yaw drift after each lap.
- Battery voltage before and after run.
- L298N heat observations after repeated runs.
