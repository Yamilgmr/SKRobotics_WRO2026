# Power Architecture

## Current Assumption

The robot uses three 3.7 V cells in a battery holder. The exact chemistry and current capability must be confirmed before final wiring. If these are lithium cells, the fully charged voltage can be higher than nominal voltage.

## Power Budget Placeholder

| Component | Voltage | Estimated Current | Evidence |
| --- | --- | --- | --- |
| ESP32 Acebott / ESP32 Dev Module | 3.3 V logic, board 5 V input | TBD | Datasheet or measurement |
| MG996R servo | 5 V to 6 V typical | TBD | Measure under steering load |
| DC motor | Battery/driver output | TBD | Measure stall and running current |
| L298N motor driver | Battery input, logic input tested with ESP32 | TBD | Measure heat and voltage drop during tests |
| Ultrasonic sensors | 5 V | TBD | Datasheet |
| MPU6050 IMU | 3.3 V or module-regulated 5 V | TBD | Confirm module markings |
| 8-channel level converter | 3.3 V side and 5 V side | TBD | Verify wiring with multimeter |
| HuskyLens AI camera | TBD | TBD | Confirm communication mode and supply voltage |

## Motor Driver Decision

The team will use the L298N for the first prototype. The reason is availability and easy ESP32 integration through PWM and direction pins. The trade-off is that the L298N is less efficient than newer motor drivers and may reduce motor voltage or heat up during long runs.

Required evidence:

- L298N wiring diagram.
- Battery voltage before and after a run.
- PWM values tested.
- Motor response at low, medium, and high PWM.
- Driver temperature or heat observations.
- Explanation of whether the L298N remains acceptable or should be replaced later.
- Voltage on the ESP32 side of each ultrasonic echo signal after level conversion.

