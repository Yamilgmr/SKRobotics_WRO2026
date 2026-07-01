# 10. Build and Operation Manual

## Build Checklist

1. Mount Arduino Mega 2560 securely.
2. Mount front ultrasonic sensor facing straight forward.
3. Mount left and right ultrasonic sensors perpendicular to the side walls.
4. Mount AD002 servo so the center command points the wheels straight.
5. Install a suitable motor driver before connecting the DC motor.
6. Connect all grounds together.
7. Add a main power switch.
8. Secure batteries so they cannot move during turns.

## Software Setup

1. Install Arduino IDE.
2. Install `NewPing`.
3. Open the desired sketch from `src/`.
4. Check the pin map.
5. Upload to Arduino Mega 2560.
6. Open Serial Monitor at 115200 baud.

## Pre-Run Checklist

- Battery charged.
- Wheels attached.
- Servo horn tight.
- Sensor cables secure.
- Motor driver not overheating.
- Robot lifted off the ground for first motor test.
- `MOTOR_OUTPUT_ENABLED` set correctly.
- Emergency power disconnect available.

## Open Challenge Run Procedure

1. Place robot in the start section.
2. Turn power on.
3. Wait for status LED.
4. Press start button.
5. Observe wall following and corner prefire behavior.
6. Record result in the test log.

