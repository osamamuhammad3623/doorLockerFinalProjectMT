/*
 *	File name: dc_motor.c
 *  Created on: Oct 6, 2021
 *  Author: osama
 */

#include "dc_motor.h"

void DcMotor_init(){
	GPIO_setupPinDirection(INPUT_PORT_ID, INPUT1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(INPUT_PORT_ID, INPUT2_PIN_ID, PIN_OUTPUT);

	DcMotor_Rotate(Stop);
}

void DcMotor_Rotate(DcMotor_State state){
	switch(state){
	case Stop:
		GPIO_writePin(INPUT_PORT_ID, INPUT1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(INPUT_PORT_ID, INPUT2_PIN_ID, LOGIC_LOW);
		break;
	case ACW:
		GPIO_writePin(INPUT_PORT_ID, INPUT1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(INPUT_PORT_ID, INPUT2_PIN_ID, LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(INPUT_PORT_ID, INPUT1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(INPUT_PORT_ID, INPUT2_PIN_ID, LOGIC_HIGH);
		break;
	}

}
