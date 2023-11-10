 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Abdelazez
 *
 *******************************************************************************/
 
#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(void)
{
	TWBR = 0x02; 			/* BIT RATE REGISTER VALUE */
	TWSR = 0x00; 			/* ZERO PRESCALER */
	TWAR = 0b00000010;      /* DEVICE ADDRESS 0000001 AND GENERAL RECOGNITION OFF */
	TWCR = (1<<TWEN);		/* ENABLE TWI OPERATION */
}

void TWI_start(void)
{
	/* CLEAR INTERRUPT FLAG */
	/* SEND START BIT */
	/* ENABLE TWI OPERATION */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
	/* CLEAR INTERRUPT FLAG */
	/* SEND STOP BIT */
	/* ENABLE TWI OPERATION */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data)
{
	TWDR = data;
	/* CLEAR INTERRUPT FLAG */
	/* ENABLE TWI OPERATION */
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void)
{
	/* CLEAR INTERRUPT FLAG */
	/* SEND ACK BIT */
	/* ENABLE TWI OPERATION */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while(BIT_IS_CLEAR(TWCR,TWINT));

	return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/* CLEAR INTERRUPT FLAG */
	/* ENABLE TWI OPERATION */
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(BIT_IS_CLEAR(TWCR,TWINT));

	return TWDR;
}

uint8 TWI_getStatus(void)
{
	return (TWSR & 0xF8);
}
