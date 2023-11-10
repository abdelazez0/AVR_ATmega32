/*
 * ultrasonic.h
 *
 *  Created on: Apr 26, 2023
 *      Author: Mohamed Abdelazez
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"
#include "icu.h"
#include "common_macros.h"
#include <avr/io.h>




void Ultrasonic_init(void);
void Ultrasonic_Trigger(void);
uint16 Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void);



#endif /* ULTRASONIC_H_ */
