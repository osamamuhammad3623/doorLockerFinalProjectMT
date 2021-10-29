 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


/* Type definitions */
typedef enum{
	NO_Parity, Reserved, Even_Parity, Odd_Parity
}UART_ParityType;

typedef enum{
	One_Stop_Bit, Two_Stop_Bit
}UART_StopBitType;

typedef enum{
	Data_5_Bits, Data_6_Bits, Data_7_Bits, Data_8_Bits, Data_9_Bits = 7
}UART_DataBitType;

typedef struct{
	uint32 baud_rate;
	UART_ParityType parityConfig;
	UART_StopBitType stopBitConfig;
	UART_DataBitType dataType;
}UART_configType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_configType * configPtr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
