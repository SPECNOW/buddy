/*
 * MotorStateMachine.h
 *
 *  Created on: Oct 8, 2014
 *      Author: Charanpreet
 */

#ifndef MOTORSTATEMACHINE_H_
#define MOTORSTATEMACHINE_H_

#include "COM.h"

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD -1
const char motor_QEM[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};               // Quadrature Encoder Matrix

typedef struct
{
	void (* const initMotor)(motor_state_machine_struct*);
	void (* const nextState)(motor_state_machine_struct*);
	char  (* const getDirection)(motor_state_machine_struct*);
	unsigned long (* const getTicks)(motor_state_machine_struct*);
	serial_struct * serial;
	unsigned char curr_state;
	unsigned char prev_state;
	unsigned char ChannelA;
	unsigned char ChannelB;
	unsigned char Interupt;
	unsigned long encoder_ticks;
} motor_state_machine_struct;

extern motor_state_machine_struct const LeftMotor;
extern motor_state_machine_struct const RightMotor;

__interrupt void PORT1_ISR(void);

#endif /* MOTORSTATEMACHINE_H_ */
