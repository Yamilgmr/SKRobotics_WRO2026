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
| T04 | Ultrasonic static test | Front, right, and left readings are within acceptable error at fixed distances |
| T05 | L298N low PWM test | Motor starts smoothly without overheating driver |
| T06 | Side-opening straight test | Robot drives one straight section without oscillation while side sensors stay stable |
| T07 | Left/right turn decision test | Robot chooses the expected turn from side wall-to-opening transitions |
| T08 | Post-turn recovery test | Robot does not immediately chain repeated turns after countersteer recovery |
| T09 | One lap test | Robot completes one lap |
| T10 | Three lap movement test | Robot completes three laps and stops after the final advance |
| T11 | HuskyLens red/green test | HuskyLens identifies red/green reliably after Arduino communication is integrated |
| T12 | HuskyLens parking view test | Both parking walls are detected, then reliably leave the camera view at the intended approach position |
| T13 | Reverse parking maneuver test | Robot reverses and aligns into the parking box after the selected visual trigger |

## Metrics

- Lap completion rate.
- Average lap time.
- Wall touches per run.
- Turn decision consistency.
- Ultrasonic invalid reading percentage.
- Battery voltage before and after run.
- L298N heat observations after repeated runs.
