# Electromechanical Overview

This diagram shows the intended relationship between the main electrical and mechanical components.

```mermaid
flowchart TD
    BAT["3 x 3.7 V battery holder"] --> SW["Main switch"]
    SW --> PROT["Fuse/current protection TBD"]
    PROT --> REG["5 V regulator TBD"]
    PROT --> DRV["L298N motor driver"]
    REG --> ESP["ESP32 Acebott / ESP32 Dev Module"]
    ESP --> SERVO["MG996R steering servo signal"]
    ESP --> LVL["8-channel level converter"]
    LVL --> USF["Front HC-SR04 trigger/echo"]
    LVL --> USL["Left HC-SR04 trigger/echo"]
    LVL --> USR["Right HC-SR04 trigger/echo"]
    ESP --> IMU["MPU6050 I2C"]
    ESP --> CAM["HuskyLens communication TBD"]
    ESP --> BTN["Start button"]
    ESP --> DRV
    DRV --> MOTOR["DC drive motor"]
```

## Notes

- The L298N will be used for the first DC motor control implementation.
- The MG996R servo may need a separate 5 V supply depending on current draw.
- The ESP32 uses 3.3 V logic, so HC-SR04 echo signals must be protected with the level converter.
- All grounds must be common.
- Final wire colors and connector photos must be documented after the real wiring is cleaned up.

