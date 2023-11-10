/***************************************************************
 * main.c
 *
 *  Created on: May 8, 2023
 *      Author: Mohamed Abdelazez
 ***************************************************************/
#include "std_types.h"
#include "common_macros.h"

#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include "timer1.h"

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@		Definitions		@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
#define CONTROL_MCU_READY     0x10
#define CHECK_SET_PASS_MATCH  0x20
#define CHECK_PASS			  0x30
#define OPEN_DOOR			  0x40
#define MATCH_TRUE			  0x50
#define MATCH_FALSE			  0x60
#define ENABLE_ALARM		  0x70

#define PASSWORD_LENGTH	 	  7
#define UART_BAUDRATE		  9600

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Function Declaration	@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
void getPassword(void);
void setPassword(void);
void openDoor(void);
void TIMER1_tick(void);
void errorMessage (void);

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Global Variables	@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
uint8 passArr[PASSWORD_LENGTH];
uint8 key;
uint8 wrongAttempts = 0;

uint16 tick = 0;
uint8 g_flag15sec = 0;
uint8 g_flag3sec = 0;
uint8 g_flag60sec = 0;

Timer1_ConfigType timerConfig = {0 , 60000 , F_CPU_8 , COMPARE_MATCH_MODE};

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Main	@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
int main(void)
{
	/* Initialization */
	UART_init(UART_BAUDRATE);
	LCD_init();
	while (UART_recieveByte() != CONTROL_MCU_READY){}
	Timer1_setCallBack(TIMER1_tick);
	SET_BIT(SREG , 7);

	setPassword();

	/* Main Loop */
	while (1)
	{
		LCD_clearScreen();
		LCD_displayString("+: Open Door");
		LCD_displayStringRowColumn(1,0,"-: Change Pass");

		key = KEYPAD_getPressedKey();
		_delay_ms(500);

		if ((key == '+') || (key == '-'))
		{
			LCD_clearScreen();
			LCD_displayString("Enter Pass:");
			LCD_moveCursor(1,0);

			UART_sendByte(CHECK_PASS);
			getPassword();
			UART_sendString(passArr);

			if (UART_recieveByte() == MATCH_TRUE)
			{
				switch(key)
				{
					case ('+'):		openDoor();	    wrongAttempts = 0;	break;
					case ('-'):		setPassword();	wrongAttempts = 0;	break;
				}
			}
			else
			{
				LCD_clearScreen();
				LCD_displayString(" Wrong Password ");
				_delay_ms(500);

				wrongAttempts++;
				if (wrongAttempts == 3)
				{
					errorMessage();
				}
			}
		}

	}
	return 0;
}

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Function Definition		@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

void getPassword(void)
{
	uint8 counter = 0;
	uint8 getPassFlag = 0;
	uint8 pressedKey;

	while (getPassFlag == 0)
	{
		pressedKey = KEYPAD_getPressedKey();

		if ((pressedKey <= 9) && (pressedKey >= 0) && (counter < PASSWORD_LENGTH - 2))
		{
			passArr[counter] = '0' + pressedKey;
			counter++;
			LCD_displayCharacter('*');
		}
		else if (pressedKey == '=')
		{
			getPassFlag = 1;
		}
		_delay_ms(500);
	}
	passArr[counter] = '#';
	passArr[counter+1] = '\0';
}

void setPassword(void)
{
	volatile uint8 exitFlag = 0;

	while(exitFlag == 0)
	{
		UART_sendByte(CHECK_SET_PASS_MATCH);

		LCD_clearScreen();
		LCD_displayString("Plz Enter Pass:");
		LCD_moveCursor(1,0);
		getPassword();
		UART_sendString(passArr);

		LCD_clearScreen();
		LCD_displayString("Plz Re-enter the");
		LCD_displayStringRowColumn(1,0,"Same Pass: ");
		getPassword();
		UART_sendString(passArr);

		if(UART_recieveByte() == MATCH_TRUE)
		{
			LCD_clearScreen();
			LCD_displayString("Pass is Set");
			_delay_ms(500);
			exitFlag = 1;
		}
		else
		{
			LCD_clearScreen();
			LCD_displayString("Mismatch!");
			_delay_ms(500);
			exitFlag = 0;
		}
	}
}

void openDoor(void)
{
	UART_sendByte(OPEN_DOOR);
	Timer1_init(&timerConfig);
	LCD_clearScreen();
	LCD_displayString("Door's Unlocking");
	while(g_flag15sec == 0){}
	Timer1_deInit();
	tick = 0;
	g_flag15sec = 0;
	g_flag3sec = 0;

	Timer1_init(&timerConfig);
	LCD_clearScreen();
	LCD_displayString("Door is Open");
	while(g_flag3sec == 0){}
	Timer1_deInit();
	tick = 0;
	g_flag3sec = 0;

	Timer1_init(&timerConfig);
	LCD_clearScreen();
	LCD_displayString("Door's locking");
	while(g_flag15sec == 0){}
	Timer1_deInit();
	tick = 0;
	g_flag15sec = 0;
	g_flag3sec = 0;
}

void errorMessage (void)
{
	UART_sendByte(ENABLE_ALARM);
	Timer1_init(&timerConfig);
	LCD_clearScreen();
	LCD_displayString("      Error     ");
	while(g_flag60sec == 0){}
	Timer1_deInit();
	tick = 0;
	g_flag15sec = 0;
	g_flag3sec = 0;
	g_flag60sec = 0;
}

void TIMER1_tick(void)
{
	tick++;
	if(tick == 50)
	{
		g_flag3sec = 1;
	}
	else if(tick == 250)
	{
		g_flag15sec = 1;
	}
	else if(tick == 1000)
	{
		g_flag60sec = 1;
		tick = 0;
	}
	else
	{
		/* Do Nothing */
	}
}
