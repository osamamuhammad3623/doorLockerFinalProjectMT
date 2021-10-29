/*
 *	File name: CTRL_Application.c
 *  Created on: Oct 21, 2021
 *  Author: osama
 */

#include "buzzer.h"
#include "timer.h"
#include "dc_motor.h"
#include "external_eeprom.h"
#include "twi.h"
#include "CTRL_Application.h"
#include <avr/io.h>
#include <avr/delay.h>
#include "uart.h"

int main(void){
	SREG |=(1<<SREG_I); // enable I-BIT

	UART_configType configPtr = {9600,NO_Parity, One_Stop_Bit, Data_8_Bits};
	UART_init(&configPtr);

	/* Timer freq = 8MHz/1024, one clock-cycle time = 128 uSecond
	 so to force the timer to produce an interrupt every 1 second:
	 we set the compare value to be 1/(128u) = 7813 */
	Timer_configuration timer_config = { Timer1, CTC, 0, 7813, Prescaler_1024, timerCallBack };
	Timer_init(&timer_config);

	/* initialize I2C */
	TWI_Configurations config = {0x02, TWI_CONTROL_ECU_ADDRESS};
	TWI_init(&config);

	DcMotor_init();
	BZR_init();

	initializePassword();

	uint8 receivedByte=0;
	while(1){
		if (UART_recieveByte() == READY_TO_SEND){
			receivePasswordViaUART(g_receivedPassword);
			receivedByte = UART_recieveByte();

			if ( receivedByte == '+'){
				if (compare_passwords(g_storedPassword, g_receivedPassword) == PASSWORD_MATCHED){
					UART_sendByte(UNLOCKING_DOOR); /* inform HMI ECU to display that door is unlocking */
					DoorOpeningTask(); /* start opening door process/task */
				}else{
					UART_sendByte(WRONG_PASSWORD);
					/* count number of wrong attempts, and turn on a buzzer of it exceeds the limit */
					g_wrongPasswordCounter++;
					if (g_wrongPasswordCounter == NUMBER_OF_WRONG_PASSWORD_ATTEMPTS){
						BZR_ON();
						g_seconds=0;
						while(g_seconds < ALARM_ON_DELAY); /* turn on alarm for a certain period */
						BZR_OFF();
						g_wrongPasswordCounter=0; /* reset the counter */
					}
				}


			} else if (receivedByte == CHANGE_PASSWORD_OPTION) {
				if (compare_passwords(g_storedPassword, g_receivedPassword) == PASSWORD_MATCHED) {
					UART_sendByte(CHANGING_PASSWORD); /* inform HMI to process changing password */
					initializePassword();
				}else{
					UART_sendByte(WRONG_PASSWORD);
				}
			}
		}
	}
}


/********************************************************/
/* Functions implementations */
/********************************************************/
uint8 compare_passwords(uint8 a_password1[PASSWORD_LENGTH],uint8 a_password2[PASSWORD_LENGTH]) {
	updateStoredPassword();
	uint8 i;
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		if (a_password1[i] != a_password2[i]) {
			return PASSWORD_MISMATCHED;
		}
	}
	return PASSWORD_MATCHED;
}

void DoorOpeningTask(void){
	/* run the DC motor clockwise for 15 seconds */
	g_seconds = 0;
	DcMotor_Rotate(CW);
	while (g_seconds < DOOR_UNLOCKING_PERIOD);

	/* let the door be open for 3 seconds */
	g_seconds = 0;
	DcMotor_Rotate(Stop);
	while (g_seconds < DOOR_LEFT_OPEN_PERIOD);

	/* hold the system for 15 seconds & display to user that door is locking */
	g_seconds = 0;
	DcMotor_Rotate(ACW);
	while (g_seconds < DOOR_UNLOCKING_PERIOD);

	DcMotor_Rotate(Stop);
}

void timerCallBack(void){
	g_seconds++;
}

void initializePassword(void){
	/* do not return from this function till Control ECU sends a PASSWORD_MATCH signal */
	uint8 confirmationPassword[PASSWORD_LENGTH];
	uint8 check=0;
	while(!check){
		while (UART_recieveByte() != READY_TO_SEND); /* wait till HMI gets ready */
		UART_sendByte(READY_TO_RECEIVE); /* inform HMI that Control ECU ready to receive the password */
		receivePasswordViaUART(g_receivedPassword);

		while (UART_recieveByte() != READY_TO_SEND);
		UART_sendByte(READY_TO_RECEIVE); /* inform HMI to send the confirmation password */
		receivePasswordViaUART(confirmationPassword);

		if (compare_passwords(g_receivedPassword, confirmationPassword) == PASSWORD_MATCHED){
			UART_sendByte(READY_TO_SEND);
			UART_sendByte(PASSWORD_MATCHED);
			storePassword();
			check=1;
		}else{
			UART_sendByte(READY_TO_SEND);
			UART_sendByte(PASSWORD_MISMATCHED);
		}
	}
}

void receivePasswordViaUART(uint8 * passwordArray){
	uint8 cnt;
	for (cnt=0;cnt<PASSWORD_LENGTH;cnt++){
		*(passwordArray+cnt) = UART_recieveByte();
		_delay_ms(100);
	}
}

void updateStoredPassword(void){
	uint8 i;
	for (i=0;i<PASSWORD_LENGTH;i++){
		EEPROM_readByte(EEPROM_STORE_ADDREESS+i, g_storedPassword+i);
	}
}

void storePassword(void){
	uint8 i;
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		EEPROM_writeByte(EEPROM_STORE_ADDREESS + i, g_receivedPassword[i]);
		_delay_ms(100);
	}
}
