/*
 * COM.h
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar
 */

#ifndef COM_H_
#define COM_H_

#include <msp430g2553.h>
#include <stdbool.h>

#define MAX_RX_BUFF_SIZE 64


typedef struct
{
	void (* const initCOM)();
	bool (* const serialAvailable)();
	void (* const write)(char*, int);
	char (* const read)();
	char *_rx_buff;
	bool *_rx_flag;
	int *_rx_index;
} serial_struct;

extern serial_struct const Serial;

__interrupt void USCI0RX_ISR(void);

#endif /* COM_H_ */
