/*
 * COM.c
 *
 *  Created on: Oct 10, 2012
 *      Author: Charanpreet Parmar
 */

#include "COM.h"

char rx_buff[MAX_RX_BUFF_SIZE] = {0};
bool rx_flag = false;
int rx_index = 0;


void serial_initCOM()
{
	BCSCTL1 = CALBC1_16MHZ; // Set DCO to 1MHz
	DCOCTL = CALDCO_16MHZ; // Set DCO to 1MHz

	/* Configure hardware UART */
	P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2; // Use SMCLK
	UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
	UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
	UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
	IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}
void serial_write(char* tx, int length)
{
	unsigned int pointer;
	for(pointer = 0; pointer < length; pointer++)
	{
		UCA0TXBUF = tx[pointer];
		while (!(IFG2 & UCA0TXIFG));
	}
}

bool serial_serialAvailable()
{
	return rx_flag;
}

char serial_read()
{
	char last_rx = 0;
	while(!serial_serialAvailable());
	if(rx_index >= 0)
	{
		last_rx = *(rx_buff + rx_index - 1);
		rx_index--;
	}
	else
	{
		serial_write("ERROR: RX_BUFF_INDEX NEG\n", 25);
		rx_index = 0;
	}
	if( rx_index == 0)
	{
		rx_flag = false;
	}
	return last_rx;	
}

serial_struct const Serial = {
								serial_initCOM,
								serial_serialAvailable,
								serial_write,
								serial_read,
								&rx_buff[0],
								&rx_flag,
								&rx_index,
							};

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	//It looks like Global Static Variables like rx and RXFLAG
	//can be used in 1 .c file only. Otherwise multiple rxs are created
	//all with different addresses for some reason...
	if(rx_index != MAX_RX_BUFF_SIZE -1)
	{
		rx_buff[rx_index]= UCA0RXBUF;
		rx_flag = true;
		rx_index++;
	}
}

