/*
 *	Module: TIMERS
 *	File name: timers.h
 *  Created on: Oct 21, 2021
 *  Author: osama
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/********************************************************/
/* Type definitions */
/********************************************************/

typedef enum{
	Timer0, Timer1, Timer2
}Timer_ID;

typedef enum{
	Normal, CTC
}Timer_mode;

typedef enum{
	NO_Clock , Prescaler_1, Prescaler_8, Prescaler_64, Prescaler_256, Prescaler_1024,
	External_T1_Falling_Edge, External_T1_Rising_Edge
}Timer_Prescaler;

typedef struct{
	Timer_ID timer_ID;
	Timer_mode mode;
	uint16 initialValue;
	uint16 compareValue;
	Timer_Prescaler prescaler;
	void (*callBackPtr)(void);
}Timer_configuration;


/********************************************************/
/* Functions prototypes */
/********************************************************/

/*
 * Description: A function that initialize a timer with dynamic configurations.
 *
 * Restrictions: - for Timer1 CTC mode, it's configured to control channel A.
 * 				 - support normal port operations, disable any timer-related pins.
 * */
void Timer_init(const Timer_configuration* timer_config);

/*
 * Description: A function to disable a specific timer
 *  the function disables: the clock, overflow & CTC interrupts
 *  the function clears: initial value, compare value & the call-back function
 * */
void Timer_deInit(Timer_ID timer_ID);

#endif /* TIMER_H_ */
