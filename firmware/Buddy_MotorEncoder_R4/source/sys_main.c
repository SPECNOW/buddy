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
#include "adc.h"
#include "het.h"
#include "sys_core.h"

//	User Libraries
#include "MotorStateMachine.h"
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
//adcData_t adc_data[2];

void delay(int del)
{
	int i = del;
	unsigned int j = 0xFFFF;
	while(i != 0)
	{
		for(j = 0xFFFF; j !=0; j--)
		{

		}
		i--;
	}
}

/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */
	_enable_IRQ();
	adc_data = calloc(6, sizeof(adcData_t));

	//	Initialize Modules
	muxInit();
	gioInit();
	sciInit();
	QEPInit();
	adcInit();
	hetInit();

	initDAC(&leftDAC, 4, 6, 7, 0);	// CLK, DATA, LOAD, DACNUM
	initDAC(&rightDAC, 4, 6, 7, 1);	// CLK, DATA, LOAD, DACNUM

	// PreScalers for eQEP
	eqepSetCapturePrescale(eqepREG1, QCAPCTL_Ccps_Capture_Div_32);
	eqepSetUnitPosnPrescale(eqepREG1, QCAPCTL_Upps_Div_64_Prescale);

	eqepEnableUnitTimer(eqepREG1);	/* Enable Unit Timer. */
	eqepEnableCounter(eqepREG1);	/* Enable Position Counter */
	eqepEnableCapture(eqepREG1);	/* Enable capture timer and capture period latch. */

	gioToggleBit( gioPORTA, SW_ENABLE); // ENABLES // not needed?
	gioSetDirection(hetPORT1, 0x100);	// COpied from ADC example, sets Het8
	adcStartConversion(adcREG1,adcGROUP1);
	adcEnableNotification(adcREG1, adcGROUP1);

	sciReceive( scilinREG, 2, (unsigned char *)&command[0]);	// Start Serial RX in interrupt mode, wait for 2 bytes for message
	//hetSIGNAL_t het_sig;

	sonar_sensor sonar1 = {
			/*&HCSR04*/&US100,
			0,
			0,
			0,
			0,
			0,
			0
	};
	//addSonarSensor(&sonar1);	// Does this work?

	while(1)
	{
		/* Status flag is set to indicate that a new value is latched in the QCPRD register. */
		if((eqepREG1->QEPSTS & 0x80U) !=0U)
		{
			/* Elapsed time between unit position events */
			deltaT = eqepREG1->QCPRD;

			/* Calculate Velocity from deltaT and the value of the unit position. */
			current_speed = (float)eqepREG1->QPOSCNT/(float)deltaT;

			/* Clear the Status flag. */
			eqepREG1->QEPSTS |= 0x80U;
		}





		// ADC Stuff
		//gioSetBit(hetPORT1, 8, 1);
		//while((adcIsConversionComplete(adcREG1,adcGROUP1))==0);
		//adcGetData(adcREG1, adcGROUP1,&adc_data[0]);
		//gioSetBit(hetPORT1, 8, 0);
		//print_debug("ADC Value", "%d", adc_data[1].value);
		//capGetSignal(hetRAM1, cap0, &het_sig);
		//print_info("HET", "Distance: %f cm", (float)het_sig.duty * het_sig.period/58);
		//if(!Sonar_Array.array->_did_i_timeout)
		//{
				//print_info("HET", "Distance: %f ", getDistance(Sonar_Array.array));
		//}

		if(adc_data_is_ready)
		{
			adcGetData(adcREG1, adcGROUP1, adc_data);
			adc_data_is_ready = false;
		}

		// Handle Requests made by user
		if(print_debug_ADC)
		{
			print_debug("ADC 0 Value", "ID: %d Value: %d", (adc_data+0)->id, (adc_data+0)->value);
			print_debug("ADC 1 Value", "ID: %d Value: %d", (adc_data+1)->id, (adc_data+1)->value);
			print_debug("ADC 2 Value", "ID: %d Value: %d", (adc_data+2)->id, (adc_data+2)->value);
			print_debug("ADC 3 Value", "ID: %d Value: %d", (adc_data+3)->id, (adc_data+3)->value);
			print_debug("ADC 4 Value", "ID: %d Value: %d", (adc_data+4)->id, (adc_data+4)->value);
			print_debug("ADC 5 Value", "ID: %d Value: %d", (adc_data+5)->id, (adc_data+5)->value);
			print_debug_ADC = false;
		}
		if(print_status_flag)
		{
			print_info("Velocity", "%f", current_speed);
			print_info("Position Count", "%d", eqepREG1->QPOSCNT);
			print_info("Speed Period", "%d", eqepREG1->QCPRD);
			print_info("eQEP Status Register HEX", "%02x", eqepREG1->QEPSTS);
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
			gioSetBit(gioPORTA, SW_SELECT, command[1]);
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
	}
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
