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
- [Source Code](#-source-code)

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

\`\`\`mermaid
flowchart LR
    subgraph HMI["HMI ECU — ATmega32 #1 @ 8MHz"]
        Keypad["4x4 Keypad"]
        LCD["4x16 LCD"]
        HMICore["Drivers: GPIO, UART, Timer, LCD, Keypad"]
        Keypad --> HMICore
        LCD --> HMICore
    end

    subgraph CTRL["Control ECU — ATmega32 #2 @ 8MHz"]
        US["HC-SR04 Ultrasonic"] -- ICU --> CTRLCore["ATmega32 #2 Core"]
        LM35["LM35 Temp Sensor"] -- ADC --> CTRLCore
        CTRLCore -- I2C --> EEPROM["24C16 EEPROM"]
        CTRLCore -- GPIO/PWM --> Motors["DC Motors (Windows)"]
    end

    HMICore <-- UART --> CTRLCore
\`\`\`

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

## 💻 Source Code

## HMI ECU

## HMI_main.c — HMI Application Layer

/* File name: Vehicle_Fault_Detection_HMI_ECU.c
 *
 * Description:
 * Display live values of the temperature,distance
 * and windows state on LCD.
 * Report faults to the driver on LCD interface.
 *
 * Created on: Oct 6, 2025
 *
 * Author: Omar Toleba
 */

#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>




/*==================Definitions==================*/

#define YES							1
#define NO							0

#define CONTROL_ECU_READY 			60

#define OPEN 						1
#define CLOSE 						0

#define RECIVED						1

#define START_MONITOR   			1
#define DISPLAY_VALUES  			2
#define RETRIEVE_FAULTS 			3
#define STOP_MONITOR    			4

/*window defines*/
#define WIN_PORT       				PORTB_ID
#define WIN1_OPEN_PIN       		PIN0_ID
#define WIN1_CLOSE_PIN    		  	PIN1_ID
#define WIN2_OPEN_PIN      			PIN2_ID
#define WIN2_CLOSE_PIN     			PIN3_ID
#define WIN1_OPEN					11
#define WIN1_CLOSE					10
#define WIN2_OPEN					21
#define WIN2_CLOSE					20
/*==============================================*/


/*===============Global Variables===============*/

/*to get the option that the user choose from the menu*/
uint8 g_options = 0;

/*
 * to detect the engine state
 * to prevent the user display before monitor opened
 * or after monitor closed
 */
uint8 g_engineState = CLOSE;

/*to get the window state*/
uint8 g_window1State = CLOSE;
uint8 g_window2State = CLOSE;

/*
 * to prevent (open/close) the window
 * while its already (open/close)
 */
uint8 g_motor1State = CLOSE;
uint8 g_motor2State = CLOSE;
/*==============================================*/

/*
 * Discription:
 * Display the main menu on the LCD
 */
void display_main_menu(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"1.Start Engine");
	LCD_displayStringRowColumn(1,0,"2.Display Values");
	LCD_displayStringRowColumn(2,0,"3.Retrieve Faults");
	LCD_displayStringRowColumn(3,0,"4.Stop Engine");
}

/*(Call back function for timer1)
 *
 * Discription:
 * wait until 10 sec to pass so we can return to the main menu
 * and return the option value to zero
 */
void timer10sec_Handler (void)
{
	static uint8 tick = 0;
	tick++;
	if (tick == 2)
	{
		tick = 0;


		if (g_options == START_MONITOR)
		{
			g_options = 0;
			Timer_deInit(TIMER1);
			display_main_menu();
			g_engineState = OPEN;
		}
		else if(g_options == DISPLAY_VALUES)
		{
			g_options = 0;
		}
		else if (g_options == RETRIEVE_FAULTS)
		{
			g_options = 0;
		}
		else if (g_options == STOP_MONITOR)
		{
			g_options = 0;
			Timer_deInit(TIMER1);
			display_main_menu();
			g_engineState = CLOSE;
		}
	}
}

/*
 * Discription:
 * check the four button of the window and if pressed
 * send to the control ECU to open or close the two window
 * */
void windowCheck (void)
{

	if (g_engineState != OPEN)
	{
		 return;
	}

	if(GPIO_readPin(WIN_PORT,WIN1_OPEN_PIN) == LOGIC_HIGH && g_motor1State == CLOSE)
	{
		UART_sendByte(WIN1_OPEN);
		UART_recieveByte();
		g_motor1State = OPEN;
	}
	if(GPIO_readPin(WIN_PORT,WIN1_CLOSE_PIN) == LOGIC_HIGH && g_motor1State == OPEN)
	{
		UART_sendByte(WIN1_CLOSE);
		UART_recieveByte();
		g_motor1State = CLOSE;
	}

	if(GPIO_readPin(WIN_PORT,WIN2_OPEN_PIN) == LOGIC_HIGH && g_motor2State == CLOSE)
	{
		UART_sendByte(WIN2_OPEN);
		UART_recieveByte();
		g_motor2State = OPEN;
	}
	if(GPIO_readPin(WIN_PORT,WIN2_CLOSE_PIN) == LOGIC_HIGH && g_motor2State == OPEN
					&& g_engineState == OPEN)
	{
		UART_sendByte(WIN2_CLOSE);
		UART_recieveByte();
		g_motor2State = CLOSE;
	}
}


int main (void)
{
	/*array to get the faults from control ECU (max number of faults is 10)*/
	uint8 faultsList[10][5];

	/*for the loops count*/
	uint8 i , j;

	/*to ask the user if he want to display again or not*/
	uint8 userChoice ;

	/*to get the number of errors logged in the EEPROM in the control ECU */
	uint8 errorNumber = 0;

	/*flag to detect if we going to display again or not*/
	uint8 displayAgain = YES;

	/*the two massage that we going to send it to the control ECU*/
	uint8 msg1[20] = "Start Monitoring#";
	uint8 msg2[20] ="Retrieve Faults#";

	/*to get the keypad pressed value*/
	uint8 key = 0;

	/*to get the temperature and ultrasonic sensors value from the control ECU*/
	uint8 temp , distance ;

	/* Enable Global Interrupts */
	SREG |= (1 << 7);

/* =====================TIMER===================== */
	/*Configure the timer1 to calculate 5 sec*/
	Timer_ConfigType timer_Configrations = { 0, 39061, TIMER1, F_CPU_1024,COMPARE_MODE };
	Timer_setCallBack(timer10sec_Handler, TIMER1);
/* =============================================== */

/* =====================UART===================== */
	/*Configure the UART*/
	UART_ConfigType uart_Configrations = { BAUD_RATE_9600, DISABLE_PARITY,ONE_BIT, EIGHT_BITS };
	UART_init(&uart_Configrations);
/* ============================================== */

/* =====================LCD===================== */
	LCD_init();
	display_main_menu();
/* ============================================= */

/* ===================BUTTONS==================== */
	GPIO_setupPinDirection(WIN_PORT, WIN1_OPEN_PIN, PIN_INPUT);
	GPIO_setupPinDirection(WIN_PORT, WIN1_CLOSE_PIN, PIN_INPUT);
	GPIO_setupPinDirection(WIN_PORT, WIN2_OPEN_PIN, PIN_INPUT);
	GPIO_setupPinDirection(WIN_PORT, WIN2_CLOSE_PIN, PIN_INPUT);
/* ============================================== */

	while (1)
	{
		windowCheck();

		/* get the value from the keypad */
		key = KEYPAD_getPressedKey();

		/*Opening the engine*/
		if (key == START_MONITOR && g_engineState == CLOSE)
		{
			/*Send the command to the control ECU*/
			UART_sendByte(START_MONITOR);
			UART_recieveByte();

			g_options = START_MONITOR;

			/*send the massage "Start Monitoring#" to the control ECU*/
			UART_sendString(msg1);
			if (UART_recieveByte() == CONTROL_ECU_READY)
			{
				LCD_clearScreen();
				LCD_displayString("Engine Started!");
				LCD_displayStringRowColumn(1, 0, "Activating...");
				Timer_init(&timer_Configrations);
			}
		}
		/*Displaying the data from Control_ECU in the lcd on HMI_ECU*/
		else if (key == DISPLAY_VALUES && g_engineState == OPEN)
		{
			displayAgain = YES;
			while(displayAgain == YES)
			{
				g_options = DISPLAY_VALUES;

				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Temp =   C");
				LCD_displayStringRowColumn(1, 0, "Distance =   cm");
				LCD_displayStringRowColumn(2, 0, "Win1:");
				LCD_displayStringRowColumn(3, 0, "Win2:");

				Timer_init(&timer_Configrations);

				while (g_options == DISPLAY_VALUES)
				{
					windowCheck();
					/*Send the command to the control ECU*/
					UART_sendByte(DISPLAY_VALUES);

					if (UART_recieveByte() == RECIVED)
					{
						/*
						 * Recieve values of the temperature and distance
						 * and two motor states from the control ECU
						 */
						temp = UART_recieveByte();
						UART_sendByte(RECIVED);
						distance = UART_recieveByte();
						UART_sendByte(RECIVED);
						g_window1State = UART_recieveByte();
						UART_sendByte(RECIVED);
						g_window2State = UART_recieveByte();
						UART_sendByte(RECIVED);

						LCD_moveCursor(0, 6);
						/*Display the temperature*/
						if (temp >= 100)
						{
							LCD_intgerToString(temp);
						}
						else if (temp >= 10)
						{
							LCD_intgerToString(temp);
							LCD_displayString(" ");
						}
						else
						{
							LCD_intgerToString(temp);
							LCD_displayString("  ");
						}
						/*Display the Distance*/
						LCD_moveCursor(1, 10);

						if (distance >= 100)
						{
							LCD_intgerToString(distance);
						}
						else if (distance >= 10)
						{
							LCD_intgerToString(distance);
							LCD_displayString(" ");
						}
						else
						{
							LCD_intgerToString(distance);
							LCD_displayString("  ");
						}

						/*Display the Motor1 state*/
						LCD_moveCursor(2, 5);

						if (g_motor1State == OPEN)
						{
							LCD_displayString("Open ");
						}
						else
						{
							LCD_displayString("Close");
						}

						/*Display the Motor2 state*/
						LCD_moveCursor(3, 5);

						if (g_motor2State == OPEN)
						{
							LCD_displayString("Open ");
						}
						else
						{
							LCD_displayString("Close");
						}
					}
				}
				/*
				 * After the 10 sec end
				 * ask the user if he want to display again or not
				 * if not then return to the main menu
				 */
				Timer_deInit(TIMER1);
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Display again?");
				LCD_displayStringRowColumn(1,0,"Press 2 = YES");
				LCD_displayStringRowColumn(2,0,"OTHER key = NO");

				/*delay for a keypad to pervent multiple pressed*/
				_delay_ms(500);
				userChoice = KEYPAD_getPressedKey();

				if (userChoice == 2)
				{
					displayAgain = YES;
				}
				else
				{
					displayAgain = NO;
					display_main_menu();
				}
			}
		}
		/*Displaying the faults from Control_ECU in the lcd on HMI_ECU*/
		else if (key == RETRIEVE_FAULTS && g_engineState == OPEN)
		{
			displayAgain = YES;
			while(displayAgain == YES)
			{
				g_options = RETRIEVE_FAULTS;

				LCD_clearScreen();

				Timer_init(&timer_Configrations);

				if(g_options == RETRIEVE_FAULTS)
				{
					/*Send the command to the control ECU*/
					UART_sendByte(RETRIEVE_FAULTS);
					UART_recieveByte();
					/*Recieve the number of errors logged*/
					errorNumber = UART_recieveByte();
					UART_sendByte(RECIVED);
					/*send the massage "Retrieve Faults#" to the control ECU*/
					UART_sendString(msg2);
					UART_recieveByte();

					if (errorNumber == 0)
					{
						LCD_displayStringRowColumn(0, 0, "No Faults Logged");
					}
					else
					{
						LCD_displayStringRowColumn(0, 0, "Logged Faults:  ");

						for (i = 0; i < errorNumber; i++)
						{
							for (j = 0; j < 4; j++)
							{
								faultsList[i][j] = UART_recieveByte();
								UART_sendByte(RECIVED);
							}
						}
						/*
						 * Display the faults that we saved it early in the array
						 * in the lcd in scrolling way
						 */
						for (i = 0; i < errorNumber; i++)
						{

							if (faultsList[i][3] == '1')
							{
								LCD_displayStringRowColumn(1, 0,"P001: Over Heat ");
							}
							else if (faultsList[i][3] == '2')
							{
								LCD_displayStringRowColumn(1, 0,"P002: Too Close ");
							}

							if ((i + 1) < errorNumber)
							{
								if (faultsList[i+1][3] == '1')
								{
									LCD_displayStringRowColumn(2, 0,"P001: Over Heat ");
								}
								else if (faultsList[i+1][3] == '2')
								{
									LCD_displayStringRowColumn(2, 0,"P002: Too Close ");
								}
							}
							else
							{
								LCD_displayStringRowColumn(3, 0,"==End of List== ");
								break;
							}

							if ((i + 2) < errorNumber)
							{
								if (faultsList[i+2][3] == '1')
								{
									LCD_displayStringRowColumn(3, 0,"P001: Over Heat ");
								}
								else if (faultsList[i+2][3] == '2')
								{
									LCD_displayStringRowColumn(3, 0,"P002: Too Close ");
								}
							}
							else
							{
								LCD_displayStringRowColumn(3, 0,"==End of List== ");
								break;
							}
							/*wait 1 sec to scroll*/
							_delay_ms(1000);
						}
					}
					/*
					 * in case all the faults is displayed and still there is time
					 * until the 10 sec end so wait them to end to prevent repeat the
					 * loop
					 */
					while(g_options == RETRIEVE_FAULTS);
				}
				/*
				 * After the 10 sec end
				 * ask the user if he want to display again or not
				 * if not then return to the main menu
				 */
				errorNumber = 0;
				Timer_deInit(TIMER1);
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Display again?");
				LCD_displayStringRowColumn(1, 0, "Press 3 = YES");
				LCD_displayStringRowColumn(2, 0, "OTHER key = NO");

				/*delay for a keypad to pervent multiple pressed*/
				_delay_ms(500);
				userChoice = KEYPAD_getPressedKey();
				windowCheck();
				if (userChoice == 3)
				{
					displayAgain = YES;
				}
				else
				{
					displayAgain = NO;
					display_main_menu();
				}
			}
		}
		/*Closing the engine*/
		else if (key == STOP_MONITOR && g_engineState == OPEN)
		{
			g_options = STOP_MONITOR;

			/*Send the command to the control ECU*/
			UART_sendByte(STOP_MONITOR);
			if (UART_recieveByte() == RECIVED)
			{
				LCD_clearScreen();
				LCD_displayString("Engine Stopped");
				LCD_displayStringRowColumn(1, 0, "Return to Menu..");
				Timer_init(&timer_Configrations);
			}
			/*
			 * wait until 10 sec is end to prevent any other buttons
			 * pressed while monitor is closeing
			 */
			while (g_engineState != CLOSE);
		}

		/*delay for a keypad to pervent multiple pressed*/
		_delay_ms(500);
	}
}

## Control ECU

## Conrol_main.c — HMI Application Layer

/*
 *	File name: Vehicle_Fault_Detection_Control_ECU.c
 *
 *	Description:
 *	Reading the temperature and distance value from the sensors.
 *	Log errors (DTCs) into external EEPROM.
 *
 *  Created on: Oct 6, 2025
 *
 *  Author: Omar Toleba
 */

#include "gpio.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "lm35_sensor.h"
#include "twi.h"
#include "uart.h"
#include "adc.h"
#include "ultrasonic.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

/*==================Definitions==================*/
#define CONTROL_ECU_READY			60

#define OPEN						1
#define CLOSE						0

#define START_MONITOR				1
#define DISPLAY_VALUES				2
#define RETRIEVE_FAULTS				3
#define STOP_MONITOR    			4

#define RECIVED						1

/*window defines*/
#define WIN1_OPEN					11
#define WIN1_CLOSE					10
#define WIN2_OPEN					21
#define WIN2_CLOSE					20

/*==============================================*/

int main(void)
{
	/*
	 * to get the monitor state to prevent the control ECU get
	 * the values from sensors or store the faults in the EEPROM
	 */
	uint8 monitorState = STOP_MONITOR;

	/*to get the error from the EEPROM and send it to the HMI ECU*/
	uint8 arr_Error[5];

	/*for the loops count*/
	uint8 i , j;

	/*
	 * to get the previous values from the sensors
	 * to prevent duplicate storeing in the EEPROM
	 */
	uint8 prevTemp = 0 , prevDistance = 0;

	/*to get the massage from the HMI ECU*/
	uint8 str1[20] , str2[20];

	/*to get the value of the sensors and send it to the HMI ECU*/
	uint8 temp = 0, distance = 0 ;

	/*to send the motor state to the HMI ECU*/
	uint8 motor1State = CLOSE, motor2State = CLOSE;

	/*to get the command from the HMI ECU*/
	uint8 command = 0;

	/*to count the error number that stored in the EEPROM*/
	uint8 errorNumber = 0;

	/* Enable Global Interrupts */
	SREG |= (1 << 7);

	/* =====================UART===================== */
	UART_ConfigType uart_Configrations = {BAUD_RATE_9600, DISABLE_PARITY, ONE_BIT, EIGHT_BITS};
	UART_init(&uart_Configrations);
	/* ============================================== */

	/* =====================ADC====================== */
	ADC_ConfigType ADC_Configerations = {DIVISION_FACTOR_128, EXTERNAL_VREF};
	ADC_init(&ADC_Configerations);
	/* ============================================== */

	/* =====================TWI====================== */
	TWI_ConfigType TWI_Configerations = {0x01, FAST_MODE_400KBps};
	TWI_init(&TWI_Configerations);
	/* ============================================== */

	/* ==================ULTRASONIC================== */
	Ultrasonic_init();
	/* ============================================== */

	/* ===================DC_MOTOR=================== */
	DcMotor_Init();
	/* ============================================== */

	while (1)
	{
		if (monitorState == START_MONITOR)
		{
			/* Update sensor readings */
			prevTemp = temp;
			temp = LM35_getTemperature();
			prevDistance = distance;
			distance = Ultrasonic_readDistance();

			/*store the faults in EEPROM once every time temperature sensor beyond the 90C*/
			if (temp > 90 && prevTemp <= 90)
			{
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 0, 'P');
				_delay_ms(10);
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 1, '0');
				_delay_ms(10);
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 2, '0');
				_delay_ms(10);
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 3, '1');
				_delay_ms(10);
				errorNumber++;
			}
			/*store the faults in EEPROM once every time ultrasonic sensor below the 10cm*/
			if (distance < 10 && prevDistance >= 10)
			{
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 0, 'P');
				_delay_ms(10);
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 1, '0');
				_delay_ms(10);
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 2, '0');
				_delay_ms(10);
				EEPROM_writeByte(0x0000 + (errorNumber * 4) + 3, '2');
				_delay_ms(10);
				errorNumber++;
			}
		}
		/*
		 * if there is command wait for recived contine
		 * if not skip the whole code and loop again
		 */
		if (UART_isDataAvailable())
		{
			/*recive the command from the HMI ECU*/
			command = UART_recieveByte();
			UART_sendByte(RECIVED);

			if (command == START_MONITOR)
			{
				UART_receiveString(str1);
				if (strcmp((const char*) str1, "Start Monitoring") == 0)
				{
					/*send the HMI ECU that control ECU is ready for other options*/
					UART_sendByte(CONTROL_ECU_READY);
					monitorState = START_MONITOR;
				}
			}
			else if (command == DISPLAY_VALUES)
			{
				/*
				 * send the sensors values and two motor states
				 * to the HMI ECU so it can display them on the LCD
				 */
				UART_sendByte(temp);
				UART_recieveByte();
				UART_sendByte(distance);
				UART_recieveByte();
				UART_sendByte(motor1State);
				UART_recieveByte();
				UART_sendByte(motor2State);
				UART_recieveByte();

			}
			else if (command == RETRIEVE_FAULTS)
			{
				/*Send the number of errors logged to the HMI ECU*/
				UART_sendByte(errorNumber);
				UART_recieveByte();

				UART_receiveString(str2);
				if (strcmp((const char*) str2, "Retrieve Faults") == 0)
				{
					/*send the HMI ECU that you recived the massage*/
					UART_sendByte(RECIVED);

					/*
					 * this loop is to get the stored faults in the EEPROM
					 * and send them to the HMI ECU
					 */
					for (i = 0; i < errorNumber; i++)
					{
						/*get the stored faults in the EEPROM*/
						for (j = 0; j < 4; j++)
						{
							EEPROM_readByte(0x0000 + (i * 4) + j,&arr_Error[j]);
							_delay_ms(10);

							/*send the faults to the HMI ECU*/
							UART_sendByte(arr_Error[j]);
							UART_recieveByte();
						}
					}
				}
			}
			else if (command == STOP_MONITOR)
			{
				monitorState = STOP_MONITOR;
			}
			/*Window handels*/
			else if (command == WIN1_OPEN)
			{
				DcMotor_Rotate(A_CW, MOTOR1, 100);
				/*the motor will rotate and then will stop when the window opened(1.5Sec)*/
				_delay_ms(1500);
				DcMotor_Rotate(STOP, MOTOR1, 0);
				motor1State = OPEN;
			}
			else if (command == WIN1_CLOSE)
			{
				DcMotor_Rotate(CW, MOTOR1, 100);
				/*the motor will rotate and then will stop when the window closed(1.5Sec)*/
				_delay_ms(1500);
				DcMotor_Rotate(STOP, MOTOR1, 0);
				motor1State = CLOSE;
			}
			else if (command == WIN2_OPEN)
			{
				DcMotor_Rotate(A_CW, MOTOR2, 100);
				/*the motor will rotate and then will stop when the window opened(1.5Sec)*/
				_delay_ms(1500);
				DcMotor_Rotate(STOP, MOTOR2, 0);
				motor2State = OPEN;
			}
			else if (command == WIN2_CLOSE)
			{
				DcMotor_Rotate(CW, MOTOR2, 100);
				/*the motor will rotate and then will stop when the window closed(1.5Sec)*/
				_delay_ms(1500);
				DcMotor_Rotate(STOP, MOTOR2, 0);
				motor2State = CLOSE;
			}
		}
	}
}



---


*This was the final capstone project of the Standard Embedded Systems Diploma under Edges for Training, supervised by Eng. Mohamed Tarek. It represents the most complex project in the diploma — combining dual-ECU architecture, five communication protocols (UART, I2C, ADC, ICU, PWM), persistent non-volatile fault logging, and a complete automotive-grade software design pattern.*

**Final Capstone Project — Standard Embedded Systems Diploma | Edges for Training | 2025**
