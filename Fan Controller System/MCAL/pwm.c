/*
 * pwm.c
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */

#include "avr/io.h"
#include "common_macros.h"
#include "gpio.h"
#include "pwm.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{
	compareValue = (duty_cycle*255)/100;

	TCNT0 = 0;
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);
	OCR0  = compareValue;
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

