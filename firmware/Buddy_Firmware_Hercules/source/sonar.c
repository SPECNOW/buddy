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

/*	sonar_array Sonar_Array
 *
 * 	Global Sonar Array Management Variable. This is used to get access
 * 	to all Sonar Sensors that are in use. Use the addSonarSensor function
 * 	to add elements to this array.
 */
sonar_array Sonar_Array = {
		/*.array =*/ NULL,
		/*.number_sensors =*/ 0
};	//	Initialize Array


/*	void initSonar(sonar_sensor * sonar)
 *
 * 	Intitlializes the Sonar Sensor. Current implementation of this function effectivley does nothing :(
 */
void initSonar(sonar_sensor * sonar)
{
	if(sonar != NULL)
	{
		//rtiSetPeriod(rtiCOMPARE0,10 * sonar->module->pulse_width - 1);
		//rtiEnableNotification(sonar->rti_compare);
		//gioSetBit(gioPORTA, sonar->trig_pwmpin, 0);
		sonar->pwm_state = Sonar_Triggered;
	}
}

/*	void rtiSonarNotification(uint32 notification)
 *
 * 	This is the notification function that is called when the RTI interrupt is triggered. The purpose of this
 * 	function is to generate the Trigger Pulse for the UltraSonic Sensors used in the Sonar Array (minimum 10 uSec).
 *
 * 	For the target Sonar Sensor, the Trigger Pin should be pulled High and the Notification for the RTI Compare used
 * 	for that Sonar Sensor should be enabled before this function is triggered (this function will not trigger until
 * 	the Compare Notification is enabled). This Function will simply pull the trigger pin Low once the required amount
 * 	of time has elapsed.
 *
 * 	TODO:
 * 		-	Instead of having to set the trigger pin High before calling this function, we may be able to simply
* 			set the Trigger Pin from Low to High on the first time this function is triggered and then from Low
* 			to High on the second time it is triggered (as we need to trigger this function twice as is to garuntee
* 			the 10 uSec Pulse width minimum)
 */
void rtiSonarNotification(uint32 notification)
{
	static sonar_sensor *sonar;
	unsigned int sonar_index = 0;
	static int _skip_first_call[32] = {0};	//	Counter for all nHET pins to see if they've been checked before
	_skip_first_call[notification]++;
	/*	The RTI is set to interrupt every 10 uSec. The Sonar requires a minimum of 10 uSec wide Trigger
	 * 	Pulse in order to receive an Echo Response. The state of the RTI clock is unknown when the notification
	 * 	is enabled so therefore the notification may be triggered anytime between 0 and 10 uSec after the
	 * 	notification is enabled. In order to garuntee a minimum of 10 uSec, we need to hit this notification twice
	 * 	before pulling the Trigger Pin low (ie, Trigger width will be between 10 and 20 uSec). IE, we Skip the
	 * 	First Call to the function (which can happen anytime between 0 and 10 uSecs of enabling the Notification)
	 *	and Clear the Trigger Pin on the Second time the notification is hit (garunteeing the 10 uSec Minimum required
	 *	by the Ultrasonic Sensor).
	 *
	 *	See the TODO for a possible alternative implementation
	 */
	for(sonar_index = 0; sonar_index < Sonar_Array.number_sensors; sonar_index++)
	{
		sonar = &Sonar_Array.array[sonar_index];
		if(notification == sonar->rti_compare && _skip_first_call[notification]%2)		//	Notification is for selected timer
		{
			rtiDisableNotification( notification );		//	Disable the Notification (until we read the echo response)
			gioSetBit(gioPORTA, sonar->trig_pwmpin,0);	//	Clear the Trigger Pin
			sonar->pwm_state = Sonar_Low;				//	Update the Sonar Flag
			return;
		}
	}
}


/*	float getDistance(sonar_sensor * sensor)
 *
 *  Returns the Distance that was last measured by the spefied sensor
 */
float getDistance(sonar_sensor * sensor)
{
	return sensor->_last_distance;
}

/*	void addSonarSensor(sonar_sensor * sensor)
 *
 *  Copies the specified Sensor to the Sonar Array (Sonar Array has Global Scope)
 */
void addSonarSensor(sonar_sensor * sensor)
{
	if(sensor != NULL)	//	Check if valid
	{
		sonar_sensor * _new_array = calloc(Sonar_Array.number_sensors+1, sizeof(sonar_sensor));	//	Allocate enough memory for old sensors plus new one

		if(Sonar_Array.number_sensors != 0 || Sonar_Array.array != NULL)	//	Check if Sensor_Array is empty
		{
			memcpy( _new_array, Sonar_Array.array, Sonar_Array.number_sensors*sizeof(sonar_sensor) );	//	Copy Old sensors
			memcpy( _new_array + Sonar_Array.number_sensors, sensor, sizeof(sonar_sensor));	//	Add new Sensor to new array
			free(Sonar_Array.array);	//	Free Old Sonar_Array Data
		}
		else
		{
			memcpy( _new_array + Sonar_Array.number_sensors, sensor, sizeof(sonar_sensor));	//	Add new Sensor
		}

		Sonar_Array.array = _new_array;	//	Make Array point to new sensor array
		Sonar_Array.number_sensors++;	//	Increment number of sensors in array
		_new_array = NULL;

		initSonar(getSonarSensor(Sonar_Array.number_sensors-1));	//	Initialize the New Sonar Sensor
	}
}

/*	getSonarSensor(unsigned int index)
 *
 *  Return a pointer to the Sonar Sensor element in the Sonar Array at the speicifed index.
 */
sonar_sensor * getSonarSensor(unsigned int index)
{
	sonar_sensor * ret = NULL;
	if(index < Sonar_Array.number_sensors)
	{
		 ret = Sonar_Array.array + index;
	}
	return ret;
}

/*	getNextSonar(unsigned int current_index)
 *
 * 	Returns a pointer to the Sonar Sensor that comes next in the Sonar Array after the element at
 * 	the specifed current_index. If current_index is the Last Element in the Sonar Array, this
 * 	function will return the First Element.
 */
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

/*	void sonarEchoNotification(hetBASE_t * hetREG, uint32 edge)
 *
 * 	This function is used to measure the the width the Echo Response from the Sonar Sensor.
 * 	This function will take a timestamp at the start (Rising Edge) and end (Falling Edge)
 * 	of the Echo Response Signal. At the End of the signal, it will then do the neccesary
 * 	calculations using the acquired timestamps to calculate the Distance.
 *
 * 	The Echo Start Timestamp, Echo End Timestamp, and the Calculated Distance are stored
 * 	the appropriate  Sonar Sensor data structure within the Sonar Array.
 */
void sonarEchoNotification(hetBASE_t * hetREG,uint32 edge)
{
	uint32_t _current_time = rtiREG1->CNT[0].FRCx; //Save Start Time
	uint32_t isRisingEdge;
	//uint32_t isRisingEdge = gioGetBit(hetPORT1, edge);
	sonar_sensor * sonar = NULL;

	uint16_t sonar_index = 0;
	for(sonar_index= 0; sonar_index < Sonar_Array.number_sensors; sonar_index++)
	{
		sonar = getSonarSensor(sonar_index);
		if(sonar->echo_edgepin == edge)
		{
			isRisingEdge = gioGetBit(hetPORT1, sonar->echo_edgepin*2);	//*2 takes care of the problem where edge1 needs to be mapped to nHET pin2 and edge0 needs to be mapped to nHET pin0. Find a better way to fix this later.
			if(isRisingEdge)	// Rising Edge
			{
				// 	Save Start Time
				//	Set Print Flag to False
				sonar->_is_echo_time_valid = false;
				sonar->echo_start_time = _current_time;
				//	rtiREG1->CNT[0].UCx = 0;	// This was meant to reset teh RTI clock, but it doesn't seem to work
			}
			else	// Falling Edge
			{
				//	Save End Time
				//  Set Print Flag to True
				sonar->echo_end_time = _current_time;
				sonar->_is_echo_time_valid = true;
				sonar->_last_distance = calculateSonarDistance(sonar);

				rtiEnableNotification(sonar->rti_compare);
				gioSetBit(gioPORTA, sonar->trig_pwmpin,1);
			}
		}
	}

}

/*	float calculateSonarDistance(sonar_sensor * sonar)
 *
 * 	This function will calculate the Distance to the nearest object for the Sonar Sensor.
 *
 * 	This function should be called after the Echo Pulse Width has been measured (ie, we have the timestamps
 * 	for the Rising and Falling edge of the Echo Signal from the Sonar Sensor).
 */
float calculateSonarDistance(sonar_sensor * sonar)
{
	float _delta_time_in_ms = ((float)(sonar->echo_end_time - sonar->echo_start_time))/(100000000.00/(1000000*rtiREG1->CNT[0U].CPUCx));
	float val = _delta_time_in_ms*sonar->module->cm_conversion_factor;

	return val;
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

	return;
}

void doSonar(uint16_t sonar)
{
	getSonarSensor(sonar)->pwm_state = Sonar_Triggered;	//	Set PWM	Flag, signify trig to start
	gioSetBit(gioPORTA, getSonarSensor(sonar)->trig_pwmpin, 1);
	rtiEnableNotification(getSonarSensor(sonar)->rti_compare);
}

void sonarEdgeNotification(hetBASE_t * hetREG,uint32 edge)
{
	//	Loop through all sensors in Sonar Array and find which one is interrupted
	uint16_t sensor_index = 0;
	for(sensor_index = 0; sensor_index < Sonar_Array.number_sensors; sensor_index++)
	{
		sonar_sensor * sensor = &Sonar_Array.array[sensor_index];	//	Current Sensor
		if(edge == sensor->echo_edgepin)	//	Check if this sensor has been Interrupted
		{
			hetSIGNAL_t het_sig;
			capGetSignal(hetRAM1, sensor->echo_edgepin, &het_sig);
			//	Calculate distance
			sensor->_last_distance = (float)het_sig.duty/100.0 * het_sig.period*sensor->module->cm_conversion_factor;
			is_conversion_complete = true;
			get_sonar_sensor = false;
		}
	}
}
