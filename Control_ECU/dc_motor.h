/*
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: Header file for the DC_MOTOR driver
 *
 * Author: 	Omar Toleba
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MOTORS_NUMBER						 2
#define MOTOR1								 1
#define MOTOR2								 2

#define DC_MOTOR_IN1_PORT_ID                 PORTC_ID
#define DC_MOTOR_IN1_PIN_ID                  PIN4_ID

#define DC_MOTOR_IN2_PORT_ID                 PORTC_ID
#define DC_MOTOR_IN2_PIN_ID                  PIN5_ID

#define DC_MOTOR_IN3_PORT_ID                 PORTC_ID
#define DC_MOTOR_IN3_PIN_ID                  PIN6_ID

#define DC_MOTOR_IN4_PORT_ID                 PORTC_ID
#define DC_MOTOR_IN4_PIN_ID                  PIN7_ID
/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	STOP , CW , A_CW
}DC_MOTOR_STATE;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

void DcMotor_Init(void);

void DcMotor_Rotate(DC_MOTOR_STATE state, uint8 motorNumber , uint8 speed);

#endif /* DC_MOTOR_H_ */
