 /*
 *
 * Module: EXTERNAL INTERRUPT
 *
 * File Name: external interrupt.h
 *
 * Description: Header file for the AVR EXTERNAL INTERRUPT driver
 *
 * Author: Omar Toleba
 *
 */

#ifndef EXTERNAL_INTERRUPT_H_
#define EXTERNAL_INTERRUPT_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define NO_OF_EXTERNALINTERRUPT			3

#define EXTERNAL_INTERRUPT0_PORT_ID		PORTD_ID
#define EXTERNAL_INTERRUPT0_PIN_ID		PIN2_ID

#define EXTERNAL_INTERRUPT1_PORT_ID		PORTD_ID
#define EXTERNAL_INTERRUPT1_PIN_ID		PIN3_ID

#define EXTERNAL_INTERRUPT2_PORT_ID		PORTB_ID
#define EXTERNAL_INTERRUPT2_PIN_ID		PIN2_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	FALLING_EDGE = 2, RISING_EDGE
}External_Interrupt_EdgeType;

typedef enum
{
	INTERRUPT0 , INTERRUPT1 , INTERRUPT2
}External_Interrupt_ID_type;

typedef struct
{
	External_Interrupt_ID_type interrupt_id;
	External_Interrupt_EdgeType interrupt_edge;
}External_Interrupt_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void ExternalInterrupt_init(External_Interrupt_ConfigType * Config_ptr);

void ExternalInterrupt_deinit(External_Interrupt_ID_type interrupt_id);

void ExternalInterrupt_setCallBack(void(*a_ptr)(void), External_Interrupt_ID_type a_interrupt_ID);

#endif /* EXTERNAL_INTERRUPT_H_ */
