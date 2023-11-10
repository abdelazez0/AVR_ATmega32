/*
 * dc_motor.c
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */
#include "dc_motor.h"
#include "avr/io.h"
#include "common_macros.h"
#include "gpio.h"
#include "pwm.h"

void DcMotor_Init(void)
{
	GPIO_setupPinDirection(MOTOR_CW_PORT, MOTOR_CW_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_ACW_PORT, MOTOR_ACW_PIN, PIN_OUTPUT);

	GPIO_writePin(MOTOR_CW_PORT, MOTOR_CW_PIN, LOGIC_LOW);
	GPIO_writePin(MOTOR_ACW_PORT, MOTOR_ACW_PIN, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	switch(state)
	{
		case CW_DIRECTION:
			GPIO_writePin(MOTOR_CW_PORT, MOTOR_CW_PIN, LOGIC_HIGH);
			GPIO_writePin(MOTOR_ACW_PORT, MOTOR_ACW_PIN, LOGIC_LOW);
			break;

		case ACW_DIRECTION:
			GPIO_writePin(MOTOR_CW_PORT, MOTOR_CW_PIN, LOGIC_LOW);
			GPIO_writePin(MOTOR_ACW_PORT, MOTOR_ACW_PIN, LOGIC_HIGH);
			break;

		case STOP:
			GPIO_writePin(MOTOR_CW_PORT, MOTOR_CW_PIN, LOGIC_LOW);
			GPIO_writePin(MOTOR_ACW_PORT, MOTOR_ACW_PIN, LOGIC_LOW);
			break;
	}

	PWM_Timer0_Start(speed);
}
