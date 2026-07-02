/*
 *
 * Module: ULTRASONIC
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ULTRASONIC driver
 *
 * Author: Omar Toleba
 *
 */

#include "ultrasonic.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

void Ultrasonic_init(void)
{
	ICU_ConfigType ICU_Configurations = {F_CPU_8,RAISING};

	ICU_init(&ICU_Configurations);

	ICU_setCallBack(Ultrasonic_edgeProcessing);

	GPIO_setupPinDirection(ULTRASONIC_SENSOR_TRIGGER_PORT_ID , ULTRASONIC_SENSOR_TRIGGER_PIN_ID , PIN_OUTPUT);
	GPIO_writePin(ULTRASONIC_SENSOR_TRIGGER_PORT_ID , ULTRASONIC_SENSOR_TRIGGER_PIN_ID , LOGIC_LOW);
}

/*Send the trigger pulse to the ultrasonic sensor*/

void Ultrasonic_Trigger(void)
{
	GPIO_writePin(ULTRASONIC_SENSOR_TRIGGER_PORT_ID , ULTRASONIC_SENSOR_TRIGGER_PIN_ID , LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(ULTRASONIC_SENSOR_TRIGGER_PORT_ID , ULTRASONIC_SENSOR_TRIGGER_PIN_ID , LOGIC_LOW);
}

uint16 Ultrasonic_readDistance(void)
{
	uint16 Distance_cm = 0;
	Ultrasonic_Trigger();
	Distance_cm = ((SPEED_OF_SOUND_WAVE / 2.0) * g_timeHigh * (8.0 / F_CPU));
	return Distance_cm;
}

/*
 * Description:
 * Detect the first rising edge then start the timer to calculate the time
 * to the next falling edge
 *
 */

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		ICU_clearTimerValue();
		/* Detect falling edge */
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = ICU_getInputCaptureValue();
		/* Detect rising edge */
		ICU_setEdgeDetectionType(RAISING);
		/*clear the count to start again*/
		g_edgeCount = 0;
	}
}
