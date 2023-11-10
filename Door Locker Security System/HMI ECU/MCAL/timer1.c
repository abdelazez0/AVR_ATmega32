/*
 * timer1.c
 *
 *  Created on: May 8, 2023
 *      Author: Mohamed Abdelazez
 */

#include "std_types.h"
#include "common_macros.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer1.h"

/****************************** ISR ***************************************/
ISR(TIMER1_COMPA_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();									/* Call the function whose address in this pointer */
	}
}

ISR(TIMER1_OVF_vect)
{
	if (g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();									/* Call the function whose address in this pointer */
	}
}

/*********************** Function Definition ******************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TIMSK  = 0;												/* Disable all Interrupts */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;												/* Clear Timer1 Registers */
	OCR1A  = 0;												/* Clear Compare value */

	if (Config_Ptr -> mode == NORMAL_MODE)
	{
		TCCR1A |= (1 << FOC1A) | (1 << FOC1B);						/* COM1A1 = 0, COM1A0 = 0, COM1B1 = 0, COM1B0 = 0, FOC1A = 1, FOC1B = 1, WGM11 = 0, WGM10 = 0 */
		TCCR1B |= 0;												/* ICNC1 = 0, ICES1 = 0, WGM13 = 0, WGM12 = 0, (CS12 , CS11 , CS10) = PRESCALER */
		TCCR1B  = (TCCR1B & 0xF8) | (Config_Ptr -> prescaler);		/* Set clock prescaler */
		TCNT1   = Config_Ptr -> initial_value;						/* Initialize Timer1 Register */
		TIMSK  |= (1 << TOIE1);										/* Overflow interrupt enable */
	}

	else if (Config_Ptr -> mode == COMPARE_MATCH_MODE)
	{
		DDRD   |= (1 << PD5);										/* Set OC1A as output */
		TCCR1A |= (1 << FOC1A) | (1 << FOC1B);						/* COM1A1 = 0, COM1A0 = 0, COM1B1 = 0, COM1B0 = 0, FOC1A = 1, FOC1B = 1, WGM11 = 0, WGM10 = 0 */
		TCCR1B |= (1 << WGM12);										/* ICNC1 = 0, ICES1 = 0, WGM13 = 0, WGM12 = 0, (CS12 , CS11 , CS10) = PRESCALER */
		TCCR1B  = (TCCR1B & 0xF8) | (Config_Ptr -> prescaler);		/* Set clock prescaler */
		OCR1A   = Config_Ptr -> compare_value;						/* Set the compare value in register OCR1A */
		TCNT1   = Config_Ptr -> initial_value;						/* Initialize Timer1 Register */
		TIMSK  |= (1 << OCIE1A);									/* Output Compare A Match Interrupt Enable */
	}

	else
	{
		/* DO NOTHING */
	}

}

void Timer1_deInit(void)
{
	TIMSK  = 0;												/* Disable all Interrupts */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;												/* Clear Timer1 Registers */
	OCR1A  = 0;												/* Clear Compare value */
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}
