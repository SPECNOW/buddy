/*
 * defines.c
 *
 *  Created on: Mar 14, 2015
 *      Author: Charanpreet
 */

#include "defines.h"

unsigned char command[100];

bool print_status_flag = false;
bool set_left_motor_speed_flag = false;
bool set_right_motor_speed_flag  = false;
bool set_encoder_switch_flag = false;
bool print_command_error_flag = false;
bool print_debug_ADC = false;
bool get_sonar_sensor = false;
bool is_conversion_complete = false;
bool send_serial_packet = false;
bool print_debug_sonar = false;

bool adc_data_is_ready = false;

uint8_t left_motor_speed = 193;
uint8_t right_motor_speed = 193;
uint8_t switch_position = 0;

uint16 deltaT = 0;
float current_speed = 0;

SerialPacket 	serialPacketWrite 	= {0xFF, 0, 0, 0, 0, 0, {0}},
				serialPacketRead 	= {0xFF, 0, 0, 0, 0, 0, {0}};
ir_sample irArray;
sonar_array sonarArray;

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

void delay(int del)
{
	int i = del;
	unsigned int j = 0x0FFF;
	while(i != 0)
	{
		for(j = 0x0FFF; j !=0; j--)
		{

		}
		i--;
	}
}

void copySerialData(void* data, serial_data_type type)
{
	switch(type)
	{
	case encoderLeft:
		serialPacketWrite.validData |=  0x80 >> encoderLeft;
		memcpy((float*)&(serialPacketWrite.encoderLeft), (float*)data, sizeof(float));
		return;
	case encoderRight:
		serialPacketWrite.validData |=  0x80 >> encoderRight;
		memcpy((float*)&(serialPacketWrite.encoderRight), (float*)data, sizeof(float));
		return;
	case ultrasonicFront:
		serialPacketWrite.validData |=  0x80 >> ultrasonicFront;
		memcpy((uint8*)&(serialPacketWrite.ultrasonicFront), (uint8*)data, sizeof(uint8));
		return;
	case ultrasonicBack:
		serialPacketWrite.validData |=  0x80 >> ultrasonicBack;
		memcpy((uint8*)&(serialPacketWrite.ultrasonicBack), (uint8*)data, sizeof(uint8));
		return;
	case infraredArray:
		serialPacketWrite.validData |= 0x80 >> infraredArray;
		memcpy((uint8*)&(serialPacketWrite.infraredArray), (uint8*)data, sizeof(uint8)*NUM_ADC_SENSORS);
		return;
	default:
		break;
	};
}

void addADCSample()
{
	unsigned int adc_index = 0, array_index = 0;
	irArray.index = (irArray.index++)%NUM_ADC_SAMPLES;
	array_index = irArray.index++%NUM_ADC_SAMPLES;
	unsigned int i=0, j=0;
	uint16_t temp_average = 0;

	for(i=0; i < NUM_ADC_SENSORS; i++)
	{
		adc_index = adc_data[i].id;
		irArray.data[adc_index][array_index] = adc_data[i].value;
		temp_average = 0;
		for(j=0; j < NUM_ADC_SAMPLES; j++)
		{
			temp_average += (uint16_t)irArray.data[adc_index][j];
		}
		irArray.average[adc_index] = (uint8_t) (temp_average/NUM_ADC_SAMPLES);
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

void addSonarSample(uint8_t sensor_index)
{
	unsigned int array_index = 0;
	Sonar_Array.sonarSampler.index = (Sonar_Array.sonarSampler.index++)%NUM_ADC_SAMPLES;
	array_index = Sonar_Array.sonarSampler.index++%NUM_ADC_SAMPLES;
	unsigned int j=0;
	float32 temp_average = 0;


	Sonar_Array.sonarSampler.data[sensor_index][array_index] = Sonar_Array.array[sensor_index]._last_distance;
	temp_average = 0;
	for(j=0; j < NUM_SONAR_SAMPLES; j++)
	{
		temp_average += Sonar_Array.sonarSampler.data[sensor_index][j];
	}

	Sonar_Array.sonarSampler.average[sensor_index] = (temp_average/NUM_SONAR_SAMPLES);
}
