/** @file sys_main.c 
*   @brief Application main file
*   @date 9.Sep.2014
*   @version 04.01.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* (c) Texas Instruments 2009-2014, All rights reserved. */

/* USER CODE BEGIN (0) */
#include "sci.h"
#include "gio.h"
#include "MotorStateMachine.h"
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
static unsigned char command;
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */
	_enable_IRQ();
	gioInit();
	sciInit();
	LeftMotor.initMotor(&LeftMotor, 0, 1, 4);

	while(1)
	{
		sciReceive(scilinREG, 1, (unsigned char *)&command);
		//sciSendByte(scilinREG, 'a');*/
	}
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
