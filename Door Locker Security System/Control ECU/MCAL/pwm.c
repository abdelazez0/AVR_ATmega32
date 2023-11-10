/*
 * pwm.c
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */

#include <avr/io.h>
#include "pwm.h"
#include "gpio.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{
	/*Set the motor pins to be output*/
	GPIO_setupPinDirection(ENABLE_PIN_PORT_ID,ENABLE_PIN_BIT_ID , PIN_OUTPUT);

	TCCR0 = (1<<WGM01) | (1<<WGM00) | (1<<CS01) | (1<<COM01);

	TCNT0 = 0;

	OCR0  = ((float)duty_cycle/100) * 255;

}

