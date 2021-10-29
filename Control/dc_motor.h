/*
 *	Module: DC Motor
 *	File name: dc_motor.h
 *  Created on: Oct 6, 2021
 *  Author: osama
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "gpio.h"
#include "std_types.h"

/* Configurations */
#define INPUT_PORT_ID	PORTC_ID
#define INPUT1_PIN_ID	PIN6_ID
#define INPUT2_PIN_ID	PIN7_ID

typedef enum{
	Stop, CW, ACW
}DcMotor_State;


 /* functions prototypes */
void DcMotor_init();

void DcMotor_Rotate(DcMotor_State state);

#endif /* DC_MOTOR_H_ */
