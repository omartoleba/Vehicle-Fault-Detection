/*
 *
 * Module: ULTRASONIC
 *
 * File Name: ultrasonic.h
 *
 * Description: Header file for the ULTRASONIC driver
 *
 * Author: Omar Toleba
 *
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define ULTRASONIC_SENSOR_TRIGGER_PORT_ID				PORTD_ID
#define ULTRASONIC_SENSOR_TRIGGER_PIN_ID				PIN7_ID

#define SPEED_OF_SOUND_WAVE       						34000

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

extern uint8 g_edgeCount;
extern uint16 g_timeHigh;

void Ultrasonic_init(void);

void Ultrasonic_Trigger(void);

uint16 Ultrasonic_readDistance(void);

/*Calling back function for the ICU*/

void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
