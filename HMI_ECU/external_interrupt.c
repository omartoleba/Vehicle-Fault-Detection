 /*
 *
 * Module: EXTERNAL INTERRUPT
 *
 * File Name: external interrupt.c
 *
 * Description: Source file for the AVR EXTERNAL INTERRUPT driver
 *
 * Author: Omar Toleba
 *
 */

#include "gpio.h"
#include "external_interrupt.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*
 * Array of Global variables to hold the address of
 * the external interrupt call back function in the application
 */
static volatile void (*g_externalInterruptCallBackPtr[NO_OF_EXTERNALINTERRUPT])(void) = {NULL_PTR};

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR (INT0_vect)
{
	if(g_externalInterruptCallBackPtr[INTERRUPT0] != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt0 happend */
		(g_externalInterruptCallBackPtr[INTERRUPT0])();
	}
}

ISR (INT1_vect)
{
	if(g_externalInterruptCallBackPtr[INTERRUPT1] != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt0 happend */
		(g_externalInterruptCallBackPtr[INTERRUPT1])();
	}
}

ISR (INT2_vect)
{
	if(g_externalInterruptCallBackPtr[INTERRUPT2] != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt0 happend */
		(g_externalInterruptCallBackPtr[INTERRUPT2])();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ExternalInterrupt_init(External_Interrupt_ConfigType * Config_ptr)
{
	switch(Config_ptr -> interrupt_id)
	{
	case INTERRUPT0:
		GPIO_setupPinDirection(EXTERNAL_INTERRUPT0_PORT_ID , EXTERNAL_INTERRUPT0_PIN_ID , PIN_INPUT);
		GICR |= (1 << INT0);
		MCUCR = MCUCR | ((Config_ptr -> interrupt_edge) & 0x03);
		break;
	case INTERRUPT1:
		GPIO_setupPinDirection(EXTERNAL_INTERRUPT1_PORT_ID , EXTERNAL_INTERRUPT1_PIN_ID , PIN_INPUT);
		GICR |= (1 << INT1);
		MCUCR = MCUCR | (((Config_ptr -> interrupt_edge) << 2) & 0x0C);
		break;
	case INTERRUPT2:
		GPIO_setupPinDirection(EXTERNAL_INTERRUPT2_PORT_ID , EXTERNAL_INTERRUPT2_PIN_ID , PIN_INPUT);
		GICR |= (1 << INT2);
		/*
		 * We will use the first bit from falling(0) or rising edge(1)
		 * to determine it in interrupt2
		 */
		MCUCSR = MCUCSR | (((Config_ptr -> interrupt_edge) << 6) & 0x40);
		break;
	}
}


void ExternalInterrupt_deinit(External_Interrupt_ID_type interrupt_id)
{
	switch(interrupt_id)
		{
		case INTERRUPT0:
			GICR &= ~(1 << INT0);
			MCUCR &= ~(1 << ISC00);
			MCUCR &= ~(1 << ISC01);
			break;
		case INTERRUPT1:
			GICR &= ~(1 << INT1);
			MCUCR &= ~(1 << ISC10);
			MCUCR &= ~(1 << ISC11);
			break;
		case INTERRUPT2:
			GICR &= ~(1 << INT2);
			MCUCSR &= ~(1 << ISC2);
			break;
		}
}


void ExternalInterrupt_setCallBack(void(*a_ptr)(void), External_Interrupt_ID_type a_interrupt_ID)
{
	/* Save the address of the Call back function in a global variable */
	g_externalInterruptCallBackPtr[a_interrupt_ID] = a_ptr;
}
