/*
 *	File name: timer.c
 *  Created on: Oct 21, 2021
 *  Author: osama
 */

#include "timer.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>


/********************************************************/
/* Global variables */
/********************************************************/
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_Timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2CallBackPtr)(void) = NULL_PTR;


/********************************************************/
/* ISRs */
/********************************************************/

/* Timer0 overflow*/
ISR(TIMER0_OVF_vect) {
	if (g_Timer0CallBackPtr != NULL_PTR) {
		(*g_Timer0CallBackPtr)();
	}
}

/* Timer0 compare */
ISR(TIMER0_COMP_vect) {
	if (g_Timer0CallBackPtr != NULL_PTR) {
		(*g_Timer0CallBackPtr)();
	}
}

/* Timer1 overflow*/
ISR(TIMER1_OVF_vect) {
	if (g_Timer1CallBackPtr != NULL_PTR) {
		(*g_Timer1CallBackPtr)();
	}
}

/* Timer1 compare */
ISR(TIMER1_COMPA_vect) {
	if (g_Timer1CallBackPtr != NULL_PTR) {
		(*g_Timer1CallBackPtr)();
	}
}

/* Timer2 overflow*/
ISR(TIMER2_OVF_vect) {
	if (g_Timer2CallBackPtr != NULL_PTR) {
		(*g_Timer2CallBackPtr)();
	}
}

/* Timer2 compare */
ISR(TIMER2_COMP_vect) {
	if (g_Timer2CallBackPtr != NULL_PTR) {
		(*g_Timer2CallBackPtr)();
	}
}


/********************************************************/
/* Functions Implementation */
/********************************************************/

void Timer_init(const Timer_configuration *timer_config){
	if (timer_config->timer_ID == Timer0){
		/* starting with adjusting common bits for the timer, then mode-specific bits */

		SET_BIT(TCCR0,FOC0); /* for non-PWM */
		TCNT0 = timer_config->initialValue;
		TCCR0 = 0;
		TCCR0 |= (timer_config->prescaler);

		if (timer_config->mode == Normal){
			SET_BIT(TIMSK,TOIE0); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV0); /* enable overflow flag */

		}else if (timer_config->mode == CTC){
			SET_BIT(TCCR0,WGM01);
			OCR0 = timer_config->compareValue;
			SET_BIT(TIMSK,OCIE0); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF0); /* enable CTC flag */
		}

		g_Timer0CallBackPtr = timer_config->callBackPtr; /* set call back function */


	}else if (timer_config->timer_ID == Timer1){
		TCCR1A =0;
		SET_BIT(TCCR1A,FOC1A); /* for non-PWM mode */
		SET_BIT(TCCR1A,FOC1B);
		TCCR1B = timer_config->prescaler;
		TCNT1 = timer_config->initialValue;

		if (timer_config->mode == Normal) {
			SET_BIT(TIMSK, TOIE1); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV1);

		} else if (timer_config->mode == CTC) {
			SET_BIT(TCCR1B, WGM12);
			OCR1A = timer_config->compareValue;
			SET_BIT(TIMSK, OCIE1A); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF1A); /* enable CTC flag */
		}
		g_Timer1CallBackPtr = timer_config->callBackPtr;


	}else if (timer_config->timer_ID == Timer2){
		SET_BIT(TCCR2, FOC2); /* for non-PWM */
		TCNT2 = timer_config->initialValue;
		TCCR2 = 0;
		TCCR2 |= (timer_config->prescaler);

		if (timer_config->mode == Normal) {
			SET_BIT(TIMSK, TOIE2); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV2); /* enable overflow flag */

		} else if (timer_config->mode == CTC) {
			SET_BIT(TCCR0, WGM21);
			OCR2 = timer_config->compareValue;
			SET_BIT(TIMSK, OCIE2); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF2); /* enable CTC flag */
		}
		g_Timer2CallBackPtr = timer_config->callBackPtr;
	}
}


void Timer_deInit(Timer_ID timer_ID){
	if (timer_ID == Timer0){
		TCCR0 = 0;
		TCNT0= 0; /* clear initial value */
		OCR0 = 0; /* clear compare value for CTC mode*/
		CLEAR_BIT(TIMSK,OCIE0); /* disable interrupts for CTC mode */
		CLEAR_BIT(TIMSK,TOIE0); /* disable interrupts for overflow mode */
		g_Timer0CallBackPtr = NULL_PTR; /* clear the call-back function */

	}else if (timer_ID == Timer1){
		TCCR1A =0;
		TCCR1B = 0; /* disable clock */
		TCNT1 = 0;
		CLEAR_BIT(TIMSK, TOIE1); /* disable interrupts for overflow mode */
		OCR1A = 0;  /* clear compare value for CTC mode */
		CLEAR_BIT(TIMSK, OCIE1A); /* disable interrupts for CTC mode */
		g_Timer1CallBackPtr = NULL_PTR;

	}else if (timer_ID == Timer2){
		TCCR2 = 0;
		TCNT2 = 0; /* clear initial value */
		OCR2 = 0; /* clear compare value for CTC mode*/
		CLEAR_BIT(TIMSK, OCIE2); /* disable interrupts for CTC mode */
		CLEAR_BIT(TIMSK, TOIE2); /* disable interrupts for overflow mode */
		g_Timer2CallBackPtr = NULL_PTR;
	}
}
