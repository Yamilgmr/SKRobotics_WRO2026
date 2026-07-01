# 3. Mechanical Mobility

## Current Mobility Concept

The robot uses a car-like layout:

- Rear propulsion with one DC motor.
- Front steering controlled by an AD002 servo.
- Ultrasonic sensors mounted forward, left, and right.

This layout matches the Future Engineers challenge because the robot must move smoothly around a road-like track and perform controlled turns rather than rotate in place.

## Steering

The AD002 servo controls the front axle direction. The first tuning task is to find:

- Mechanical center angle.
- Maximum safe left angle.
- Maximum safe right angle.
- Minimum turn radius at low, medium, and high speed.

The starter code uses these default values:

```cpp
const int SERVO_CENTER = 90;
const int SERVO_LEFT_LIMIT = 55;
const int SERVO_RIGHT_LIMIT = 125;
```

These are only placeholders. They must be replaced after using the servo sweep test.

## Propulsion

The DC motor provides forward motion. A motor driver is required between the Arduino and the motor. The driver should be selected by measuring or estimating the motor current and selecting a driver with safe current margin.

Selection criteria:

- Supports the battery voltage.
- Supports motor stall current.
- Accepts Arduino PWM.
- Has clear wiring documentation.
- Wastes less power than the removed L298N.

## Prefire Cornering

The team wants the robot to turn without stopping. The prefire method starts a steering turn when the front ultrasonic sensor sees the wall at a calibrated distance. The expected benefit is lower lap time. The risk is lower repeatability if the robot enters corners from different lateral positions.

Variables to tune:

- `FRONT_PREFIRE_CM`
- `TURN_HOLD_MS`
- `TURN_STEERING_ANGLE`
- `TARGET_SIDE_DISTANCE_CM`
- `SIDE_CORRECTION_GAIN`

## Mechanical Evidence To Add

- Chassis dimensions.
- Wheel diameter.
- Wheelbase.
- Track width.
- Motor gear ratio or motor model.
- Servo horn geometry.
- Sensor mount angles.
- Photos of each revision.
- CAD files for mounts or chassis parts.

