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
#include "MotorStateMachine.h"

#define MLCHA BIT0
#define MLCHB BIT3
#define MLCHI BIT4

#define MRCHA BIT5
#define MRCHB BIT6
#define MRCHI BIT7

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	//LeftMotor.initMotor(&LeftMotor, MLCHA, MLCHB, MLCHI);
	P1DIR |= BIT0;
	RightMotor.initMotor(&RightMotor, MRCHA, MRCHB, MRCHI);
	Serial.initCOM();

	__bis_SR_register(GIE); // Interrupts enabled

	while(1)
	{
		if(Serial.serialAvailable())
		{
			Serial.read();
			switch(RightMotor.getDirection(&RightMotor))
			{
				case MOTOR_FORWARD:
				{
					Serial.write("f", 1);
					break;
				}
				case MOTOR_BACKWARD:
				{
					Serial.write("b", 1);
					break;
				}
				case MOTOR_HALT:
				{
					Serial.write("h", 1);
					break;
				}
				case MOTOR_ERROR:
				{
					Serial.write("e", 1);
					break;
				}
				default:
				{
					Serial.write("d", 1);
					break;
				}
			}
			Serial.write("\n\r", 2);
		}
		else
		{
			static int index;
			index++;
		}
	}

	return 0;
}
