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

void addSonarSample()
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
