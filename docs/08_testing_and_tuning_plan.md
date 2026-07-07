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
| T05 | L298N low PWM test | Motor starts smoothly without overheating driver |
| T06 | Right-wall following test | Robot follows one straight section without oscillation |
| T07 | Left/right turn decision test | Robot chooses the expected turn from front/right distances |
| T08 | Post-turn recovery test | Robot does not immediately chain repeated turns |
| T09 | One lap test | Robot completes one lap |
| T10 | Three lap movement test | Robot completes three laps; final stop still requires later code |
| T11 | HuskyLens red/green test | HuskyLens identifies red/green reliably after integration |

## Metrics

- Lap completion rate.
- Average lap time.
- Wall touches per run.
- Turn decision consistency.
- Ultrasonic invalid reading percentage.
- Battery voltage before and after run.
- L298N heat observations after repeated runs.
