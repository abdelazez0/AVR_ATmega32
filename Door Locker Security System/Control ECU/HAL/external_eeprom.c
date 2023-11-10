 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Mohamed Abdelazez
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	TWI_start();															/* 1- send start bit */
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7)));				/* 2- send memory address part 1, r/w bit = 0 (write) */
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	TWI_writeByte((uint8)(u16addr));										/* 3- send memory address part 2*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	TWI_writeByte(u8data);													/* 4- send data*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	TWI_stop();																/* 5- send stop bit */

	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	TWI_start();															/* 1- send start bit */
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7)));				/* 2- send memory address part 1, r/w bit = 0 (write) */
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	TWI_writeByte((uint8)(u16addr));										/* 3- send memory address part 2*/
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	TWI_start();															/* 4- send start bit again */
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;

	TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7) | 1));			/* 5- send memory address part 1, r/w bit = 1 (read) */
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
			return ERROR;

	*u8data = TWI_readByteWithNACK();										/* 6- read data from the sent location */
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
			return ERROR;

	TWI_stop();																/* 7- send stop bit */

	return SUCCESS;
}
