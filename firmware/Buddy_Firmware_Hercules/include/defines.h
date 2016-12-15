/*
 * dac_buddy.h
 *
 *  Created on: Mar 2, 2015
 *      Author: Charanpreet
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include "print_debug.h"
#include "adc.h"
#include <stdbool.h>
#include <stdlib.h>


// GIOA ports
//#define SW_ENABLE 0
#define SW_SELECT 1

#define SONAR_ECHO_0 0
#define SONAR_ECHO_1 2

#define NUM_ADC_SENSORS 6

extern unsigned char command[100];

extern bool print_status_flag;
extern bool set_left_motor_speed_flag;
extern bool set_right_motor_speed_flag;
extern bool set_encoder_switch_flag;
extern bool print_command_error_flag;
extern bool print_debug_ADC;
extern bool get_sonar_sensor;
extern bool is_conversion_complete;
extern bool send_serial_packet;

extern bool adc_data_is_ready;

extern uint8_t left_motor_speed;
extern uint8_t right_motor_speed;
extern uint8_t switch_position;

extern uint16 deltaT;
extern float current_speed;

enum serial_data
{
	encoderLeft, encoderRight, ultrasonicFront, ultrasonicBack, infraredArray
};

typedef uint8 serial_data_type;

adcData_t *adc_data;

void delay(int del);
void copySerialData(void* data, serial_data_type type);

typedef struct serial_packet
{
	const uint8_t header;
	uint8_t validData;
	uint8_t ultrasonicFront;
	uint8_t ultrasonicBack;
	float encoderLeft;
	float encoderRight;
	uint8_t infraredArray[NUM_ADC_SENSORS];
} SerialPacket;

extern SerialPacket serialPacketRead, serialPacketWrite;

#endif
