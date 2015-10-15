/*
 * sonar.c
 *
 *  Created on: Apr 29, 2015
 *      Author: Charanpreet
 */

#include <sonar.h>

void _delay(int del)
{
	int i = del;
	unsigned int j = 0xF;
	while(i != 0)
	{
		for(j = 0xF; j !=0; j--)
		{

		}
		i--;
	}
}

sonar_array Sonar_Array = {
		/*.array =*/ NULL,
		/*.number_sensors =*/ 0
};	//	Initialize Array

void initSonar(sonar_sensor * sonar)
{
	if(sonar != NULL)
	{
		rtiSetPeriod(rtiCOMPARE0,10 * sonar->module->pulse_width - 1);
		rtiEnableNotification(sonar->rti_compare);
		rtiStartCounter(rtiCOUNTER_BLOCK0);
		sonar->pwm_state = Sonar_Triggered;
	}
}

void rtiSonarNotification(uint32 notification)
{
	static sonar_sensor *sonar;
	unsigned int sonar_index = 0;
	for(sonar_index = 0; sonar_index < Sonar_Array.number_sensors; sonar_index++)
	{
		sonar = Sonar_Array.array + sonar_index;
		if(notification == sonar->rti_compare)		//	Notification is for selected timer
		{
			if(	(sonar->pwm_state == Sonar_Triggered && 0 == gioGetBit(gioPORTA, sonar->trig_pwmpin)) || sonar->_did_i_timeout )	//	The Trigger pin needs to be triggered and The Pin is Low, or there has been a timeout
			{
				gioSetBit(gioPORTA, sonar->trig_pwmpin,1);
				sonar->pwm_state = Sonar_Low;
				sonar->_did_i_timeout = false;
				sonar->_timeout_timer = 0;
			}
			else if(sonar->pwm_state == Sonar_Low && 1 == gioGetBit(gioPORTA, sonar->trig_pwmpin)		//	The Trigger pin needs to be un-triggered and The Pin is High)
			)
			{
				gioSetBit(gioPORTA, sonar->trig_pwmpin,0);
			}
			else
			{
				sonar->_timeout_timer++;
				if(sonar->_timeout_timer > MAX_TIMER)
				{
					sonar->_did_i_timeout = true;
				}
			}
		}
	}
}

float getDistance(sonar_sensor * sensor)
{
	return sensor->_last_distance;
}

void addSonarSensor(sonar_sensor * sensor)
{
	if(sensor != NULL)	//	Check if valid
	{
		sonar_sensor * _new_array = calloc(Sonar_Array.number_sensors+1, sizeof(sonar_sensor));	//	Allocate enough memory for old sensors plus new one

		if(Sonar_Array.number_sensors != 0 || Sonar_Array.array != NULL)	//	Check if Sensor_Array is empty
		{
			memcpy( _new_array, Sonar_Array.array, Sonar_Array.number_sensors*sizeof(sonar_sensor) );	//	Copy Old Sesnors
			memcpy( _new_array + Sonar_Array.number_sensors, sensor, sizeof(sonar_sensor));	//	Add new Sensor to new array
			free(Sonar_Array.array);	//	Free Old Sensor Data
		}
		else
		{
			memcpy( _new_array + Sonar_Array.number_sensors, sensor, sizeof(sonar_sensor));	//	Add new Sensor
		}

		Sonar_Array.array = _new_array;	//	Make Array point to new sensor array
		Sonar_Array.number_sensors++;	//	Increment number of sensors in array
		_new_array = NULL;

		initSonar(getSonarSensor(Sonar_Array.number_sensors-1));
	}
}

sonar_sensor * getSonarSensor(unsigned int index)
{
	sonar_sensor * ret = NULL;
	if(index < Sonar_Array.number_sensors)
	{
		 ret = Sonar_Array.array + index;
	}
	return ret;
}

void sonarEdgeNotification(hetBASE_t * hetREG,uint32 edge)
{
	//	Loop through all sensors in Sonar Array and find which one is interrupted
	uint16_t sensor_index = 0;
	for(sensor_index = 0; sensor_index < Sonar_Array.number_sensors; sensor_index++)
	{
		sonar_sensor * sensor = Sonar_Array.array + sensor_index;	//	Current Sensor
		if(edge == sensor->echo_edgepin)	//	Check if this sesnor has been Interrupted
		{
			/*** NOT YET TESTED HERE ***/
			//	Get the Latest Capture information
			hetSIGNAL_t het_sig;
			capGetSignal(hetRAM1, sensor->echo_edgepin, &het_sig);
			//	Calculate distance
			sensor->_last_distance = (float)het_sig.duty/100.0 * het_sig.period*sensor->module->cm_conversion_factor;
			sensor->_did_i_timeout = false;      //    Obtains distance
			/*****/

			/*//pwmStart(hetRAM1, sensor->trig_pwmpin);	//	Restart PWM for Triggering
			startPWM_reg(sensor);*/
			sensor->pwm_state = Sonar_Triggered;	//	Set PWM	Flag
		}
	}
}

/*********************************************************/
/***************** DEPRECATED CODE ***********************/
/*********************************************************/
//	We Now Use RTI (real Time Interrupt) to do the one Shot PWM instead of the nHET
void startPWM_reg(sonar_sensor* sonar)
{
	pwmSetDuty(hetRAM1, sonar->trig_pwmpin, sonar->module->pulse_width);
}

void stopPWM_reg(sonar_sensor*sonar)
{
	pwmSetDuty(hetRAM1, sonar->trig_pwmpin, 0);
	//hetPORT1->DCLR |= 1U << 24;
}

void sonarPwmNotification(hetBASE_t * hetREG,uint32 pwm, uint32 notification)
{
	/*
	//	Loop through all sensors in Sonar Array and find which one is interrupted
	uint16_t sensor_index = 0;
	for(sensor_index = 0; sensor_index < Sonar_Array.number_sensors; sensor_index++)
	{
		sonar_sensor * sensor = Sonar_Array.array + sensor_index;	//	Current Sesnor
		if(pwm == sensor->trig_pwmpin) //	Check if this sesnor has been Interrupted
		{
			if(sensor->_is_pwm_active)	//	Check if PWM is running (ie, outputting)
			{
				sensor->_timeout_timer = 0;		//	Reset Timeout
				//pwmStop(hetRAM1, pwm);			//	Stop PWM
				stopPWM_reg(sensor);
				sensor->_is_pwm_active = false;	//	Clear PWM Flag
				//_delay(1);
			}
			else
			{
				sensor->_timeout_timer++;		//	If not running (ie, outputting), increment timeout counter
			}

			if(sensor->_timeout_timer >= MAX_TIMER)	//	Check if Sensor has timed out
			{
				sensor->_last_distance = 0.0; //SENSOR_CLEAR;	//	Set that there is nothing blocking sensro
				//pwmStart(hetRAM1, pwm);					//	Restart the PWM for Triggering
				startPWM_reg(sensor);
				sensor->_is_pwm_active = true;			//	Set the PWM Flag
				sensor->_did_i_timeout = true;			//  Set the timeout status flag
			}

			break;	//	Once the interrupted trigger is found, no need to check others, break out of loop
		}
	}
	*/
}
