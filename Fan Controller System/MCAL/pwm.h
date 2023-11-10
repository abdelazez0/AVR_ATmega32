/*
 * pwm.h
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

volatile static uint16 compareValue = 0;

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
