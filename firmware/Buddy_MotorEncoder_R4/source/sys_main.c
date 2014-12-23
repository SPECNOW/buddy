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
//	HalCoGen Libraries
#include "sci.h"
#include "gio.h"
#include "eqep.h"
#include "pinmux.h"

//	User Libraries
#include "sys_core.h"
#include "MotorStateMachine.h"
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "defines.h"
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

	//	Initialize Modules
	muxInit();
	gioInit();
	sciInit();
	QEPInit();
	//eqepDisableAllInterrupts(eqepREG1);
	//eqepEnableCapture(eqepREG1);

	//	Initialize Structures
	//LeftMotor.initMotor(&LeftMotor, 0, 1, 4);

	//RANDOM CRAP FROM EXAMPLE
	//eqepClearAllInterruptFlags (eqepREG1);
	//eqepSetMaxPosnCount (eqepREG1, 0xFFFFFFFF);
	//eqepEnableInterrupt (eqepREG1, QEINT_Pco);
	eqepSetCapturePrescale(eqepREG1, QCAPCTL_Ccps_Capture_Div_32);
	eqepSetUnitPosnPrescale(eqepREG1, QCAPCTL_Upps_Div_1024_Prescale);
	eqepEnableCounter(eqepREG1);

	gioToggleBit(gioPORTA, SW_ENABLE); // ENABLES
	sciReceive((sciBASE_t *)0xFFF7E400U, 1, (unsigned char *)&command[0]);
	while(1)
	{
		//sciSendByte(scilinREG, 'a');*/
	}
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
