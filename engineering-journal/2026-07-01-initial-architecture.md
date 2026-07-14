# 2026-07-01 Initial Architecture

## Context

The team is preparing for WRO 2026 Future Engineers. At this point in the design process, the first documented robot hardware included an Arduino Mega 2560, three ultrasonic sensors, an AD002 steering servo, a DC motor, and a 3 x 3.7 V battery holder. The L298N motor driver had been removed, so motor control was treated as an unresolved subsystem.

Update on 2026-07-04: the team temporarily tested an ESP32 Acebott / ESP32 Dev Module architecture with an MG996R steering servo, MPU6050 IMU, three HC-SR04 ultrasonic sensors, L298N motor driver, and 8-channel bidirectional level converter. This remains as project history because the active 2026-07-06 build returned to Arduino Mega.

Update on 2026-07-05: due to urgent practical constraints, the active control architecture returned to Arduino Mega 2560. The then-current component list was Arduino Mega 2560, L298N motor driver, two ultrasonic sensors mounted front and right, MG996R steering servo, breadboard, two 3.7 V cells for 7.4 V nominal, and one DC motor. The ESP32 wiring is now treated as an archived iteration, not the active baseline.

Update on 2026-07-06: the then-current Open Challenge code no longer used a gyroscope, start button, status LED, encoder, or left ultrasonic sensor. The active pin map at that time was servo D9, L298N ENA D5, IN1 D6, IN2 D7, front ultrasonic D22/D23, and right ultrasonic D24/D25. This was later superseded.

Update on 2026-07-13: the active Open Challenge baseline now uses Arduino Mega 2560, L298N motor driver, MG996R steering servo, three HC-SR04 ultrasonic sensors, HuskyLens installed for future Obstacle Challenge perception, breadboard wiring, two 3.7 V cells for 7.4 V nominal, and one DC motor. The final code pin map is servo D9, L298N ENA D5, IN1 D6, IN2 D7, front ultrasonic D42/D43, right ultrasonic D46/D47, and left ultrasonic D52/D53.

## Design Direction

The current goal is an Open Challenge baseline using continuous motion and side-opening predictive turns. The robot should not stop before every curve. Instead, it should detect wall-to-opening transitions with the side ultrasonic sensors and use the front ultrasonic sensor as fallback and turn-exit evidence.

## Main Constraint

The DC motor cannot be controlled safely until a motor driver is selected and wired correctly. The starter firmware includes motor control functions, but motor output is disabled by default.

## Update On 2026-07-04

The team decided to use the L298N again for the first motor-control prototype. This is not the most efficient motor driver, but it is available and simple to test with Arduino Mega PWM and direction pins. The next evidence needed is the L298N wiring diagram, PWM response, and heat behavior during repeated runs.

## Next Tests

1. Verify ultrasonic sensor readings at fixed distances.
2. Find servo center and safe steering limits.
3. Verify L298N wiring, breadboard connections, and common ground.
4. Test low-speed motion with robot lifted.
5. Tune side-opening detection, turn timing, final advance, and full three-lap stop behavior.
6. Add HuskyLens Arduino communication and parking-area detection tests.

