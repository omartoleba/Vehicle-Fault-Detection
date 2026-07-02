/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the AVR PWM driver
 *
 * Author: Omar Toleba
 *
 *******************************************************************************/

#include <avr/io.h>
#include "pwm.h"
#include "gpio.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = 0; // Set Timer Initial Value to 0

	OCR0  = (uint8)((uint16)(duty_cycle * 255) / 100); //Set Compare value

	GPIO_setupPinDirection(PWM_PORT_ID , PWM_PIN_ID , PIN_OUTPUT); // Configure PB3 as output pin

		/* Configure timer control register
		 * 1. Fast PWM mode FOC0=0
		 * 2. Fast PWM Mode WGM01=1 & WGM00=1
		 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
		 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
		 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00) | (1<<CS02);
}

