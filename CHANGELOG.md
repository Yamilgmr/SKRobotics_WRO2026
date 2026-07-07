# Changelog

## 2026-07-01

- Created WRO 2026 repository structure for SKRobotics.
- Added English documentation aligned with the WRO 2026 Future Engineers rubric.
- Added Arduino starter code and calibration sketches for the current hardware baseline.

## 2026-07-04

- Temporarily updated the hardware baseline to ESP32 Acebott / ESP32 Dev Module, MG996R steering servo, MPU6050 IMU, L298N motor driver, and 8-channel level converter.
- Added the confirmed first ESP32 pin map to the README and power/sensor documentation.
- Replaced the Open Challenge starter firmware with the then-current ESP32 base code using ultrasonic opening detection, MPU6050 yaw feedback, and continuous corner turns.
- Updated calibration sketches for ESP32 ultrasonic, servo, and L298N testing.
- Added a hardware evidence photo for the level converter.

## 2026-07-05

- Returned the active hardware baseline to Arduino Mega 2560 after urgent hardware constraints.
- Updated the temporary component list to Arduino Mega 2560, L298N, two ultrasonic sensors, MG996R servo, gyroscope/IMU, breadboard, 2 x 3.7 V battery pack, and DC motor.
- Replaced the active pin map with an Arduino Mega draft pin map for the reduced sensor set.
- Reworked the Open Challenge firmware and calibration sketches for Arduino Mega.
- Marked Obstacle Challenge color recognition as unresolved because no color/vision sensor is currently in the hardware list.

## 2026-07-06

- Updated the active Open Challenge code to the final two-ultrasonic Arduino Mega version provided by the team.
- Updated the pin map to servo D9, L298N D5/D6/D7, front ultrasonic D22/D23, and right ultrasonic D24/D25.
- Replaced the six required vehicle photos in `v-photos/` with the final current vehicle views.
- Added formal, individual, and working-session team photos in `t-photos/`.
- Added the current electrical wiring diagram image in `schemes/electrical_wiring_diagram.svg`.
- Removed active documentation references to gyroscope, start button, status LED, encoder, and left ultrasonic sensor.
- Documented that lap counting and automatic final stop are still missing from the current movement baseline.

