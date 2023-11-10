/*
 * main.c
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
#include "lcd.h"
#include "gpio.h"

int main(void)
{
	uint16 measuredDistance = 0;

	SET_BIT(SREG,7);					/* Enable Global Interrupts */
	LCD_init();							/* Initialize LCD */
	LCD_displayString("Distance =    cm");
	Ultrasonic_init();					/* Initialize Ultrasonic */

	while(1)
	{
		measuredDistance = Ultrasonic_readDistance();
		LCD_moveCursor(0,11);

		if(measuredDistance >= 100)
		{
			LCD_intgerToString(measuredDistance);
		}
		else
		{
			LCD_intgerToString(measuredDistance);
			LCD_displayCharacter(' ');
		}
	}

	return 0;
}
