/*
 * MotorStateMachine.c
 *
 *  Created on: Oct 8, 2014
 *      Author: Charanpreet
 */

#include "MotorStateMachine.h"

void initMotor(void* Self, unsigned char ChannelA, unsigned char ChannelB, unsigned char Interrupt)
{
	motor_state_machine_struct* self = (motor_state_machine_struct*)Self;
	self->ChannelA = ChannelA;
	self->ChannelB = ChannelB;
	self->Interupt = Interrupt;
	gioREG->INTDET |= 1 << self->Interupt;
	/*P1DIR  &= ~(self->ChannelA + self->ChannelB + self->Interupt);
	P1IE |= self->Interupt;
	P1IES |= self->Interupt;*/
}

void nextState(void* Self)
{
	motor_state_machine_struct* self = (motor_state_machine_struct*)Self;
	self->prev_state = self->curr_state;
	unsigned char _chan_A_val = 0;
	unsigned char _chan_B_val = 0;
	_chan_B_val = (unsigned char)( gioGetBit(gioPORTA, self->ChannelB) != 0);
	_chan_A_val = (unsigned char)( gioGetBit(gioPORTA, self->ChannelA) != 0);
	self->curr_state = (_chan_B_val << 1) + _chan_A_val;
}

char getDirection(void* Self)
{
	motor_state_machine_struct* self = (motor_state_machine_struct*)Self;
	unsigned int offset = self->prev_state*4 + self->curr_state;
	return motor_QEM[offset];
}

unsigned long getTicks(void* Self)
{
	motor_state_machine_struct* self = (motor_state_machine_struct*)Self;
	return self->encoder_ticks;
}

void sendTicks(void* Self, sciBASE_t *sci)
{
	motor_state_machine_struct* self = (motor_state_machine_struct*)Self;
	//Serial->write((char*)&self->encoder_ticks, sizeof(long));
	sciSend(sci, sizeof(long), (uint8*)&self->encoder_ticks);
	self->encoder_ticks = 0;
}

/*#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	motor_state_machine_struct * current_motor = 0;
	if(  (P1IFG & LeftMotor.Interupt) != 0)
	{
		current_motor = &LeftMotor;
	}
	else if( (P1IFG & RightMotor.Interupt) != 0)
	{
		current_motor = &RightMotor;
	}

	if(current_motor != 0)
	{
		P1IES ^= current_motor->Interupt;			// Toggle between rising and falling edge
		P1IFG &= ~current_motor->Interupt;			// Clear the Interrupt Flag

		current_motor->nextState(current_motor);	//	Calculate Next State

		//	Increment tick counter accordingly
		if(current_motor->getDirection(current_motor) == MOTOR_FORWARD)
		{
			current_motor->encoder_ticks++;
		}
		else if(current_motor->getDirection(current_motor) == MOTOR_BACKWARD)
		{
			current_motor->encoder_ticks--;
		}
	}
	else
	{
		P1IFG &= ~(LeftMotor.Interupt + RightMotor.Interupt);			// Clear the Interrupt Flag// Should never enter here
	}
}*/


motor_state_machine_struct LeftMotor = 	{
													initMotor,
													nextState,
													getDirection,
													getTicks,
													sendTicks,
													0,		//	Current State
													0,		//	Previous State
													0,		//	Channel A Pin
													0,		//	Channel B Pin
													0,		// 	Interrupt Pin
													0,		//	Encoder Ticks
												};

motor_state_machine_struct RightMotor = 	{
													initMotor,
													nextState,
													getDirection,
													getTicks,
													sendTicks,
													0,	//	Current State
													0,	//	Previous State
													0,	//	Channel A Pin
													0,	//	Channel B Pin
													0,	// 	Interrupt Pin
													0,	//	Encoder Ticks
												};
