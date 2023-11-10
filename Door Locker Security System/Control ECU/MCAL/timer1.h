/**********************************************************************
 * ****** Driver: timer1.h
 *
 * ****** Created on: May 8, 2023
 * ****** Author: Mohamed Abdelazez
 *********************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

/**************************** Global variables ******************************/
static volatile void (*g_callBackPtr)(void) = NULL_PTR;				/* hold the address of the call back function */


/**************************** Enums Declaration *****************************/
typedef enum
{
	NO_CLOCK , F_CPU_CLOCK , F_CPU_8 , F_CPU_64 , F_CPU_256 , F_CPU_1024 , EXTERNAL_CLK_FALLING , EXTERNAL_CLK_RISING

}Timer1_Prescaler;

typedef enum
{
	NORMAL_MODE , COMPARE_MATCH_MODE

}Timer1_Mode;


/**************************** Structure Declaration *****************************/
typedef struct
{
	uint16 				initial_value;
	uint16 				compare_value; 	/* it will be used in compare mode only */
	Timer1_Prescaler 	prescaler;
	Timer1_Mode			mode;

}Timer1_ConfigType;


/**************************** Functions Declaration *****************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
