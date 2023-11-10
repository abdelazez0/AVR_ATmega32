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
#include <string.h>

#include "gpio.h"
#include "uart.h"
#include "twi.h"
#include "timer1.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "pwm.h"
#include "dc_motor.h"

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@		Definitions		@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
#define CONTROL_MCU_READY     0x10
#define CONTROL_MCU_READY     0x10
#define CHECK_SET_PASS_MATCH  0x20
#define CHECK_PASS			  0x30
#define OPEN_DOOR			  0x40
#define MATCH_TRUE			  0x50
#define MATCH_FALSE			  0x60
#define ENABLE_ALARM		  0x70

#define EEPROM_BASE_ADDRESS   0x0311
#define PASSWORD_LENGTH 6
#define UART_BAUDRATE 9600

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Function Declaration	@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
void TIMER1_tick(void);
void checkMatch_savePassword(void);
void EEPROM_checkPassword(void);
void openDoor(void);
void buzzerAlarm(void);

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Global Variables	@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
uint16 tick = 0;
uint8 g_flag15sec = 0;
uint8 g_flag3sec = 0;
uint8 g_flag60sec = 0;

uint8 recievedByte;
Timer1_ConfigType timerConfig = {0 , 60000 , F_CPU_8 , COMPARE_MATCH_MODE};

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Main	@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
int main(void)
{
	/* Initialization */

	UART_init(UART_BAUDRATE);
	TWI_init();
	Buzzer_init();
	DcMotor_Init();
	UART_sendByte(CONTROL_MCU_READY);
	Timer1_setCallBack(TIMER1_tick);
	SET_BIT(SREG , 7);

	/* Main Loop */
	while(1)
	{
		recievedByte = UART_recieveByte();

		switch(recievedByte)
		{
			case (CHECK_SET_PASS_MATCH):		checkMatch_savePassword();		break;
			case (CHECK_PASS):					EEPROM_checkPassword();			break;
			case (OPEN_DOOR):					openDoor();						break;
			case (ENABLE_ALARM):				buzzerAlarm();					break;
			default:															break;
		}

	}
	return 0;
}

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@	Function Definition		@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

void checkMatch_savePassword(void)
{
	uint8 passFirst[PASSWORD_LENGTH];
	uint8 passSecond[PASSWORD_LENGTH];
	UART_receiveString(passFirst);
	UART_receiveString(passSecond);

	if(strcmp(passFirst , passSecond) == 0)
	{
		UART_sendByte(MATCH_TRUE);
		for(int i = 0 ; i < PASSWORD_LENGTH ; i++)
		{
			EEPROM_writeByte(EEPROM_BASE_ADDRESS + i, passSecond[i]);
			_delay_ms(10);
		}
	}
	else
	{
		UART_sendByte(MATCH_FALSE);
	}
}

void EEPROM_checkPassword(void)
{
	uint8 byteRead = 0;
	uint8 passToCheck[PASSWORD_LENGTH];
	UART_receiveString(passToCheck);

	for(int i = 0 ; i < PASSWORD_LENGTH ; i++)
	{
		EEPROM_readByte(EEPROM_BASE_ADDRESS + i, &byteRead);
		_delay_ms(10);

		if(passToCheck[i] != byteRead)
		{
			UART_sendByte(MATCH_FALSE);
			return;
		}
	}
	UART_sendByte(MATCH_TRUE);
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

void openDoor(void)
{
	Timer1_init(&timerConfig);
	DcMotor_Rotate(CW_DIRECTION , 100);
	while(g_flag15sec == 0){}
	DcMotor_Rotate(STOP , 0);
	Timer1_deInit();
	tick = 0;
	g_flag15sec = 0;
	g_flag3sec = 0;

	Timer1_init(&timerConfig);
	while(g_flag3sec == 0){}
	Timer1_deInit();
	tick = 0;
	g_flag3sec = 0;

	Timer1_init(&timerConfig);
	DcMotor_Rotate(ACW_DIRECTION , 100);
	while(g_flag15sec == 0){}
	DcMotor_Rotate(STOP , 0);
	Timer1_deInit();
	tick = 0;
	g_flag15sec = 0;
	g_flag3sec = 0;
}

void buzzerAlarm(void)
{
	Timer1_init(&timerConfig);
	Buzzer_on();
	while(g_flag60sec == 0){}
	Buzzer_off();
	Timer1_deInit();
	tick = 0;
	g_flag15sec = 0;
	g_flag3sec = 0;
	g_flag60sec = 0;
}
