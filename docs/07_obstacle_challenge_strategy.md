# 7. Obstacle Challenge Strategy

## Current Status

The obstacle strategy is in the planning and sensor-integration stage. The team plans to use a HuskyLens AI camera to detect the red and green traffic signs and send the detected color to the ESP32. The ESP32 will then choose the correct steering maneuver.

The HuskyLens is selected but not integrated into the current wiring or firmware yet. Parking is still unresolved. The team has not yet selected whether the robot will detect the magenta parking box with the HuskyLens, another color sensor, distance geometry, or a timed/encoder-based approach.

## Rule-Based Requirement

The robot must pass red and green traffic signs on the correct side. It must also complete three laps and later perform the parking task. The final strategy needs perception, decision-making, and recovery behavior.

## Selected Vision Direction

| Option | Benefit | Risk |
| --- | --- | --- |
| HuskyLens AI camera | Microcontroller-friendly object/color recognition without a separate computer | Needs stable mounting, training, and lighting tests |
| Ultrasonic sensors | Reliable short-distance wall and corner information | Cannot identify block color |
| Future parking sensor/logic | Needed for final parking score | Method is not selected yet |

## Planned State Machine Extension

```mermaid
stateDiagram-v2
    [*] --> FOLLOW_LANE
    FOLLOW_LANE --> READ_HUSKYLENS: object detected
    READ_HUSKYLENS --> EVADE_RED: red block
    READ_HUSKYLENS --> EVADE_GREEN: green block
    EVADE_RED --> RECOVER_LANE
    EVADE_GREEN --> RECOVER_LANE
    RECOVER_LANE --> FOLLOW_LANE
    FOLLOW_LANE --> PARKING_SEARCH: three laps completed
    PARKING_SEARCH --> PARKING_MANEUVER: magenta parking markers found
    PARKING_MANEUVER --> [*]
```

## Placeholder Interfaces

The Obstacle Challenge firmware will need these interfaces:

- `readHuskyLensColor()`
- `handleRedObstacle()`
- `handleGreenObstacle()`
- `recoverAfterObstacle()`
- `searchParkingBox()`
- `performParkingManeuver()`

These functions should be implemented after the HuskyLens wiring, communication mode, and detection tests are documented.

## Evidence To Collect

- HuskyLens mounting photos.
- Red and green detection samples under competition lighting.
- Detection accuracy table.
- False positive and false negative examples.
- Recovery behavior after each obstacle.
- Communication test between HuskyLens and ESP32.
- Parking marker detection tests.

