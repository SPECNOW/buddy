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

static const sonar_module HCSR04 = {
		/*.pulse_width =*/ 10,
		/*.cm_conversion_factor =*/ 1.0/57.0
};	// 2 cm - 4 m
static const sonar_module US100 = {
		/*.pulse_width =*/ 15,
		/*.cm_conversion_factor =*/ 17.0/1026.0
};	// 2.58 cm - 4.31 m

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
void sonarEchoNotification(hetBASE_t * hetREG,uint32 edge);

#endif /* SONAR_H_ */
