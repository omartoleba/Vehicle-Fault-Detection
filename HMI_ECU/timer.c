 /*
 *
 * Module: TIMER
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR TIMER driver
 *
 * Author: Omar Toleba
 *
 */

#include "timer.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/*Array of Global variables to hold the address of the timer call back function in the application */
static volatile void (*g_timerCallBackPtr[NO_OF_TIMERS])(void) = {NULL_PTR};

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR (TIMER0_OVF_vect)
{
	if(g_timerCallBackPtr[TIMER0] != NULL_PTR)
		{
			/* Call the Call Back function in the application after the timer over flow */
			(*g_timerCallBackPtr[TIMER0])();
		}
}

ISR (TIMER0_COMP_vect)
{
	if(g_timerCallBackPtr[TIMER0] != NULL_PTR)
		{
			/* Call the Call Back function in the application after the timer reach the compare value */
			(*g_timerCallBackPtr[TIMER0])();
		}
}

ISR (TIMER1_OVF_vect)
{
	if(g_timerCallBackPtr[TIMER1] != NULL_PTR)
		{
			/* Call the Call Back function in the application after the timer over flow */
			(*g_timerCallBackPtr[TIMER1])();
		}
}

ISR (TIMER1_COMPA_vect)
{
	if(g_timerCallBackPtr[TIMER1] != NULL_PTR)
		{
			/* Call the Call Back function in the application after the timer reach the compare value */
			(*g_timerCallBackPtr[TIMER1])();
		}
}

ISR (TIMER2_OVF_vect)
{
	if(g_timerCallBackPtr[TIMER2] != NULL_PTR)
		{
			/* Call the Call Back function in the application after the timer over flow */
			(*g_timerCallBackPtr[TIMER2])();
		}
}

ISR (TIMER2_COMP_vect)
{
	if(g_timerCallBackPtr[TIMER2] != NULL_PTR)
		{
			/* Call the Call Back function in the application after the timer reach the compare value */
			(*g_timerCallBackPtr[TIMER2])();
		}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer_init (const Timer_ConfigType * Config_Ptr)
{
	switch (Config_Ptr -> timer_ID)
	{
	case TIMER0:
		if (Config_Ptr -> timer_mode == NORMAL_MODE)
		{
			/* Set Timer initial value */

			TCNT0 = Config_Ptr -> timer_InitialValue;

			/* Enable Timer0 Overflow Interrupt */

			TIMSK |= (1<<TOIE0);

			/* configure the timer0 */

			TCCR0 = (1<<FOC0) | (TCCR0 & 0xF8) | (Config_Ptr -> timer_clock & 0x07);
		}
		else if (Config_Ptr -> timer_mode == COMPARE_MODE)
		{
			/* Set Timer initial value */

			TCNT0 = Config_Ptr -> timer_InitialValue;

			/* Set Timer compare match value */

			OCR0 = Config_Ptr -> timer_compare_MatchValue;

			/* Enable Timer0 Compare mode Interrupt */

			TIMSK |= (1<<OCIE0);

			/* configure the timer0 */

			TCCR0 = (1<<FOC0) | (1<<WGM01) | (TCCR0 & 0xF8) | (Config_Ptr -> timer_clock & 0x07);
		}
		break;
	case TIMER1:
		if (Config_Ptr -> timer_mode == NORMAL_MODE)
		{
			/* Set Timer initial value */

			TCNT1 = Config_Ptr -> timer_InitialValue;

			/* Enable Timer1 Compare Interrupt */

			TIMSK |= (1<<TOIE1);

			/* configure the timer1 */

			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> timer_clock & 0x07);
		}
		else if (Config_Ptr -> timer_mode == COMPARE_MODE)
		{
			/* Set Timer initial value */

			TCNT1 = Config_Ptr -> timer_InitialValue;

			/* Set Timer compare match value */

			OCR1A = Config_Ptr -> timer_compare_MatchValue;

			/* Enable Timer1 Compare Interrupt */

			TIMSK |= (1<<OCIE1A);

			/* configure the timer1 */

			TCCR1B = (1 << WGM12) | (TCCR1B & 0xF8) | (Config_Ptr -> timer_clock & 0x07);
		}
		break;
	case TIMER2:
		if (Config_Ptr -> timer_mode == NORMAL_MODE)
		{
			/* Set Timer initial value */

			TCNT2 = Config_Ptr -> timer_InitialValue;

			/* Enable Timer2 Overflow Interrupt */

			TIMSK |= (1<<TOIE2);

			/* configure the timer2 */

			TCCR2 = (1<<FOC2) | (TCCR2 & 0xF8) | (Config_Ptr -> timer_clock & 0x07);
		}
		else if (Config_Ptr -> timer_mode == COMPARE_MODE)
		{
			/* Set Timer initial value */

			TCNT2 = Config_Ptr -> timer_InitialValue;

			OCR2 = Config_Ptr -> timer_compare_MatchValue;

			/* Enable Timer0 Compare mode Interrupt */
			TIMSK |= (1<<OCIE2);

			/* configure the timer2 */
			TCCR2 = (1<<FOC2) | (1<<WGM21) | (TCCR2 & 0xF8) | (Config_Ptr -> timer_clock & 0x07);
		}
		break;
	}
}

void Timer_deInit(Timer_ID_Type timer_type)
{
	switch(timer_type)
	{
	case TIMER0:
		/* Clear normal and compare mode bits for timer0*/
		TIMSK &= ~((1 << TOIE0) | (1 << OCIE0));
		/*Clear the clock of the timer0*/
		TCCR0 = 0;
		OCR0 = 0;
		break;
	case TIMER1:
		/* Clear normal and compare mode bits for timer2*/
		TIMSK &= ~((1<<TOIE1) | (1<<OCIE1A));
		/*Clear the clock of the timer2*/
		TCCR1A = 0;
		TCCR1B = 0;
		break;
	case TIMER2:
		/* Clear normal and compare mode bits for timer2*/
		TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
		/*Clear the clock of the timer2*/
		TCCR2 = 0;
		OCR2 = 0;
		break;
	}
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
	/* Save the address of the Call back function in a global variable */
	g_timerCallBackPtr[a_timer_ID] = a_ptr;
}
