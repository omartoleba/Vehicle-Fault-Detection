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
