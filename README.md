# 🚗 Vehicle Fault Detection & Logging System (VFDLS)

> A dual-ECU automotive diagnostic system built using **ATmega32 microcontrollers** that performs real-time vehicle monitoring, fault detection, Diagnostic Trouble Code (DTC) logging, and driver interaction through a dedicated Human Machine Interface (HMI).

<p align="center">

![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C-orange?style=for-the-badge)
![Architecture](https://img.shields.io/badge/Architecture-Layered-lightgrey?style=for-the-badge)

</p>

---

# 📸 Demo

<p align="center">

<img src="https://github.com/user-attachments/assets/000121f6-d0e2-4fce-8dbb-ac2c7e8cc69b" width="900"/>

</p>

---

# ✨ Features

- 🚗 Dual ECU Architecture
- 📡 UART Communication Between ECUs
- 🌡️ Real-Time Temperature Monitoring
- 📏 Ultrasonic Parking Assistance
- 💾 Persistent Fault Logging using External EEPROM
- 📋 Diagnostic Trouble Codes (DTC)
- 🖥️ LCD-Based Human Machine Interface
- ⌨️ Keypad Navigation
- 🪟 Dual Window Control
- ⚡ Layered Embedded Software Architecture
- 🔄 Real-Time Sensor Monitoring
- 📦 Modular Driver Design

---

# 📖 Overview

Modern vehicles rely on multiple Electronic Control Units (ECUs) that communicate continuously to monitor sensors, control actuators, and detect system faults.

The **Vehicle Fault Detection & Logging System (VFDLS)** simulates this real-world automotive architecture by implementing two independent ECUs communicating through UART.

The system continuously monitors vehicle subsystems, detects abnormal operating conditions, stores Diagnostic Trouble Codes (DTCs) in external EEPROM, and allows the driver to retrieve fault information through a keypad-controlled LCD interface.

This project was developed as the **Final Capstone Project** for the **Standard Embedded Systems Diploma** at **Edges for Training**.

---

# 🏗️ System Architecture

```text
                 +----------------------+
                 |      HMI ECU         |
                 |----------------------|
                 | LCD Display          |
                 | 4x4 Keypad           |
                 | UART Driver          |
                 +----------+-----------+
                           |
                     UART Communication
                           |
                 +----------+-----------+
                 |    Control ECU       |
                 |----------------------|
                 | LM35 Temperature     |
                 | HC-SR04 Ultrasonic   |
                 | DC Motors            |
                 | EEPROM (I²C)         |
                 +----------------------+
```

---

# 🛠️ Hardware Components

| Component | Interface |
|------------|-----------|
| ATmega32 (x2) | UART |
| HC-SR04 Ultrasonic Sensor | ICU |
| LM35 Temperature Sensor | ADC |
| 24C16 EEPROM | I²C |
| LCD 20×4 | GPIO |
| 4×4 Matrix Keypad | GPIO |
| DC Motors | PWM + GPIO |

---

# 💻 Software Architecture

The firmware follows a modular layered architecture.

```text
Application Layer
│
├── Vehicle Monitoring
├── Fault Detection
├── DTC Management
└── HMI Logic

HAL Layer
│
├── LCD
├── Keypad
├── Ultrasonic
├── LM35
├── EEPROM
└── DC Motor

MCAL Layer
│
├── GPIO
├── UART
├── ADC
├── Timer
├── PWM
├── ICU
└── I²C

ATmega32 Hardware
```

---

# ⚙️ System Workflow

```text
Driver

↓

Select Operation

↓

HMI ECU

↓

UART Command

↓

Control ECU

↓

Read Sensors

↓

Detect Faults

↓

Store DTC in EEPROM

↓

Return Data

↓

Display on LCD
```

---

# 🔍 System Subsystems

---

## 🌡️ Engine Temperature Monitoring

The engine temperature is monitored using an **LM35 analog temperature sensor** connected to the Control ECU through the ATmega32 Analog-to-Digital Converter (ADC).

### Working Principle

- The LM35 generates an analog voltage proportional to temperature (10 mV/°C).
- The ADC periodically samples the sensor output.
- The firmware converts the ADC value into degrees Celsius.
- If the measured temperature exceeds **90°C**, the system logs an engine overheating fault.

### Hardware Interface

| Device | Peripheral |
|---------|------------|
| LM35 | ADC |

### Fault Detection

| Temperature | System Response |
|------------|-----------------|
| ≤ 90°C | Normal Operation |
| > 90°C | Store DTC P002 |

---

## 📏 Parking Assistance System

Obstacle detection is implemented using an **HC-SR04 Ultrasonic Sensor**.

### Working Principle

1. The Control ECU generates a trigger pulse.
2. The sensor emits an ultrasonic wave.
3. The reflected echo is captured using the Timer1 Input Capture Unit (ICU).
4. Distance is calculated from the measured echo time.

### Hardware Interface

| Device | Peripheral |
|---------|------------|
| HC-SR04 | ICU (Timer1) |

### Fault Detection

| Distance | System Response |
|----------|-----------------|
| ≥ 10 cm | Normal Operation |
| < 10 cm | Store DTC P001 |

---

## 💾 Fault Logging System

Detected faults are permanently stored inside an external **24C16 EEPROM** connected through the I²C (TWI) interface.

### Features

- Permanent storage
- Non-volatile memory
- Fault history retrieval
- One-time logging for each fault occurrence

### Stored Data

- Diagnostic Trouble Code
- Fault History

---

## 🖥️ Human Machine Interface

The HMI ECU allows the driver to interact with the system through a 20×4 LCD and a 4×4 matrix keypad.

### Responsibilities

- Display the main menu
- Show live sensor readings
- Display window status
- Retrieve logged faults
- Send commands to the Control ECU

---

## 🪟 Window Control System

Two DC motors simulate the vehicle's power windows.

The HMI ECU sends user commands to the Control ECU through UART, where the motors are driven using GPIO and PWM.

### Features

- Independent window control
- Open / Close operations
- Motor state monitoring
- LCD status display
- Protection against duplicate commands

---

## 📡 UART Communication

Both ECUs communicate using a custom UART protocol operating at **9600 baud**.

### Supported Commands

| Command | Description |
|----------|-------------|
| Start Monitoring | Activate all subsystems |
| Display Values | Read sensor values |
| Retrieve Faults | Read EEPROM contents |
| Stop Monitoring | Disable monitoring |

---

## ⚡ Layered Software Design

The firmware follows a layered architecture similar to those used in automotive embedded software.

```text
Application Layer
        │
HAL Drivers
        │
MCAL Drivers
        │
ATmega32 Hardware
```

### Advantages

- Modular firmware
- Easier debugging
- Driver reusability
- Better maintainability
- Scalable architecture

---

# 🚨 Diagnostic Trouble Codes

| Code | Description |
|------|-------------|
| P001 | Vehicle Too Close to Obstacle |
| P002 | Engine Over Temperature |

---

# 📂 Repository Structure

```text
Vehicle-Fault-Detection-System
│
├── HMI_ECU
│   ├── APP
│   ├── HAL
│   ├── MCAL
│   └── main.c
│
├── Control_ECU
│   ├── APP
│   ├── HAL
│   ├── MCAL
│   └── main.c
│
├── Proteus
│
├── Docs
│
├── Images
│
└── README.md
```

---

# 🚀 Getting Started

## Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Vehicle-Fault-Detection-System.git
```

## Requirements

- AVR-GCC
- Eclipse IDE (or Microchip Studio)
- Proteus 8 Professional
- ATmega32 Development Boards

## Build

Compile each ECU project separately and flash the generated firmware to its corresponding ATmega32.

---

# 🛠️ Technologies Used

### Programming

- C

### Microcontroller

- ATmega32

### Communication Protocols

- UART
- I²C (TWI)
- ADC
- ICU
- PWM
- GPIO

### Tools

- Eclipse IDE
- AVR-GCC
- Proteus
- Git
- GitHub

---

# 🏆 Key Achievements

- Designed a complete dual-ECU embedded architecture.
- Developed modular MCAL, HAL, and Application layers.
- Implemented custom drivers for UART, ADC, GPIO, Timer, ICU, PWM, and I²C.
- Developed a custom UART communication protocol.
- Implemented persistent Diagnostic Trouble Code (DTC) logging.
- Integrated multiple sensors and actuators into one embedded application.
- Applied software architecture principles commonly used in automotive embedded systems.

---

# 🎯 Learning Outcomes

This project demonstrates practical experience in:

- Embedded C Programming
- Automotive Embedded Systems
- Layered Software Design
- Driver Development
- UART Communication
- EEPROM Data Logging
- Sensor Interfacing
- Real-Time Embedded Systems
- Fault Detection Algorithms
- Modular Firmware Design

---

# 📸 Future Improvements

- CAN Bus Communication
- FreeRTOS Integration
- STM32 Implementation
- SD Card Data Logging
- Bluetooth Mobile Application
- OBD-II Style Diagnostic Interface

---

# ⭐ Support

If you found this project useful or interesting, consider giving it a **⭐ Star**.

It helps others discover the project and supports future development.
