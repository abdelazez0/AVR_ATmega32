/*
 * pwm.h
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */

#ifndef PWM_H_
#define PWM_H_
#include "std_types.h"

#define ENABLE_PIN_PORT_ID			PORTB_ID
#define ENABLE_PIN_BIT_ID			PIN3_ID

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
