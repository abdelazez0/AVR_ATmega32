/*
 * uart.c
 *
 *  Created on: Apr 12, 2023
 *      Author: ZYZ
 */
#include "uart.h"
#include "common_macros.h"
#include <avr/io.h>

void UART_init(uint32 baud_rate)
{
	uint16 ubrr_value = 0;

	/* Asynchronous, no parity, 1-bit stop bit */
	SET_BIT(UCSRA,U2X); /* double transmission speed */
	SET_BIT(UCSRB,RXEN); /* receiver enable */
	SET_BIT(UCSRB,TXEN); /* transmitter enable */
	SET_BIT(UCSRC,URSEL); /* USART register select UCSRC */
	SET_BIT(UCSRC,UCSZ1); /* 8-bit character size */
	SET_BIT(UCSRC,UCSZ0); /* 8-bit character size */

	/* baud rate vale */
	ubrr_value = (uint16)((F_CPU / (8UL * baud_rate)) - 1);

	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

void UART_sendByte(const uint8 data)
{
	/* if UDRE is 1 ,the buffer is empty and ready to be written */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){/* loop */}
	UDR = data;
}

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){/* loop */}

	return UDR;
}

void UART_sendString(const uint8 *Str)
{
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
}

void UART_receiveString(uint8 *Str)
{
	/* null is only defined in C, so UART doesn't recognize it*/
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#', the string i receive must end by # */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
