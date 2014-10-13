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
#define MOTOR_BACKWARD 3
#define MOTOR_HALT 0
#define MOTOR_ERROR 2

static const char motor_QEM[16] = {
								MOTOR_HALT,
								MOTOR_BACKWARD,
								MOTOR_FORWARD,
								MOTOR_ERROR,
								MOTOR_FORWARD,
								MOTOR_HALT,
								MOTOR_ERROR,
								MOTOR_BACKWARD,
								MOTOR_BACKWARD,
								MOTOR_ERROR,
								MOTOR_HALT,
								MOTOR_FORWARD,
								MOTOR_ERROR,
								MOTOR_FORWARD,
								MOTOR_BACKWARD,
								MOTOR_HALT
							};	// Quadrature Encoder Matrix

typedef struct
{
	void (* const initMotor)(void*, unsigned char, unsigned char, unsigned char);
	void (* const nextState)(void*);
	char  (* const getDirection)(void*);
	unsigned long (* const getTicks)(void*);
	void (* const sendTicks)(void*, serial_struct*);
	unsigned char curr_state;
	unsigned char prev_state;
	unsigned char ChannelA;
	unsigned char ChannelB;
	unsigned char Interupt;
	long encoder_ticks;
} motor_state_machine_struct;

extern motor_state_machine_struct RightMotor;
extern motor_state_machine_struct LeftMotor;

__interrupt void PORT1_ISR(void);

#endif /* MOTORSTATEMACHINE_H_ */
