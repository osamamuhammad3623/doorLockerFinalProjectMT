/*
 *	File name: HMI_Application.h
 *  Created on: Oct 21, 2021
 *  Author: osama
 */

#ifndef HMI_APPLICATION_H_
#define HMI_APPLICATION_H_

/********************************************************/
/* Definitions & Configurations */
/********************************************************/
#define PASSWORD_LENGTH			(5)
#define KEYPAD_INPUT_DELAY		(3000)
#define DOOR_UNLOCKING_PERIOD	(5)
#define DOOR_LEFT_OPEN_PERIOD	(3)
#define DISPLAY_MESSAGE_DELAY	(5000)
/* following definitions used to communicate with Control ECU */
#define PASSWORD_MATCHED		(1)
#define PASSWORD_MISMATCHED		(0)
#define READY_TO_SEND			(0x15)
#define READY_TO_RECEIVE		(0x16)
#define CHANGE_PASSWORD_OPTION	(0x18)
#define UNLOCKING_DOOR			(0x25)
#define WRONG_PASSWORD			(0x30)
#define CHANGING_PASSWORD		(0X31)


/********************************************************/
/* Global variables */
/********************************************************/
uint8 g_inputPassword[PASSWORD_LENGTH];
uint8 g_password_match_status = 0;
uint16 g_seconds = 0;


/********************************************************/
/* Functions prototypes */
/********************************************************/
/*
 * Description: a function to initialize the password in first-run OR to change the password
 * */
void initializePassword(void);

/*
 * Description: A function to get the password from user and store it in a given array
 * */
void getPassword(uint8 * arrayName);

/*
 * Description: A function to display application options on LCD
 * */
void appMainOptions(void);

/*
 * Description: the call-back function called by the timer every 1 second
 * */
void timerCallBack(void);

/*
 * Description: A function that displays on LCD that door is opening or closing for a certain period of time
 * */
void DoorOpeningTask(void);

/*
 * Description: A function to send the password via UART by looping on sendByte function
 * */
void sendPasswordViaUART(uint8 * passwordArray);

#endif /* HMI_APPLICATION_H_ */
