# 10. Build and Operation Manual

## Build Checklist

1. Mount the Arduino Mega 2560 securely.
2. Mount front ultrasonic sensor facing straight forward.
3. Mount right ultrasonic sensor perpendicular to the right wall.
4. Mount MG996R servo so the center command points the wheels straight.
5. Install the L298N before connecting the DC motor.
6. Connect all grounds together.
7. Connect the gyroscope/IMU to Arduino Mega I2C pins.
8. Add a main power switch.
9. Secure the two 3.7 V cells so they cannot move during turns.
10. Keep the breadboard wiring short and secured to reduce loose connections.

## Software Setup

1. Install Arduino IDE.
2. Select `Arduino Mega or Mega 2560`.
3. Select processor `ATmega2560`.
4. Open the desired sketch from `src/`.
5. Check the pin map.
6. Upload to Arduino Mega 2560.
7. Open Serial Monitor at 115200 baud.

## Pre-Run Checklist

- Battery charged.
- Wheels attached.
- Servo horn tight.
- Sensor cables secure.
- Breadboard jumpers secure.
- L298N not overheating.
- Robot lifted off the ground for first motor test.
- Start button behavior checked before placing the robot on the field.
- Emergency power disconnect available.

## Open Challenge Run Procedure

1. Place robot in the start section.
2. Turn power on.
3. Wait a moment after power-on for sensor and gyroscope initialization.
4. Press the start button.
5. Observe right-wall following and corner prefire behavior.
6. Record result in the test log.
