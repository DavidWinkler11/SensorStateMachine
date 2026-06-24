# Sensor-Driven Robot State Machine on ESP32-WROVER-E

A concurrent embedded system built with FreeRTOS that reads gesture and proximity input from an APDS-9960 sensor and drives a live-updating state machine on a TFT display. Built as a hands-on project to gain practical experience with real-time embedded systems and concurrent task design ahead of pursuing a career in embedded/robotics software engineering.

---

## Hardware

| Component | Part |
|---|---|
| Microcontroller | Espressif ESP32-WROVER-E |
| Sensor | APDS-9960 (gesture + proximity) via Qwiic/I2C |
| Display | 2.4" ILI9341 TFT LCD with touch (Soldered) via SPI |

---

## What it does

The system runs three concurrent FreeRTOS tasks:

- **Sensor task** — polls the APDS-9960 over I2C and pushes readings onto a queue
- **State machine task** — consumes sensor readings and drives state transitions
- **Display task** — redraws the TFT only when the state actually changes

State machine:

```
IDLE ──(gesture detected)──► MOVING ──(proximity > 150)──► OBSTACLE DETECTED
 ▲                                                                   │
 └──────────────(proximity < 100)── STOPPED ◄───────────────────────┘
```

Inter-task communication uses two FreeRTOS queues (sensor → state machine, state machine → display) and a mutex guarding all I2C access, since both the sensor task and the state machine task access the APDS-9960 over the shared I2C bus.
