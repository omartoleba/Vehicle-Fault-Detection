/*
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the DC_MOTOR driver
 *
 * Author: 	Omar Toleba
 */

#include "dc_motor.h"
#include "gpio.h"
#include "pwm.h"

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(DC_MOTOR_IN1_PORT_ID , DC_MOTOR_IN1_PIN_ID , PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_IN2_PORT_ID , DC_MOTOR_IN2_PIN_ID , PIN_OUTPUT);

	GPIO_writePin(DC_MOTOR_IN1_PORT_ID , DC_MOTOR_IN1_PIN_ID ,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_IN2_PORT_ID , DC_MOTOR_IN2_PIN_ID ,LOGIC_LOW);

#if(MOTORS_NUMBER == 2)
	GPIO_setupPinDirection(DC_MOTOR_IN3_PORT_ID , DC_MOTOR_IN3_PIN_ID , PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_IN4_PORT_ID , DC_MOTOR_IN4_PIN_ID , PIN_OUTPUT);

	GPIO_writePin(DC_MOTOR_IN3_PORT_ID , DC_MOTOR_IN3_PIN_ID ,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_IN4_PORT_ID , DC_MOTOR_IN4_PIN_ID ,LOGIC_LOW);
}
#endif
void DcMotor_Rotate(DC_MOTOR_STATE state, uint8 motorNumber , uint8 speed)
{
	switch(state)
	{
	case STOP:
		if (motorNumber == MOTOR1)
		{
			GPIO_writePin(DC_MOTOR_IN1_PORT_ID , DC_MOTOR_IN1_PIN_ID ,LOGIC_LOW);
			GPIO_writePin(DC_MOTOR_IN2_PORT_ID , DC_MOTOR_IN2_PIN_ID ,LOGIC_LOW);
		}
		else if (motorNumber == MOTOR2)
		{
			GPIO_writePin(DC_MOTOR_IN3_PORT_ID , DC_MOTOR_IN3_PIN_ID ,LOGIC_LOW);
			GPIO_writePin(DC_MOTOR_IN4_PORT_ID , DC_MOTOR_IN4_PIN_ID ,LOGIC_LOW);
		}
		break;
	case CW:
		if (motorNumber == MOTOR1)
			{
				GPIO_writePin(DC_MOTOR_IN1_PORT_ID , DC_MOTOR_IN1_PIN_ID ,LOGIC_LOW);
				GPIO_writePin(DC_MOTOR_IN2_PORT_ID , DC_MOTOR_IN2_PIN_ID ,LOGIC_HIGH);
			}
			else if (motorNumber == MOTOR2)
			{
				GPIO_writePin(DC_MOTOR_IN3_PORT_ID , DC_MOTOR_IN3_PIN_ID ,LOGIC_LOW);
				GPIO_writePin(DC_MOTOR_IN4_PORT_ID , DC_MOTOR_IN4_PIN_ID ,LOGIC_HIGH);
			}
		break;
	case A_CW:
		if (motorNumber == MOTOR1)
			{
				GPIO_writePin(DC_MOTOR_IN1_PORT_ID , DC_MOTOR_IN1_PIN_ID ,LOGIC_HIGH);
				GPIO_writePin(DC_MOTOR_IN2_PORT_ID , DC_MOTOR_IN2_PIN_ID ,LOGIC_LOW);
			}
			else if (motorNumber == MOTOR2)
			{
				GPIO_writePin(DC_MOTOR_IN3_PORT_ID , DC_MOTOR_IN3_PIN_ID ,LOGIC_HIGH);
				GPIO_writePin(DC_MOTOR_IN4_PORT_ID , DC_MOTOR_IN4_PIN_ID ,LOGIC_LOW);
			}
		break;
	}
	PWM_Timer0_Start(speed);
}
