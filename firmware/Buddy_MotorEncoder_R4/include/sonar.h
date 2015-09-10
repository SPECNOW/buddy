/*
 * sonar.h
 *
 *  Created on: Apr 29, 2015
 *      Author: Charanpreet
 */

#ifndef SONAR_H_
#define SONAR_H_

#include <stdlib.h>
#include <het.h>

#define MAX_TIMER 		100
#define SENSOR_CLEAR 	-1

//	Struct used to differentiate between Sonar Sensor types (HCSR04 and US100)
typedef struct
{
	const uint16_t pulse_width;
	const float32 cm_conversion_factor;
}sonar_module;

static const sonar_module HCSR04 = {
		/*.pulse_width =*/ 2,
		/*.cm_conversion_factor =*/ 1.0/57.0
};	// 2 cm - 4 m
static const sonar_module US100 = {
		/*.pulse_width =*/ 2,
		/*.cm_conversion_factor =*/ 17.0/1026.0
};	// 2.58 cm - 4.31 m

//	Struct used to keep track of important data for each sonar sensro
typedef struct
{
	const sonar_module * module;		//	Sonar Module for this sensor
	uint16_t trig_pwmpin;		//	PWM Pin used (Set in HalCoGen)
	uint16_t echo_edgepin;		//	EDGE Pin used (Set in HalCoGen) NOTE:	Each EDGE Pin needs to have a matchin CAP Pin
	uint16_t _timeout_timer;	//	Timer used to check if Sesnro has timed out
	float32 _last_distance;		//	Distance returned from latest trigger
	boolean _is_pwm_active;		//	Returns whether the PWM is running for the Trigger Channel
	boolean _did_i_timeout;		//  Returns last state
}sonar_sensor;

//	Struct used to keep track of all sensors in an Array
typedef struct
{
	sonar_sensor * array;
	uint16_t number_sensors;
}sonar_array;

extern sonar_array Sonar_Array;
//= {
//		/*.array =*/ NULL,
//		/*.number_sensors =*/ 0
//};	//	Initialize Array

//	Functions
void startPWM_reg();
void stopPWM_reg();
void sonarPwmNotification(hetBASE_t * hetREG,uint32 pwm, uint32 notification);	//	Pwm Interrupt function
void sonarEdgeNotification(hetBASE_t * hetREG,uint32 edge);	//	Edge Intterupt function
float getDistance(sonar_sensor * sensor);	//	Returns Distance measured by Sensor
void addSonarSensor(sonar_sensor * sonar);		//	Adds a Sensor to the Sonar Array

#endif /* SONAR_H_ */
