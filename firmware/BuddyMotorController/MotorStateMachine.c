/*
 * MotorStateMachine.c
 *
 *  Created on: Oct 8, 2014
 *      Author: Charanpreet
 */

#include "MotorStateMachine.h"

void initMotor(motor_state_machine_struct* self)
{
	P1DIR  &= ~(self->ChannelA + self->ChannelB + self->Interupt);
	P1IE |= self->Interupt;
	P1IES |= self->Interupt;
}

void nextState(motor_state_machine_struct* self)
{
	self->prev_state = self->curr_state;
	unsigned char _chan_A_val = (unsigned char)((P1IN & self->ChannelA) != 0);
	unsigned char _chan_B_val = (unsigned char)((P1IN & self->ChannelB) != 0);
	self->curr_state = (_chan_A_val << 1) + _chan_B_val;
}

char getDirection(motor_state_machine_struct* self)
{
	return motor_QEM[self->prev_state*4 + self->curr_state];
}

unsigned long getTicks(motor_state_machine_struct* self)
{
	return self->encoder_ticks;
}

motor_state_machine_struct const LeftMotor = 	{
													initMotor,
													nextState,
													getDirection,
													getTicks,
													0,
													0,
													0,
												};

motor_state_machine_struct const RightMotor = 	{
													initMotor,
													nextState,
													getDirection,
													getTicks,
													0,
													0,
													0,
												};


#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	motor_state_machine_struct * current_motor = NULL;
	if( P1IFG & LeftMotor.Interupt != 0 )
	{
		current_motor = &LeftMotor;
	}
	else if( P1IFG & Right.Interupt != 0 )
	{
		current_motor = &RightMotor;
	}
	if(current_motor != NULL)
	{
		P1IES ^= current_motor->Interupt;			//Toggle between rising and falling edge
		P1IFG &= ~current_motor->Interupt;

		current_motor->nextState(&current_motor);
		if(current_motor->getDirection() == MOTOR_FORWARD)
		{
			current_motor->encoder_ticks++;
		}
		else if(current_motor->getDirection() == MOTOR_BACKWARD)
		{
			current_motor->encoder_ticks--;
		}
	}
}
