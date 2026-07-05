# Electromechanical Overview

This diagram shows the intended relationship between the current electrical and mechanical components.

```mermaid
flowchart TD
    BAT["2 x 3.7 V battery pack"] --> SW["Main switch"]
    SW --> PROT["Fuse/current protection TBD"]
    PROT --> REG["5 V regulation path TBD"]
    PROT --> DRV["L298N motor driver"]
    REG --> MEGA["Arduino Mega 2560"]
    MEGA --> SERVO["MG996R steering servo signal"]
    MEGA --> USF["Front HC-SR04 trigger/echo"]
    MEGA --> USR["Right HC-SR04 trigger/echo"]
    MEGA --> IMU["Gyroscope / IMU I2C"]
    MEGA --> BTN["Start button"]
    MEGA --> DRV
    DRV --> MOTOR["DC drive motor"]
    BB["Breadboard"] --> MEGA
    BB --> USF
    BB --> USR
    BB --> IMU
```

## Notes

- The L298N will be used for the first DC motor control implementation.
- The MG996R servo may need a separate 5 V supply depending on current draw.
- All grounds must be common.
- The breadboard is a temporary prototyping method; final wiring should be secured before competition.
- Final wire colors and connector photos must be documented after the real wiring is cleaned up.
