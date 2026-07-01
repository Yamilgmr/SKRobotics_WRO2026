# Power Architecture

## Current Assumption

The robot uses three 3.7 V cells in a battery holder. The exact chemistry and current capability must be confirmed before final wiring. If these are lithium cells, the fully charged voltage can be higher than nominal voltage.

## Power Budget Placeholder

| Component | Voltage | Estimated Current | Evidence |
| --- | --- | --- | --- |
| Arduino Mega 2560 | 5 V logic | TBD | Datasheet or measurement |
| AD002 servo | TBD | TBD | Measure under steering load |
| DC motor | Battery/driver output | TBD | Measure stall and running current |
| Motor driver | Battery input, logic input | TBD | Select replacement |
| Ultrasonic sensors | 5 V | TBD | Datasheet |

## Required Driver Decision

The L298N was removed. Before motor testing, select one replacement:

- TB6612FNG if current is within limits.
- BTS7960 or similar high-current driver if the motor draws more current.
- ESC if the motor and power system fit that architecture.
- Logic-level MOSFET stage only if direction control is not needed and the circuit is designed safely.

The selected driver must be added to this document with wiring and test data.

