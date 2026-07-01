# Electromechanical Overview

This diagram shows the intended relationship between the main electrical and mechanical components.

```mermaid
flowchart TD
    BAT["3 x 3.7 V battery holder"] --> SW["Main switch"]
    SW --> PROT["Fuse/current protection TBD"]
    PROT --> REG["5 V regulator TBD"]
    PROT --> DRV["Motor driver TBD"]
    REG --> MEGA["Arduino Mega 2560"]
    MEGA --> SERVO["AD002 steering servo signal"]
    MEGA --> USF["Front ultrasonic trigger/echo"]
    MEGA --> USL["Left ultrasonic trigger/echo"]
    MEGA --> USR["Right ultrasonic trigger/echo"]
    MEGA --> BTN["Start button"]
    MEGA --> LED["Status LED"]
    MEGA --> DRV
    DRV --> MOTOR["DC drive motor"]
```

## Notes

- The motor driver is required before the DC motor can be safely controlled.
- The servo may need a separate 5 V supply depending on current draw.
- All grounds must be common.
- Final wire colors, pin numbers, and connectors must be documented after the real wiring is built.

