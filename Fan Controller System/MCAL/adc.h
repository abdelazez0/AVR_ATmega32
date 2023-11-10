 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.h
 *
 * Description: header file for the ATmega32 ADC driver
 *
 * Author: Mohamed Tarek , Modified by Mohamed Abdelazez
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	PRESCALER_1, PRESCALER_2, PRESCALER_4, PRESCALER_8, PRESCALER_16, PRESCALER_32, PRESCALER_64, PRESCALER_128
}ADC_Prescaler;

typedef enum
{
	AREF, ACVV, INTERNAL_REF = 3
}ADC_ReferenceVolatge;

typedef struct {
	ADC_ReferenceVolatge ref_volt;
	ADC_Prescaler prescaler;
}ADC_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(const ADC_ConfigType* Config_Ptr);

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
uint16 ADC_readChannel(uint8 channel_num);

/*
 * Description :
 * Function responsible for disabling the ADC driver.
 */
void ADC_disable(void);

#endif /* ADC_H_ */
