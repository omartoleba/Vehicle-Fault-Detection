 /*
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Omar Toleba
 *
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Decleration                                   *
 *******************************************************************************/

typedef enum
{
	BAUD_RATE_10=10, BAUD_RATE_300=300, BAUD_RATE_600=600, BAUD_RATE_1200=1200,
	BAUD_RATE_2400=2400, BAUD_RATE_4800=4800, BAUD_RATE_9600=9600 ,BAUD_RATE_14400=14400,
	BAUD_RATE_19200=19200, BAUD_RATE_38400=38400, BAUD_RATE_57600=57600,
	BAUD_RATE_115200=115200, BAUD_RATE_128000=128000, BAUD_RATE_256000=256000
}UART_baud_Rates;

typedef enum
{
	DISABLE_PARITY , EVEN_PARITY = 2 , ODD_PARITY
}UART_parityMode;

typedef enum
{
	ONE_BIT , TWO_BIT
}UART_stopBitSetting;

typedef enum
{
	FIVE_BITS , SIX_BITS , SEVEN_BITS , EIGHT_BITS
}UART_dataBitsNumber;

typedef struct
{
	UART_baud_Rates baud_rate;
	UART_parityMode mode;
	UART_stopBitSetting stopSetting;
	UART_dataBitsNumber dataNumber;

}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

/*
 * Description :
 * Check first if there is a data to receive
 */

uint8 UART_isDataAvailable(void);

#endif /* UART_H_ */
