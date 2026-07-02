# 🚗 Vehicle Fault Detection & Logging System (VFDLS)

> A dual-ECU automotive diagnostic system built using **ATmega32 microcontrollers** that performs real-time vehicle monitoring, fault detection, Diagnostic Trouble Code (DTC) logging, and driver interaction through a dedicated Human Machine Interface (HMI).

<p align="center">

![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-ATmega32-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C-orange?style=for-the-badge)
![Architecture](https://img.shields.io/badge/Architecture-Layered-lightgrey?style=for-the-badge)
![Communication](https://img.shields.io/badge/UART-I2C-ADC-ICU-PWM-red?style=for-the-badge)

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

The **Vehicle Fault Detection & Logging System (VFDLS)** simulates this real-world automotive architecture by implementing two independent ECUs communicating over UART.

The project continuously monitors vehicle subsystems, detects abnormal conditions, stores Diagnostic Trouble Codes (DTCs) in external EEPROM, and allows the driver to retrieve fault information through a keypad-controlled LCD interface.

This project was developed as the **Final Capstone Project** for the **Standard Embedded Systems Diploma** at **Edges for Training**.

---

# 🏗️ System Architecture

```
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
                 | EEPROM (I2C)         |
                 +----------------------+
```

---

# 🛠️ Hardware Components

| Component | Interface |
|------------|-----------|
| ATmega32 (x2) | UART |
| HC-SR04 Ultrasonic | ICU |
| LM35 Temperature Sensor | ADC |
| 24C16 EEPROM | I²C |
| LCD 20x4 | GPIO |
| 4×4 Keypad | GPIO |
| DC Motors | PWM + GPIO |

---

# 💻 Software Architecture

The project follows a **Layered Embedded Software Architecture**.

```
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

```
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

# 🚨 Diagnostic Trouble Codes

| Code | Description |
|------|-------------|
| P001 | Vehicle Too Close to Obstacle |
| P002 | Engine Over Temperature |

---

# 📂 Repository Structure

```
Vehicle-Fault-Detection-System

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
├── Docs
│
├── Images
│
└── README.md
```

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

# 🚀 Getting Started

## Clone the Repository

```bash
git clone https://github.com/yourusername/Vehicle-Fault-Detection-System.git
```

## Build

Compile the project using **AVR-GCC** or import it into **Eclipse IDE**.

## Flash

Upload each project to its corresponding ATmega32 microcontroller.

## Run

Power both ECUs and interact with the HMI through the keypad.

---

# 📸 Screenshots

> Add images here showing:

- LCD Main Menu
- Live Sensor Values
- Fault Retrieval Screen
- Hardware Setup
- Proteus Simulation

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

# 👨‍💻 Author

## Omar Ahmed

**Communication Engineering Student**

Embedded Systems • IoT • Automotive Software • Artificial Intelligence

📧 **Email:** omartoleba04@email.com

💻 **GitHub:** https://github.com/omartoleba

💼 **LinkedIn:** https://linkedin.com/in/omar-ahmed-959189308

---

# ⭐ Support

If you found this project useful or interesting, consider giving it a **⭐ Star**.

It helps others discover the project and supports future development.

---
