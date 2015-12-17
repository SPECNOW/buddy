/*
 * ir_adc.h
 * created on Oct 14th, 2015
 * Author: Clark
 */
/*
#include <stdlib.h>
include <adc.h>

//	Struct used to keep track of important data for each IR sensor
typedef struct
{
	//const ir_module * module;
	uint16_t adc_pin;		//ADC in pin used for reading the voltage
	uint16_t checking_duration;	//Variable used to keep track of checking frequency
	uint16_t _last_adc_value;	//ADC value from the sensor
	boolean _adc_conversion_active;	//Flag for being in the conversion list
	boolean _adc_conversion_busy;	//Busy flag
}ir_sensor;

//	Struct used to keep track of the each number of ir sensor
typedef struct
{
	ir_sensor * sensor;
	uint16_t number_of_sensors;	//Number of sensors
}

float getDistance_IR(ir_sensor * sensor);	//Calculate fromt he adc value and return the distance in mm 
void addIRSensor(ir_sensor * sensor);		//Adds a sensor to the IR array
int addIRConversion(ir_sensor * sensor);	//Add the sensor to the list to be ADC converted
int removeIRConversion(ir_sensor *sensor);	//Remove the IR sensor from the ADC conversion list


*/
