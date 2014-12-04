/** @file notification.c 
*   @brief User Notification Definition File
*   @date 9.Sep.2014
*   @version 04.01.00
*
*   This file  defines  empty  notification  routines to avoid
*   linker errors, Driver expects user to define the notification. 
*   The user needs to either remove this file and use their custom 
*   notification function or place their code sequence in this file 
*   between the provided USER CODE BEGIN and USER CODE END.
*
*/

/* Include Files */

#include "esm.h"
#include "sys_selftest.h"
#include "gio.h"
#include "sci.h"

/* USER CODE BEGIN (0) */
#include "MotorStateMachine.h"
/* USER CODE END */
#pragma WEAK(esmGroup1Notification)
void esmGroup1Notification(uint32 channel)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (1) */
/* USER CODE END */
}

/* USER CODE BEGIN (2) */
/* USER CODE END */
#pragma WEAK(esmGroup2Notification)
void esmGroup2Notification(uint32 channel)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (3) */
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
#pragma WEAK(memoryPort0TestFailNotification)
void memoryPort0TestFailNotification(uint32 groupSelect, uint32 dataSelect, uint32 address, uint32 data)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (5) */
/* USER CODE END */
}

/* USER CODE BEGIN (6) */
/* USER CODE END */
#pragma WEAK(memoryPort1TestFailNotification)
void memoryPort1TestFailNotification(uint32 groupSelect, uint32 dataSelect, uint32 address, uint32 data)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (7) */
/* USER CODE END */
}

/* USER CODE BEGIN (8) */
/* USER CODE END */
#pragma WEAK(gioNotification)
void gioNotification(gioPORT_t *port, uint32 bit)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (19) */
	motor_state_machine_struct * current_motor = 0;
	if(  LeftMotor.Interupt == bit)
	{
		current_motor = &LeftMotor;
	}
	else if( RightMotor.Interupt != bit)
	{
		current_motor = &RightMotor;
	}

	if(current_motor != 0)
	{
		//gioREG->POL ^= current_motor->Interupt;			// Toggle between rising and falling edge

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

	//DEBUG CRAP
	/*sciSendByte(scilinREG, '0' + LeftMotor.curr_state);
	sciSendByte(scilinREG, '0' + LeftMotor.prev_state);

	if(LeftMotor.getDirection(&LeftMotor) == MOTOR_BACKWARD)
	{
		sciSendByte(scilinREG, 'B');
	}
	else if(LeftMotor.getDirection(&LeftMotor) == MOTOR_FORWARD)
	{
		sciSendByte(scilinREG, 'F');
	}
	else if(LeftMotor.getDirection(&LeftMotor) == MOTOR_HALT)
	{
		sciSendByte(scilinREG, 'H');
	}
	else if(LeftMotor.getDirection(&LeftMotor) == MOTOR_ERROR)
	{
		sciSendByte(scilinREG, 'E');
	}
	sciSendByte(scilinREG, '\n');
	sciSendByte(scilinREG, '\r');*/
	//sciSendByte(scilinREG, 'b');
/* USER CODE END */
}

/* USER CODE BEGIN (20) */
/* USER CODE END */

#pragma WEAK(sciNotification)
void sciNotification(sciBASE_t *sci, uint32 flags)     
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (29) */
	sciSendByte(sci, 'c');
	if(LeftMotor.getDirection(&LeftMotor) == MOTOR_BACKWARD)
	{
		sciSendByte(sci, 'B');
	}
	else if(LeftMotor.getDirection(&LeftMotor) == MOTOR_FORWARD)
	{
		sciSendByte(sci, 'F');
	}
	else if(LeftMotor.getDirection(&LeftMotor) == MOTOR_HALT)
	{
		sciSendByte(sci, 'H');
	}
	else if(LeftMotor.getDirection(&LeftMotor) == MOTOR_ERROR)
	{
		sciSendByte(sci, 'E');
	}
	sciSendByte(sci, '\n');
	sciSendByte(sci, '\r');
	/*RightMotor.sendTicks(&RightMotor, sci);
	LeftMotor.sendTicks(&LeftMotor, sci);*/
/* USER CODE END */
}

/* USER CODE BEGIN (30) */
/* USER CODE END */



/* USER CODE BEGIN (43) */
/* USER CODE END */


/* USER CODE BEGIN (47) */
/* USER CODE END */


/* USER CODE BEGIN (50) */
/* USER CODE END */


/* USER CODE BEGIN (53) */
/* USER CODE END */


/* USER CODE BEGIN (56) */
/* USER CODE END */

/* USER CODE BEGIN (58) */
/* USER CODE END */

/* USER CODE BEGIN (60) */
/* USER CODE END */