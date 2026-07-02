# 🚗 Vehicle Fault Detection and Logging System (VFDLS)

A professional-grade dual-ECU embedded system built on two ATmega32 microcontrollers communicating via UART — continuously monitoring vehicle subsystems, detecting faults in real time, logging Diagnostic Trouble Codes (DTCs) to external EEPROM, and reporting them to the driver through a keypad-driven LCD interface.

![Status](https://img.shields.io/badge/status-complete-brightgreen)
![Platform](https://img.shields.io/badge/platform-ATmega32-blue)
![Language](https://img.shields.io/badge/language-C-orange)
![Architecture](https://img.shields.io/badge/architecture-layered-lightgrey)
![Protocol](https://img.shields.io/badge/protocol-UART%20%7C%20I2C%20%7C%20ADC%20%7C%20ICU%20%7C%20PWM-yellow)
![Frequency](https://img.shields.io/badge/clock-8MHz-red)

---

## 📸 Demo

<img width="1465" height="806" alt="image" src="https://github.com/user-attachments/assets/000121f6-d0e2-4fce-8dbb-ac2c7e8cc69b" />

## 📋 Table of Contents

- [Overview](#-overview)
- [Why This Project Matters](#-why-this-project-matters)
- [System Architecture](#-system-architecture)
- [Dual ECU Layered Model](#-dual-ecu-layered-model)
- [Hardware Components](#-hardware-components)
- [Subsystems & Fault Detection](#-subsystems--fault-detection)
- [UART Communication Protocol](#-uart-communication-protocol)
- [System Workflow](#-system-workflow)
- [DTC Error Codes](#️-dtc-error-codes)
- [Driver Specifications](#-driver-specifications)
- [Source Code](#-source-code)
- [Getting Started](#-getting-started)

---

## 🔭 Overview

The **Vehicle Fault Detection and Logging System (VFDLS)** is a final embedded systems diploma project that simulates a real automotive diagnostic system — similar in concept to the OBD-II systems found in modern cars.

The system uses two ATmega32 microcontrollers working together as independent ECUs (Electronic Control Units), communicating over UART:

| ECU | Role |
|---|---|
| **HMI ECU** (ATmega32 #1) | Driver interface — Keypad input, LCD output, menu navigation |
| **Control ECU** (ATmega32 #2) | Core engine — Sensor reading, fault detection, actuator control, EEPROM logging |

Faults are stored permanently as Diagnostic Trouble Codes (DTCs) in an external **24C16 EEPROM** via I2C — surviving even after power loss — and can be retrieved and displayed on demand, exactly like a real vehicle diagnostic scanner.

Built as the **Final Project of the Standard Embedded Diploma under Edges for Training**.

---

## 💡 Why This Project Matters

Most embedded projects use a single microcontroller. This project deliberately mirrors how real automotive systems are designed:

- **Separation of concerns** — the HMI never talks to sensors directly; the Control ECU never drives the display
- **UART inter-ECU communication** — structured command/response protocol between two independent processors
- **Persistent fault logging** — DTCs stored in non-volatile EEPROM, readable after power reset
- **Configurable drivers** — UART, ADC, I2C, and Timer all initialized via configuration structs, not hardcoded values
- **Full layered architecture** — two separate layered stacks, one per ECU

This is the same fundamental design pattern used in automotive ECUs, industrial PLCs, and safety-critical embedded systems.

---

## 🏗 System Architecture

                    ┌─────────────────────────────────────────────────────┐
                    │                   VFDLS SYSTEM                      │
                    │                                                     │
  ┌─────────────────┴──────────┐  UART  ┌───────────────────────────────┐│
  │       HMI ECU              │◄──────►│        Control ECU            ││
  │     ATmega32 #1            │        │       ATmega32 #2             ││
  │       @ 8 MHz              │        │         @ 8 MHz               ││
  │                            │        │                               ││
  │  ┌──────────┐ ┌─────────┐  │        │  ┌───────────┐ ┌──────────┐  ││
  │  │ 4×4      │ │ 4×16    │  │        │  │ HC-SR04   │ │  LM35    │  ││
  │  │ Keypad   │ │  LCD    │  │        │  │Ultrasonic │ │ Temp     │  ││
  │  └──────────┘ └─────────┘  │        │  └─────┬─────┘ └────┬─────┘  ││
  │                            │        │        │ ICU         │ ADC    ││
  │  Drivers: GPIO, UART,      │        │  ┌─────┴─────────────┴──────┐ ││
  │           Timer, LCD,      │        │  │     ATmega32 #2 Core     │ ││
  │           Keypad           │        │  └──┬──────────┬────────────┘ ││
  │                            │        │     │ I2C      │ GPIO/PWM    ││
  └────────────────────────────┘        │  ┌──┴───┐  ┌───┴────────┐    ││
                                        │  │24C16 │  │ DC Motors  │    ││
                                        │  │EEPROM│  │ (Windows)  │    ││
                                        │  └──────┘  └────────────┘    ││
                                        └───────────────────────────────┘│
                    └─────────────────────────────────────────────────────┘

---

## 🧱 Dual ECU Layered Model

Each ECU has its own independent layered software stack:

### HMI ECU Stack

\`\`\`
┌────────────────────────────────────────┐
│           APPLICATION LAYER            │  ← Menu logic, command sending,
│                                         │    LCD display management
├──────────────────┬─────────────────────┤
│     Keypad        │        LCD          │  ← HAL Layer
├────────┬──────────┴──┬──────────────────┤
│  GPIO  │   Timer     │      UART        │  ← MCAL Layer
└────────┴─────────────┴──────────────────┘
         ATmega32 #1 Hardware
\`\`\`

### Control ECU Stack

\`\`\`
┌────────────────────────────────────────┐
│           APPLICATION LAYER            │  ← Fault detection, DTC logging,
│                                         │    sensor polling, actuator control
├────────┬──────────┬────────────────────┤
│ Motor  │   LM35   │      EEPROM        │  ← HAL Layer
│        │          │  Ultrasonic        │
├────────┴──┬───────┴──┬──────┬──────────┤
│   GPIO    │   PWM    │ ADC  │ UART │I2C│  ← MCAL Layer
└───────────┴──────────┴──────┴──────┴───┘
            ATmega32 #2 Hardware
\`\`\`

---

## 🔧 Hardware Components

| Component | Connected To | Interface |
|---|---|---|
| 4×4 Keypad | HMI ECU | GPIO |
| 4×16 LCD Display | HMI ECU | GPIO (8-bit) |
| HC-SR04 Ultrasonic | Control ECU | ICU (PD6 echo, PD7 trig) |
| LM35 Temperature Sensor | Control ECU | ADC channel |
| 24C16 External EEPROM | Control ECU | I2C |
| DC Motors × 2 (Car Windows) | Control ECU | GPIO + PWM |
| ATmega32 #1 (HMI ECU) | — | UART TX/RX |
| ATmega32 #2 (Control ECU) | — | UART TX/RX |

---

## 🔍 Subsystems & Fault Detection

### 🅿️ Parking Assistance Unit (HC-SR04 Ultrasonic)

- Continuously measures distance between car and obstacles
- Uses ICU (Input Capture Unit) for precise echo timing
- Distance calculated from echo pulse width

| Condition | Action | DTC |
|---|---|---|
| Distance < 10 cm | Log fault to EEPROM | P001 — ACCIDENT_MIGHT_HAPPENED |
| Distance ≥ 10 cm | Normal operation | — |

### 🌡️ Engine Temperature Monitoring Unit (LM35)

- Reads engine temperature via ADC
- LM35 output: 10mV per °C, converted via ADC

| Condition | Action | DTC |
|---|---|---|
| Temperature > 90°C | Log fault to EEPROM | P002 — ENGINE_HIGH_TEMPERATURE |
| Temperature ≤ 90°C | Normal operation | — |

### 🪟 Window Control Unit (DC Motors)

- Two DC motors simulate car windows (Open / Close)
- Controlled by 2 push buttons per window
- State (Open/Closed) reported to HMI ECU for LCD display
- Motors always run at maximum speed via PWM Timer0

### 💾 External EEPROM (24C16 via I2C)

- Stores all DTCs permanently — survives power reset
- Each fault logged once per occurrence
- DTCs retrievable on demand via HMI keypad command

---

## 📡 UART Communication Protocol

All communication between the two ECUs follows a structured command/response protocol:

### HMI ECU → Control ECU (Commands)

| Command | Action |
|---|---|
| Command 1 | Start Monitoring — activate all sensors and actuators |
| Command 2 | Send Live Sensor Values — return Temp, Distance, Window states |
| Command 3 | Send Logged Faults — return all DTCs from EEPROM |
| Command 4 | Stop Monitoring — halt all sensor polling |

### Control ECU → HMI ECU (Responses)

| Response | Content |
|---|---|
| Sensor data | Temperature (°C), Distance (cm), Window 1 state, Window 2 state |
| DTC list | All stored P001/P002 codes with descriptions |

### UART Configuration (both ECUs)

\`\`\`c
UART_ConfigType uart_config = {
    .bit_data = UART_8_BIT_DATA,
    .parity   = UART_NO_PARITY,
    .stop_bit = UART_1_STOP_BIT,
    .baud_rate = 9600
};
UART_init(&uart_config);
\`\`\`

---

## 🖥️ System Workflow

### Main Menu (LCD)

\`\`\`
┌──────────────────────┐
│ 1. Start Operation   │
│ 2. Display Values    │
│ 3. Retrieve Faults   │
│ 4. Stop Monitoring   │
└──────────────────────┘
\`\`\`

### Key 1 — Start Operation

\`\`\`
User presses 1
      │
      ▼
HMI sends "Start" → Control ECU
      │
      ▼
Control ECU activates: Ultrasonic + LM35 + Motors
Fault detection & DTC logging begin
      │
      ▼
LCD shows:
┌──────────────────────┐
│  Operation Started!  │
│  Monitoring Active...│
└──────────────────────┘
      │
  10 seconds → back to Main Menu
\`\`\`

### Key 2 — Display Live Values

\`\`\`
LCD shows for 10 seconds:
┌──────────────────────┐
│  Temp: 82 C          │
│  Distance: 40 cm     │
│  Win1: Closed        │
│  Win2: Open          │
└──────────────────────┘

Then prompts:
┌──────────────────────┐
│  Display again?      │
│  Press 2=YES         │
│  Other key=MAIN MENU │
└──────────────────────┘
\`\`\`

### Key 3 — Retrieve Logged Faults

\`\`\`
LCD scrolls through all stored DTCs:
┌──────────────────────┐
│  Logged Faults:      │
│  P001:DistanceTooClose│
│  P002: Overheat      │
│  --End of List--     │
└──────────────────────┘
\`\`\`

### Key 4 — Stop Monitoring

\`\`\`
LCD shows:
┌──────────────────────┐
│  System Monitoring   │
│  Stopped!             │
│  Returning to Menu…   │
└──────────────────────┘
10 seconds → back to Main Menu
\`\`\`

---

## ⚠️ DTC Error Codes

| DTC Code | Description | Trigger Condition | Logged |
|---|---|---|---|
| P001 | ACCIDENT_MIGHT_HAPPENED | Distance < 10 cm | Once per occurrence |
| P002 | ENGINE_HIGH_TEMPERATURE | Temperature > 90°C | Once per occurrence |

DTCs are stored permanently in the 24C16 EEPROM and persist after power reset.

---

## 🔌 Driver Specifications

### HMI ECU — MCAL Layer

**UART Driver (configurable)**

\`\`\`c
typedef struct {
    UART_BitDataType  bit_data;
    UART_ParityType   parity;
    UART_StopBitType  stop_bit;
    UART_BaudRateType baud_rate;
} UART_ConfigType;

void UART_init(const UART_ConfigType *Config_Ptr);
\`\`\`

**Timer Driver (Timer0 / Timer1 / Timer2 — dynamic config)**

\`\`\`c
typedef struct {
    uint16         timer_InitialValue;
    uint16         timer_compare_MatchValue; // compare mode only
    Timer_ID_Type  timer_ID;
    Timer_ClockType timer_clock;
    Timer_ModeType  timer_mode;
} Timer_ConfigType;

void Timer_init(const Timer_ConfigType *Config_Ptr);
void Timer_deInit(Timer_ID_Type timer_type);
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);
\`\`\`

> Project uses Timer1 in Compare mode for HMI ECU timing.

**GPIO Driver**

\`\`\`c
// Same GPIO driver used across all projects
// Handles pin direction and read/write for all ports
\`\`\`

### HMI ECU — HAL Layer

**LCD Driver**

\`\`\`c
// 4×16 LCD in 8-bit mode
// Connected to HMI ECU
\`\`\`

**Keypad Driver**

\`\`\`c
// 4×4 Keypad
// Connected to HMI ECU
\`\`\`

### Control ECU — MCAL Layer

**ADC Driver (configurable)**

\`\`\`c
typedef struct {
    ADC_ReferenceVoltage ref_volt;
    ADC_Prescaler        prescaler;
} ADC_ConfigType;

void ADC_init(const ADC_ConfigType *Config_Ptr);
uint16 ADC_readChannel(uint8 channel_num);
\`\`\`

**I2C Driver (configurable)**

\`\`\`c
typedef struct {
    TWI_AddressType address;
    TWI_BaudRateType bit_rate;
} TWI_ConfigType;

void TWI_init(const TWI_ConfigType *Config_Ptr);
\`\`\`

**PWM Driver (Timer0)**

\`\`\`c
void PWM_Timer0_Start(uint8 duty_cycle);
// Non-inverting mode | Prescaler: F_CPU/64 | Output: OC0
\`\`\`

**ICU Driver**

\`\`\`c
// F_CPU/8 prescaler | First edge: Rising
// Used by Ultrasonic driver
\`\`\`

**UART Driver**

\`\`\`c
// Same configurable UART driver as HMI ECU
// Shared between both ECUs
\`\`\`

### Control ECU — HAL Layer

**Ultrasonic Driver (HC-SR04)**

\`\`\`c
void Ultrasonic_init(void);
void Ultrasonic_Trigger(void);
uint16 Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void); // ICU callback
\`\`\`

**LM35 Temperature Sensor**

\`\`\`c
// ADC channel input | Returns temperature in °C
\`\`\`

**DC Motor Driver**

\`\`\`c
void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state, uint8 speed);
// Always runs at 100% speed via Timer0 PWM
\`\`\`

**External EEPROM Driver (24C16)**

\`\`\`c
// Controlled via I2C driver
// Stores and retrieves DTCs permanently
\`\`\`

---

## 💻 Source Code

Full application-layer source for both ECUs lives under `HMI_ECU/APP/HMI_main.c` and `Control_ECU/APP/Control_main.c` — see the Project Structure below for the complete layout.

---

## 🚀 Getting Started

### Prerequisites

- Atmel/Microchip Studio or Eclipse IDE with AVR plugin
- AVR-GCC toolchain
- AVRDUDE + USBasp programmer ×2 (one per ECU)
- Proteus for simulation

### Project Structure

\`\`\`
VFDLS/
├── HMI_ECU/
│   ├── APP/
│   │   └── HMI_main.c
│   ├── HAL/
│   │   ├── LCD/
│   │   └── Keypad/
│   └── MCAL/
│       ├── GPIO/
│       ├── UART/
│       └── Timer/
│
└── Control_ECU/
    ├── APP/
    │   └── Control_main.c
    ├── HAL/
    │   ├── LM35/
    │   ├── Ultrasonic/
    │   ├── DC_Motor/
    │   └── EEPROM/
    └── MCAL/
        ├── GPIO/
        ├── ADC/
        ├── UART/
        ├── PWM/
        ├── ICU/
        └── I2C/
\`\`\`

---

*This was the final capstone project of the Standard Embedded Systems Diploma under Edges for Training, supervised by Eng. Mohamed Tarek. It represents the most complex project in the diploma — combining dual-ECU architecture, five communication protocols (UART, I2C, ADC, ICU, PWM), persistent non-volatile fault logging, and a complete automotive-grade software design pattern.*

**Final Capstone Project — Standard Embedded Systems Diploma | Edges for Training | 2025**
