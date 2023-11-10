/*
 * buzzer.h
 *
 *  Created on: May 8, 2023
 *      Author: ZYZ
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/************************* Configurations ***************************/
#define BUZZER_PORT		PORTA_ID
#define BUZZER_PIN 		PIN0_ID

/********************** Functions Declaration ************************/
void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* BUZZER_H_ */
