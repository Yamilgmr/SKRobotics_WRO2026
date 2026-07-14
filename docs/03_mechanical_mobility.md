# 3. Mechanical Mobility

## Current Mobility Concept

The robot uses a car-like layout:

- Rear propulsion with one DC motor.
- Front steering controlled by an MG996R servo.
- Ultrasonic sensors mounted forward, on the right side, and on the left side.
- HuskyLens mounted for planned Obstacle Challenge sign and parking-area detection.

This layout matches the Future Engineers challenge because the robot must move smoothly around a road-like track and perform controlled turns rather than rotate in place.

## Steering

The MG996R servo controls the front axle direction. The first tuning task is to find:

- Mechanical center angle.
- Maximum safe left angle.
- Maximum safe right angle.
- Minimum turn radius at low, medium, and high speed.

The starter code uses these default values:

```cpp
static const int SERVO_CENTER = 90;
static const int SERVO_LEFT = 10;
static const int SERVO_RIGHT = 120;
```

These values match the current base firmware convention. They still need physical validation with the wheels lifted, because the safest software limits depend on the final servo horn and steering linkage geometry.

## Propulsion

The DC motor provides forward motion through an L298N motor driver. The L298N is used because it is available and easy to test with Arduino Mega PWM and direction signals. The team must still measure whether the voltage drop and heat are acceptable for repeated WRO runs.

Evidence to collect:

- Battery voltage before and after a run.
- PWM values that start the motor reliably.
- Motor response at low, medium, and high PWM.
- Driver heat after repeated runs.
- Whether the robot has enough speed and torque with the L298N installed.

## Prefire Cornering

The team wants the robot to turn without stopping. The current prefire method starts a steering turn when a side sensor changes from wall to opening. During the turn, the servo holds a fixed calibrated angle until the new wall is reacquired or the turn timeout is reached. The expected benefit is lower lap time. The main risk is that the car still has no gyroscope or encoder, so turn quality depends on ultrasonic readings, timing, and the mechanical steering limits.

Variables to tune:

- `SIDE_TURN_TRIGGER_CM`
- `FRONT_COUNT_CM`
- `LEFT_WALL_REACQUIRED_CM`
- `RIGHT_WALL_REACQUIRED_CM`
- `MIN_LEFT_TURN_MS`
- `MAX_LEFT_TURN_MS`
- `MIN_RIGHT_TURN_MS`
- `MAX_RIGHT_TURN_MS`
- `ALIGN_MS`
- `EXIT_MS`
- `SERVO_LEFT_DEG`
- `SERVO_RIGHT_DEG`

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

