# Power Architecture

## Current Assumption

The robot uses two 3.7 V cells for a 7.4 V nominal battery pack. The exact chemistry, capacity, discharge rating, and fully charged voltage must be confirmed before final wiring.

## Power Budget Placeholder

| Component | Voltage | Estimated Current | Evidence |
| --- | --- | --- | --- |
| Arduino Mega 2560 | 5 V logic | TBD | Datasheet or measurement |
| MG996R servo | 5 V to 6 V typical | TBD | Measure under steering load |
| DC motor | Battery/driver output | TBD | Measure stall and running current |
| L298N motor driver | Battery input, 5 V logic input | TBD | Measure heat and voltage drop during tests |
| Front HC-SR04 | 5 V | TBD | Datasheet |
| Right HC-SR04 | 5 V | TBD | Datasheet |

## Motor Driver Decision

The team will use the L298N for the first prototype. The reason is availability and easy Arduino Mega integration through PWM and direction pins. The trade-off is that the L298N is less efficient than newer motor drivers and may reduce motor voltage or heat up during long runs.

Required evidence:

- L298N wiring diagram.
- Battery voltage before and after a run.
- PWM values tested.
- Motor response at low, medium, and high PWM.
- Driver temperature or heat observations.
- Explanation of whether the L298N remains acceptable or should be replaced later.

## Servo Power Warning

The MG996R can draw significant current under steering load. If the Arduino Mega resets when the servo moves, the servo power path should be separated from the Arduino logic supply while keeping a common ground.
