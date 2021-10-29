/*
 *	File name: buzzer.h
 *  Created on: Oct 21, 2021
 *  Author: osama
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"


/* Configurations */
#define BUZZER_PORT		PORTC_ID
#define BUZZER_PIN		PIN3_ID


/* Functions prototypes */
void BZR_init(void);

/*
 * Decription: function to clear the buzzer pin
 * */
void BZR_ON(void);

/*
 * Decription: function to set the buzzer pin
 * */
void BZR_OFF(void);

#endif /* BUZZER_H_ */
