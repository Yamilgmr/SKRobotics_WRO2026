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
| T04 | Level converter voltage test | ESP32-side echo signals stay near 3.3 V |
| T05 | MPU6050 yaw test | Yaw changes consistently during manual 90 degree turns |
| T06 | Motor driver low PWM test | Motor starts smoothly without overheating driver |
| T07 | Wall following test | Robot follows one straight section without oscillation |
| T08 | Prefire corner test | Robot turns without stopping and avoids front wall |
| T09 | One lap test | Robot completes one lap |
| T10 | Three lap test | Robot completes three laps and stops |
| T11 | Obstacle color test | HuskyLens identifies red/green reliably after integration |

## Metrics

- Lap completion rate.
- Average lap time.
- Wall touches per run.
- Turn count accuracy.
- Ultrasonic invalid reading percentage.
- MPU6050 yaw drift after each lap.
- ESP32-side echo voltage after level conversion.
- Battery voltage before and after run.

