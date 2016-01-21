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
#include <rti.h>
#include <gio.h>
#include "defines.h"

#define MAX_TIMER 		10000	//	100ms
#define SENSOR_CLEAR 	-1

//	Struct used to differentiate between Sonar Sensor types (HCSR04 and US100)
typedef struct
{
	const uint16_t pulse_width;
	const float32 cm_conversion_factor;
}sonar_module;

static const sonar_module HCSR04 = {
		/*.pulse_width =*/ 10,
		/*.cm_conversion_factor =*/ 1.0/57.0
};	// 2 cm - 4 m
static const sonar_module US100 = {
		/*.pulse_width =*/ 15,
		/*.cm_conversion_factor =*/ 17.0/1026.0
};	// 2.58 cm - 4.31 m

typedef enum sonar_state {Sonar_Disabled, Sonar_Triggered, Sonar_Low} SONAR_STATE;

//	Struct used to keep track of important data for each sonar sensro
typedef struct
{
	const sonar_module * module;		//	Sonar Module for this sensor
	uint16_t rti_compare;
	uint16_t trig_pwmpin;		//	PWM Pin used (Set in HalCoGen) on gioPortA
	uint16_t echo_edgepin;		//	EDGE Pin used (Set in HalCoGen) NOTE:	Each EDGE Pin needs to have a matchin CAP Pin (on NHET)
	SONAR_STATE pwm_state;		//	Current State for PWM
	uint32_t _timeout_timer;	//	Timer used to check if Sesnro has timed out
	float32 _last_distance;		//	Distance returned from latest trigger
	boolean _did_i_timeout;		//  Returns last state
	uint32_t echo_start_time;
	uint32_t echo_end_time;
	boolean _is_echo_time_valid;
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
void initSonar(sonar_sensor * sonar);
void startPWM_reg();
void stopPWM_reg();
void startFirstTrigger(int highdur); //manually trigger the high 10us for the first time
void rtiSonarNotification(uint32 notification);
void sonarPwmNotification(hetBASE_t * hetREG,uint32 pwm, uint32 notification);	//	Pwm Interrupt function
void sonarEdgeNotification(hetBASE_t * hetREG,uint32 edge);	//	Edge Intterupt function
float getDistance(sonar_sensor * sensor);	//	Returns Distance measured by Sensor
void addSonarSensor(sonar_sensor * sonar);		//	Adds a Sensor to the Sonar Array
sonar_sensor * getSonarSensor(unsigned int index);
sonar_sensor * getNextSonar(unsigned int index);
void doSonar(uint16_t sonar);
float calculateSonarDistance(sonar_sensor * sonar);

#endif /* SONAR_H_ */
