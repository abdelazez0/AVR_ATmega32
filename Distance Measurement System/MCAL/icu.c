#include "icu.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"

static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER1_CAPT_vect)
{
	if(g_callBackPtr != NULL_PTR)
		{
			(*g_callBackPtr)();
		}
}

void Icu_init(const Icu_ConfigType * Config_Ptr)
{
	GPIO_setupPinDirection(PORTD_ID, PIN6_ID , PIN_INPUT);           /* Configure ICP1 as input pin */
	SET_BIT(TCCR1A,FOC1A);                                           /* Normal mode */
	SET_BIT(TCCR1A,FOC1B);                                           /* Normal mode */
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> clock);                /* Adjust clock prescaler */
	TCCR1B = (TCCR1B & 0xBF) | ((Config_Ptr -> edge) << 6);          /* Adjust capture event edge */
	TCNT1 = 0;                                                       /* Initialize timer register by zero*/
	ICR1 = 0;                                                        /* Initialize input capture register by zero*/
	SET_BIT(TIMSK,TICIE1);                                           /* Enable input capture interrupt */
}

void Icu_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

void Icu_setEdgeDetectionType(const Icu_EdgeType a_edgeType)
{
	TCCR1B = (TCCR1B & 0xBF) | (a_edgeType << 6);
}

uint16 Icu_getInputCaptureValue(void)
{
	return ICR1;
}

void Icu_clearTimerValue(void)
{
	TCNT1 = 0;
}

void Icu_DeInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;
	CLEAR_BIT(TIMSK,TICIE1);
}
