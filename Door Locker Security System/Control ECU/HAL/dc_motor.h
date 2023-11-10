/*
 * dc_motor.h
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

#define MOTOR_CW_PORT      PORTA_ID
#define MOTOR_ACW_PORT     PORTA_ID

#define MOTOR_CW_PIN       PIN4_ID
#define MOTOR_ACW_PIN      PIN5_ID

typedef enum
{
	STOP, CW_DIRECTION, ACW_DIRECTION
}DcMotor_State;

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DC_MOTOR_H_ */
