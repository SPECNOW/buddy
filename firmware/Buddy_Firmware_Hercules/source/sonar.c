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
		//rtiSetPeriod(rtiCOMPARE0,10 * sonar->module->pulse_width - 1);
		//rtiEnableNotification(sonar->rti_compare);
		gioSetBit(gioPORTA, sonar->trig_pwmpin, 0);
		rtiStartCounter(rtiCOUNTER_BLOCK0);
		sonar->pwm_state = Sonar_Triggered;
	}
}

void startFirstTrigger(int highdur)
{
	static sonar_sensor *sonar;
	unsigned int sonar_index = 0;
	for(sonar_index = 0; sonar_index < Sonar_Array.number_sensors; sonar_index++)
	{
		sonar = &Sonar_Array.array[sonar_index];
		gioSetBit(gioPORTA, sonar->trig_pwmpin, 0);
	}
	delay(10000);
	gioSetBit(gioPORTA, getSonarSensor(0)->trig_pwmpin, 1);
	hetInit();
	rtiEnableNotification(getSonarSensor(0)->rti_compare);


	//static sonar_sensor *sonar;
	//unsigned int sonar_index = 0;
	//for(sonar_index = 0; sonar_index < 1/*Sonar_Array.number_sensors*/; sonar_index++)
	//{
	//
	//	sonar = &Sonar_Array.array[sonar_index];
	//	rtiEnableNotification(sonar->rti_compare);
	//	gioSetBit(gioPORTA, sonar->trig_pwmpin,1);
	//	//edgeEnableNotification(hetREG1, 0);
	//	//edgeEnableNotification(hetREG1, 1);
	//	//gioSetBit(gioPORTA, sonar->trig_pwmpin,0);
	//}

	return;
}

void rtiSonarNotification(uint32 notification)
{
	static sonar_sensor *sonar;
	unsigned int sonar_index = 0;
	static int _skip_first_call[32] = {0};
	_skip_first_call[notification]++;
	for(sonar_index = 0; sonar_index < Sonar_Array.number_sensors; sonar_index++)
	{
		sonar = &Sonar_Array.array[sonar_index];
		if(notification == sonar->rti_compare && _skip_first_call[notification]%2)		//	Notification is for selected timer
		{
			//clark start
			rtiDisableNotification( notification );
			gioSetBit(gioPORTA, sonar->trig_pwmpin,0);
			sonar->pwm_state = Sonar_Low;

			return;
			//clark stop
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
			memcpy( _new_array, Sonar_Array.array, Sonar_Array.number_sensors*sizeof(sonar_sensor) );	//	Copy Old sensors
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

sonar_sensor * getNextSonar(unsigned int current_index)
{
	sonar_sensor * ret = NULL;
	unsigned int _index = (current_index+1) % Sonar_Array.number_sensors;
	if(_index < Sonar_Array.number_sensors)
	{
		 ret = Sonar_Array.array + _index;
	}
	return ret;
}

void doSonar(uint16_t sonar)
{
	getSonarSensor(sonar)->pwm_state = Sonar_Triggered;	//	Set PWM	Flag, signify trig to start
	gioSetBit(gioPORTA, getSonarSensor(sonar)->trig_pwmpin, 1);
	rtiEnableNotification(getSonarSensor(sonar)->rti_compare);
}

void sonarEchoNotification(hetBASE_t * hetREG,uint32 edge)
{
	uint32_t isRisingEdge = gioGetBit(hetPORT1, edge);
	uint32_t _current_time = rtiREG1->CNT[0].FRCx;//rtiGetCurrentTick(rtiCOMPARE0);
	sonar_sensor * sonar = NULL;

	//Save Start Time
	uint16_t sonar_index = 0;
	for(sonar_index= 0; sonar_index < Sonar_Array.number_sensors; sonar_index++)
	{
		sonar = getSonarSensor(sonar_index);
		if(sonar->echo_edgepin == edge)
		{
			if(isRisingEdge)
			{
				// 	Save Start Time
				//	Set Print Flag to False
				sonar->_is_echo_time_valid = false;
				sonar->echo_start_time = _current_time;
				//	rtiREG1->CNT[0].UCx = 0;
			}
			else	// Falling Edge
			{
				//	Save End Time
				//  Set Print Flag to True
				sonar->echo_end_time = _current_time;
				sonar->_is_echo_time_valid = true;
				sonar->_last_distance = calculateSonarDistance(sonar);
				is_conversion_complete = true;
			}
		}
	}

}

float calculateSonarDistance(sonar_sensor * sonar)
{
	float _delta_time_in_ms = ((float)(sonar->echo_end_time - sonar->echo_start_time))/(100000000.00/(1000000*rtiREG1->CNT[0U].CPUCx));
	float val = _delta_time_in_ms*sonar->module->cm_conversion_factor;

	if(val < 0)
	{
		is_conversion_complete = false;
	}
	else
	{
		is_conversion_complete = true;
	}

	return val;
}

void sonarEdgeNotification(hetBASE_t * hetREG,uint32 edge)
{
	//	Loop through all sensors in Sonar Array and find which one is interrupted
	uint16_t sensor_index = 0;
	for(sensor_index = 0; sensor_index < Sonar_Array.number_sensors; sensor_index++)
	{

		//CLARK START
		sonar_sensor * sensor = &Sonar_Array.array[sensor_index];	//	Current Sensor
		if(edge == sensor->echo_edgepin)	//	Check if this sensor has been Interrupted
		{
			hetSIGNAL_t het_sig;
			capGetSignal(hetRAM1, sensor->echo_edgepin, &het_sig);
			//	Calculate distance
			sensor->_last_distance = (float)het_sig.duty/100.0 * het_sig.period*sensor->module->cm_conversion_factor;
			is_conversion_complete = true;
			get_sonar_sensor = false;
			//Trigger Next Interrupt
			//getNextSonar(sensor_index)->pwm_state = Sonar_Triggered;	//	Set PWM	Flag, signify trig to start
			//gioSetBit(gioPORTA, getNextSonar(sensor_index)->trig_pwmpin, 1);
			//  set Counter to trip exactly 10 usecs from now somehow!!!! (currently can happen anytime between 10-20 usecs (Okay, not great)
			//rtiEnableNotification(getNextSonar(sensor_index)->rti_compare);
		}
		//CLARK END
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
}

void sonarPwmNotification(hetBASE_t * hetREG,uint32 pwm, uint32 notification)
{

}
