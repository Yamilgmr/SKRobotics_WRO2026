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
| T01 | Servo center test | Robot rolls straight with center value |
| T02 | Servo limit test | No mechanical binding at left/right limits |
| T03 | Ultrasonic static test | Readings within acceptable error at fixed distances |
| T04 | Motor driver low PWM test | Motor starts smoothly without overheating driver |
| T05 | Wall following test | Robot follows one straight section without oscillation |
| T06 | Prefire corner test | Robot turns without stopping and avoids front wall |
| T07 | One lap test | Robot completes one lap |
| T08 | Three lap test | Robot completes three laps and stops |
| T09 | Obstacle color test | Future sensor identifies red/green reliably |

## Metrics

- Lap completion rate.
- Average lap time.
- Wall touches per run.
- Turn count accuracy.
- Ultrasonic invalid reading percentage.
- Battery voltage before and after run.

