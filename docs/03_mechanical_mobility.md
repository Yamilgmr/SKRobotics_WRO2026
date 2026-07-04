# 3. Mechanical Mobility

## Current Mobility Concept

The robot uses a car-like layout:

- Rear propulsion with one DC motor.
- Front steering controlled by an MG996R servo.
- Ultrasonic sensors mounted forward, left, and right.

This layout matches the Future Engineers challenge because the robot must move smoothly around a road-like track and perform controlled turns rather than rotate in place.

## Steering

The MG996R servo controls the front axle direction. The first tuning task is to find:

- Mechanical center angle.
- Maximum safe left angle.
- Maximum safe right angle.
- Minimum turn radius at low, medium, and high speed.

The starter code uses these default values:

```cpp
static const int SERVO_LEFT_ANGLE = 0;
static const int SERVO_CENTER_ANGLE = 45;
static const int SERVO_RIGHT_ANGLE = 90;
```

These values match the current base firmware convention. They still need physical validation with the wheels lifted, because the safest software limits depend on the final servo horn and steering linkage geometry.

## Propulsion

The DC motor provides forward motion through an L298N motor driver. The L298N is used because it is available and easy to test with ESP32 PWM and direction signals. The team must still measure whether the voltage drop and heat are acceptable for repeated WRO runs.

Evidence to collect:

- Battery voltage before and after a run.
- PWM values that start the motor reliably.
- Motor response at low, medium, and high PWM.
- Driver heat after repeated runs.
- Whether the robot has enough speed and torque with the L298N installed.

## Prefire Cornering

The team wants the robot to turn without stopping. The prefire method starts a steering turn when the side sensors confirm an opening, with the front sensor still acting as an emergency trigger if a wall becomes too close. The expected benefit is lower lap time. The risk is lower repeatability if the robot enters corners from different lateral positions.

Variables to tune:

- `FRONT_DANGER_CM`
- `MIN_TURN_MS`
- `MAX_TURN_MS`
- `TURN_EXIT_YAW_DEG`
- `SERVO_LEFT_ANGLE`
- `SERVO_RIGHT_ANGLE`
- `WALL_PRESENT_CM`
- `WALL_LOST_CM`

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

