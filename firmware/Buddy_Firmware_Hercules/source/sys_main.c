/** @file sys_main.c 
*   @brief Application main file
*   @date 16.Feb.2015
*   @version 04.03.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2015 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
//	HalCoGen Libraries
#include "sci.h"
#include "gio.h"
#include "eqep.h"
#include "pinmux.h"
#include "adc.h"
#include "het.h"
#include "sys_core.h"
#include "rti.h"

//	User Libraries
/*#include "MotorStateMachine.h"*/	//	We use eQEP now
#include "dac_buddy.h"
#include "sonar.h"
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
	adc_data = calloc(NUM_ADC_SENSORS, sizeof(adcData_t));

	//	Initialize Modules
	muxInit();
	gioInit();
	sciInit();
	QEPInit();
	adcInit();
	rtiInit();
	hetInit();

	_enable_IRQ();

	initDAC(&leftDAC, 4, 6, 7, 0);	// CLK, DATA, LOAD, DACNUM
	initDAC(&rightDAC, 4, 6, 7, 1);	// CLK, DATA, LOAD, DACNUM

	// PreScalers for eQEP
	eqepSetCapturePrescale(eqepREG1, QCAPCTL_Ccps_Capture_Div_32);
	eqepSetUnitPosnPrescale(eqepREG1, QCAPCTL_Upps_Div_64_Prescale);
	eqepEnableUnitTimer(eqepREG1);	/* Enable Unit Timer. */
	eqepEnableCounter(eqepREG1);	/* Enable Position Counter */
	eqepEnableCapture(eqepREG1);	/* Enable capture timer and capture period latch. */

	//gioToggleBit( gioPORTA, SW_ENABLE); // ENABLES // not needed? <- you ARE RIGHT
	gioSetBit( gioPORTA, SW_SELECT, 0);

	adcStartConversion(adcREG1,adcGROUP1);
	adcEnableNotification(adcREG1, adcGROUP1);

	sonar_sensor sonar0 = {
			&HCSR04,						//	Sonar Module for this sensor
			rtiNOTIFICATION_COMPARE0,	//	RTI Compare Notification
			3,							//	GIO Port A Pin used for PWM Trigger
			0,							//	nHET CAP Pin used for PWM Edge
			Sonar_Disabled,				//	Initialize PWM
			0,							//	Initialize Timeout Timer
			0,							//	Initialize Distance
			false						//	Intialize Timeout Flag
	};
	sonar_sensor sonar1 = {
			&HCSR04,					//	Sonar Module for this sensor
			rtiNOTIFICATION_COMPARE1,	//	RTI Compare Notification
			5,							//	GIO Port A Pin used for PWM Trigger
			1,							//	nHET CAP Pin used for PWM Edge
			Sonar_Disabled,				//	Initialize PWM
			0,							//	Initialize Timeout Timer
			0,							//	Initialize Distance
			false						//	Intialize Timeout Flag
	};
	//edgeDisableNotification(hetREG1, 0);
	//edgeDisableNotification(hetREG1, 1);
	rtiStartCounter(rtiCOUNTER_BLOCK0);
	addSonarSensor(&sonar0);
	rtiEnableNotification(getSonarSensor(0)->rti_compare);
	addSonarSensor(&sonar1);
	rtiEnableNotification(getSonarSensor(1)->rti_compare);
	startFirstTrigger(1);

	sciReceive( scilinREG, 2, (unsigned char *)&command[0]);	// Start Serial RX in interrupt mode, wait for 2 bytes for message
	while(1)
	{
		/* Status flag is set to indicate that a new value is latched in the QCPRD register. */
		if((eqepREG1->QEPSTS & 0x80U) !=0U)
		{
			///* Elapsed time between unit position events */
			//deltaT = eqepREG1->QCPRD;
			//
			///* Calculate Velocity from deltaT and the value of the unit position. */
			//current_speed = (float)(current_postion-last_postion)/(float)deltaT;
			//
			///* Clear the Status flag. */
			//eqepREG1->QEPSTS |= 0x80U;
			if( motorPeriods.current_motor == LEFT_MOTOR )
			{
				motorPeriods.left_motor_period = eqepREG1->QPOSCNT;
				copySerialData(motorPeriods.left_motor_period, encoderLeft);
			}
			else
			{
				motorPeriods.right_motor_period = eqepREG1->QPOSCNT;
				copySerialData(motorPeriods.right_motor_period, encoderRight);
			}
		}
		if(adc_data_is_ready)
		{
			addADCSample();
			copySerialData(irArray.average, infraredArray);
			adc_data_is_ready = false;
		}
		if(print_debug_ADC)
		{
			unsigned int i;
			for(i = 0; i < NUM_ADC_SENSORS; i++)
			{
				print_debug("ADC Module", "ID: %d Value: %X", i, irArray.average[i]);
			}
			print_debug_ADC = false;
		}
		if(print_status_flag)
		{
			// print_info("Velocity", "%f", current_speed);
			print_info("Position Count", "%d", eqepREG1->QPOSCNT);
			//print_info("Speed Period", "%d", eqepREG1->QCPRD);
			//print_info("eQEP Status Register HEX", "%02x", eqepREG1->QEPSTS);
			sciSendByte(scilinREG, '\n');
			sciSendByte(scilinREG, '\r');
			print_status_flag = false;
		}
		if(print_command_error_flag)
		{
			print_error("Serial Receive", "INCORRECT COMMAND");
			print_command_error_flag = false;
		}
		if(set_encoder_switch_flag)
		{
			//gioSetBit(gioPORTA, SW_SELECT, switch_position);
			print_info("Switch Toggle", "Toggled");
			gioToggleBit(gioPORTA, SW_SELECT);
			set_encoder_switch_flag = false;
		}
		if(set_left_motor_speed_flag)
		{
			sendDAC(&leftDAC, command[1]);
			print_debug("LeftMotorDAC", "Left Motor command received %d", command[1]);
			set_left_motor_speed_flag = false;
		}
		if(set_right_motor_speed_flag)
		{
			sendDAC(&rightDAC, command[1]);
			print_debug("RightMotorDAC", "Right Motor command received %d", command[1]);
			set_right_motor_speed_flag = false;
		}
		if(is_conversion_complete)
		{
			print_info("Sonar", "Sonar 0: %f, Sonar 1: %f", Sonar_Array.sonarSampler.average[0], Sonar_Array.sonarSampler.average[1]);
		}
		if(send_serial_packet)
		{
			_disable_interrupt_();
			send_serial_packet = false;
			memcpy(&serialPacketRead, &serialPacketWrite, sizeof(SerialPacket));
			_enable_interrupt_();
			sciSend(scilinREG, sizeof(SerialPacket)/sizeof(uint8), (uint8*)&serialPacketRead);
		}
		if(print_debug_sonar)
		{
			print_debug_sonar = false;
			print_info("Sonar", "Sonar 0: %f, Sonar 1: %f", getSonarSensor(0)->_last_distance, getSonarSensor(1)->_last_distance);
			gioSetBit(gioPORTA, getSonarSensor(0)->trig_pwmpin,1);
		}

		if(false)
		{
			static SerialPacket testPacket = {0xFF};
			testPacket.encoderLeft = 123.0;
			testPacket.encoderRight = 4131231.0;
			testPacket.ultrasonicBack = 128;
			testPacket.ultrasonicFront = sizeof(SerialPacket);
			testPacket.validData = 0xee;
			testPacket.infraredArray[0] = 10;
			testPacket.infraredArray[1] = 20;
			testPacket.infraredArray[2] = 30;
			testPacket.infraredArray[3] = 40;
			testPacket.infraredArray[4] = 50;
			testPacket.infraredArray[5] = 60;
			sciSend(scilinREG, sizeof(SerialPacket)/sizeof(uint8)-2, (uint8*)&testPacket);
		}
	}
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
