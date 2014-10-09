//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>
#include "COM.h"

__interrupt void PORT1_ISR(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction


	P1DIR  &= ~(BIT5 + BIT6 + BIT7);
	P1IE |= BIT5;
	P1IES |= BIT5;

	Serial.initCOM();

	while(1)
	{
		if(Serial.serialAvailable())
		{
			char rcvd = Serial.read();
			Serial.write(&rcvd, 1);
			P1OUT ^= 0x01;
		}
	}

	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	if((P1IFG & BIT5) != 0)
	{
		P1IES ^= 1;			//Toggle between rising and falling edge
		P1IFG &= ~BIT5;

		//STATE MACHINE HERE



	}

}
