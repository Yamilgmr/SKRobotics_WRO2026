# 10. Build and Operation Manual

## Build Checklist

1. Mount the Arduino Mega 2560 securely.
2. Mount front ultrasonic sensor facing straight forward.
3. Mount right ultrasonic sensor perpendicular to the right wall.
4. Mount left ultrasonic sensor perpendicular to the left wall.
5. Mount the HuskyLens where it can see traffic signs and the parking area.
6. Mount MG996R servo so the center command points the wheels straight.
7. Install the L298N before connecting the DC motor.
8. Connect all grounds together.
9. Add a main power switch.
10. Secure the two 3.7 V cells so they cannot move during turns.
11. Keep the breadboard wiring short and secured to reduce loose connections.

## Software Setup

1. Install Arduino IDE.
2. Select `Arduino Mega or Mega 2560`.
3. Select processor `ATmega2560`.
4. Open the desired sketch from `src/`.
5. Check the pin map.
6. Upload to Arduino Mega 2560.
7. Open Serial Monitor at 115200 baud for the current Open Challenge firmware.

## Pre-Run Checklist

- Battery charged.
- Wheels attached.
- Servo horn tight.
- Sensor cables secure.
- Breadboard jumpers secure.
- L298N not overheating.
- Robot lifted off the ground for first motor test.
- Emergency power disconnect available.

## Open Challenge Run Procedure

1. Place robot in the start section.
2. Turn power on only when the team is ready for immediate movement.
3. Observe side-opening detection, left/right turn behavior, corner counting, final run, active brake, and final stop.
5. Record result in the test log.
