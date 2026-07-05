# Changelog

## 2026-07-01

- Created WRO 2026 repository structure for SKRobotics.
- Added English documentation aligned with the WRO 2026 Future Engineers rubric.
- Added Arduino starter code and calibration sketches for the current hardware baseline.

## 2026-07-04

- Updated the active hardware baseline to ESP32 Acebott / ESP32 Dev Module, MG996R steering servo, MPU6050 IMU, L298N motor driver, and 8-channel level converter.
- Added the confirmed first ESP32 pin map to the README and power/sensor documentation.
- Replaced the Open Challenge starter firmware with the current ESP32 base code using ultrasonic opening detection, MPU6050 yaw feedback, and continuous corner turns.
- Updated calibration sketches for ESP32 ultrasonic, servo, and L298N testing.
- Added a hardware evidence photo for the level converter.

## 2026-07-05

- Returned the active hardware baseline to Arduino Mega 2560 after urgent hardware constraints.
- Updated the component list to Arduino Mega 2560, L298N, two ultrasonic sensors, MG996R servo, gyroscope/IMU, breadboard, 2 x 3.7 V battery pack, and DC motor.
- Replaced the active pin map with an Arduino Mega draft pin map for the reduced sensor set.
- Reworked the Open Challenge firmware and calibration sketches for Arduino Mega.
- Marked Obstacle Challenge color recognition as unresolved because no color/vision sensor is currently in the hardware list.

