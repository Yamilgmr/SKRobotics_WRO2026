# 2026-07-13 Open Challenge Serial Test

## Evidence

- Screenshot: `assets/test_evidence/open_challenge_serial_monitor_2026-07-13.png`

## Observation

The Arduino Mega Serial Monitor showed live Open Challenge state output during a real robot test. The capture includes:

- `Estado=RECTA`
- `Estado=GIRANDO`
- `Estado=AJUSTE_GIRO`
- `Estado=SALIDA_GIRO`
- right-turn direction output
- corner counter moving from `Esquinas=7/12` to `Esquinas=8/12`
- lap counter output showing `Vueltas: 2`

## Engineering Meaning

This confirms that the robot firmware was producing live diagnostic telemetry during physical testing. It is useful evidence for state-machine debugging and tuning, but it does not replace measured calibration tables for ultrasonic distance, servo limits, motor PWM response, or HuskyLens detection accuracy.
