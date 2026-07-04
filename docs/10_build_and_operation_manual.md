# 10. Build and Operation Manual

## Build Checklist

1. Mount the ESP32 Acebott / ESP32 Dev Module securely.
2. Mount front ultrasonic sensor facing straight forward.
3. Mount left and right ultrasonic sensors perpendicular to the side walls.
4. Mount MG996R servo so the center command points the wheels straight.
5. Install a suitable motor driver before connecting the DC motor.
6. Connect all grounds together.
7. Route 5 V ultrasonic echo signals through the 8-channel level converter before they reach ESP32 GPIO.
8. Add a main power switch.
9. Secure batteries so they cannot move during turns.

## Software Setup

1. Install Arduino IDE.
2. Install ESP32 board support.
3. Select the ESP32 Acebott / ESP32 Dev Module board profile that matches the controller.
4. Open the desired sketch from `src/`.
5. Check the pin map.
6. Upload to the ESP32.
7. Open Serial Monitor at 115200 baud.

## Pre-Run Checklist

- Battery charged.
- Wheels attached.
- Servo horn tight.
- Sensor cables secure.
- Motor driver not overheating.
- Robot lifted off the ground for first motor test.
- Start button behavior checked before placing the robot on the field.
- Emergency power disconnect available.

## Open Challenge Run Procedure

1. Place robot in the start section.
2. Turn power on.
3. Wait a moment after power-on for sensor and IMU initialization.
4. Press the start button.
5. Observe wall following and corner prefire behavior.
6. Record result in the test log.

