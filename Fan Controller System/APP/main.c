/*
 * main.c
 *
 *  Created on: Apr 6, 2023
 *      Author: Mohamed Abdelazez
 */
#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include "dc_motor.h"
#include "lm35_sensor.h"
#include "lcd.h"

ADC_ConfigType ADC_Config = {INTERNAL_REF, PRESCALER_8};

int main(void)
{
	uint8 tempValue = 0;
	ADC_init(&ADC_Config);
	DcMotor_Init();
	LCD_init();

	LCD_displayString("Fan is ");
	LCD_moveCursor(1,0);
	LCD_displayString("Temp =    C");

	while(1)
	{
		tempValue = LM35_getTemperature();

		if(tempValue < 30)
		{
			DcMotor_Rotate(STOP,0);
		}
		else if(tempValue >= 30 && tempValue < 60)
		{
			DcMotor_Rotate(CW_DIRECTION,25);
		}
		else if(tempValue >= 60 && tempValue < 90)
		{
			DcMotor_Rotate(CW_DIRECTION,50);
		}
		else if(tempValue >= 90 && tempValue < 120)
		{
			DcMotor_Rotate(CW_DIRECTION,75);
		}
		else if(tempValue >= 120)
		{
			DcMotor_Rotate(CW_DIRECTION,100);
		}

		/* Display Fan state */
		LCD_moveCursor(0,7);
		if(tempValue < 30)
		{
			LCD_displayString("Off");
		}
		else
		{
			LCD_displayString("On");
			LCD_displayCharacter(' ');
		}

		/* Display Fan Speed */
		LCD_moveCursor(1,7);
		if(tempValue >= 100)
		{
			LCD_intgerToString(tempValue);
		}
		else
		{
			LCD_intgerToString(tempValue);
			LCD_displayCharacter(' ');
		}
	}

	return 0;
}
