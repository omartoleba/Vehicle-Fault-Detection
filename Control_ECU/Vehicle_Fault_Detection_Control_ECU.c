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

