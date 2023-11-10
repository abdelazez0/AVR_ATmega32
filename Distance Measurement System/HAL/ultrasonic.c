/*
 * ultrasonic.c
 *
 *  Created on: Apr 26, 2023
 *      Author: Mohamed Abdelazez
 */

#include "icu.h"
#include "ultrasonic.h"
#include "std_types.h"
#include "common_macros.h"
#include <avr/io.h>
#include <util/delay.h>

Icu_ConfigType Icu_Config = {F_CPU_8 , RISING};

static volatile uint8  funcCallCounter = 0 ;
static volatile uint16 captureValue = 0;

void Ultrasonic_init(void)
{
	Icu_init(&Icu_Config);								/* initializing ICU with the required configurations */
	Icu_setCallBack(Ultrasonic_edgeProcessing);			/* Set  the call back function */

	SET_BIT(DDRB,PB5);									/* Set Trigger pin as output pin */
	CLEAR_BIT(PORTB,PB5);								/* Initialize Trigger to Falling Edge */
}

void Ultrasonic_Trigger(void)
{
	SET_BIT(PORTB,PB5);									/* Trigger Rising Edge */
	_delay_us(10);
	CLEAR_BIT(PORTB,PB5);								/* Trigger Falling Edge */
}

uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();

	return (captureValue/58);
}

void Ultrasonic_edgeProcessing(void)
{
	funcCallCounter++;

	if (funcCallCounter == 1)
	{
		Icu_clearTimerValue();
		Icu_setEdgeDetectionType(FALLING);
	}
	else if (funcCallCounter == 2)
	{
		captureValue = Icu_getInputCaptureValue();
		Icu_setEdgeDetectionType(RISING);
		funcCallCounter = 0;
	}
}
