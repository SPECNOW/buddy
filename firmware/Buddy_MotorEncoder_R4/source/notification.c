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
#include "adc.h"
#include "gio.h"
#include "sci.h"
#include "het.h"
#include "eqep.h"

/* USER CODE BEGIN (0) */
#include "defines.h"
#include <stdio.h>
#include "dac_buddy.h"
#include "sonar.h"

//#define MAX_TIMER 60
bool is_pwm0_running = true;

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
#pragma WEAK(adcNotification)
void adcNotification(adcBASE_t *adc, uint32 group)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (11) */
	//adcGetData(adc, group, adc_data);
	adc_data_is_ready = true;
	//adcResetFiFo(adc, group);
	//adcStartConversion(adc,group);
	/* USER CODE END */
}

/* USER CODE BEGIN (12) */
/* USER CODE END */
#pragma WEAK(gioNotification)
void gioNotification(gioPORT_t *port, uint32 bit)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (19) */
/* USER CODE END */
}

/* USER CODE BEGIN (20) */
/* USER CODE END */

#pragma WEAK(sciNotification)
void sciNotification(sciBASE_t *sci, uint32 flags)     
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (29) */
	if( flags == SCI_RX_INT)
	{
		if( strcmp("sr", (const char*)command) == 0 )	//	Status Registers
		{
			print_status_flag = true;
		}
		else if( strcmp( "av", (const char*) command ) == 0 )	// ADC Valuse
		{
			print_debug_ADC = true;
		}
		else if(command[0] == 't')			//	Toggle Switch
		{
			set_encoder_switch_flag = true;
			switch_position = command[1];
		}
		else if(command[0] == 'L')				// Left Motor Speed
		{
			set_left_motor_speed_flag = true;
			left_motor_speed = command[1];
		}
		else if(command[0] == 'R')				// Right Motor Speed
		{
			set_right_motor_speed_flag = true;
			right_motor_speed = command[1];
		}
		else
		{
			print_command_error_flag = true;
		}
		sciReceive(scilinREG, 2, (unsigned char *)&command);
	}
	else
	{
		while(1);
	}
/* USER CODE END */
}

/* USER CODE BEGIN (30) */
/* USER CODE END */

#pragma WEAK(pwmNotification)
void pwmNotification(hetBASE_t * hetREG,uint32 pwm, uint32 notification)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (35) */
	sonarPwmNotification(hetREG, pwm, notification);
//	static uint32_t timeout_timer = 0;
//	if(pwm == pwm0 /*&& notification == pwmEND_OF_DUTY*/)
//	{
//		if(is_pwm0_running)
//		{
//			timeout_timer = 0;
//			pwmStop(hetRAM1, pwm);
//			is_pwm0_running = false;
//		}
//		else
//		{
//			timeout_timer++;
//		}
//	}
//
//	if(timeout_timer >= MAX_TIMER)
//	{
//		pwmStart(hetRAM1, pwm0);
//		is_pwm0_running = true;
//	}
/* USER CODE END */
}

/* USER CODE BEGIN (36) */
/* USER CODE END */
#pragma WEAK(edgeNotification)
void edgeNotification(hetBASE_t * hetREG,uint32 edge)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (37) */
	sonarEdgeNotification(hetREG, edge);
	/*if(edge == edge0)
	{
		pwmStart(hetRAM1, pwm0);
		is_pwm0_running = true;
	}*/
/* USER CODE END */
}

/* USER CODE BEGIN (38) */
/* USER CODE END */
#pragma WEAK(hetNotification)
void hetNotification(hetBASE_t *het, uint32 offset)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (39) */
	while(1);
/* USER CODE END */
}

/* USER CODE BEGIN (40) */
/* USER CODE END */


/* USER CODE BEGIN (43) */
/* USER CODE END */


/* USER CODE BEGIN (47) */
/* USER CODE END */

#pragma WEAK(eqepNotification)
void eqepNotification(eqepBASE_t *eqep,uint16 flags)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (48) */
/* USER CODE END */
}
/* USER CODE BEGIN (49) */
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
