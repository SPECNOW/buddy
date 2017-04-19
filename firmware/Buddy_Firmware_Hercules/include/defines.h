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
#define NUM_ADC_SAMPLES 10

#define NUM_SONAR_SENSORS 2
#define NUM_SONAR_SAMPLES 10

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
extern bool print_debug_sonar;

extern bool adc_data_is_ready;

extern uint8_t left_motor_speed;
extern uint8_t right_motor_speed;
extern uint8_t switch_position;

extern uint32 deltaT;
extern float current_speed, last_postion, current_postion;

enum serial_data
{
	encoderLeft, encoderRight, ultrasonicFront, ultrasonicBack, infraredArray
};

typedef uint8 serial_data_type;

adcData_t *adc_data;

void delay(int del);
void copySerialData(void* data, serial_data_type type);
void addADCSample();

typedef struct serial_packet
{
	const uint8_t header;
	uint8_t validData;
	uint8_t ultrasonicFront;
	uint8_t ultrasonicBack;
	uint32_t encoderLeft;
	uint32_t encoderRight;
	uint8_t infraredArray[NUM_ADC_SENSORS];
} SerialPacket;

extern SerialPacket serialPacketRead, serialPacketWrite;

//	Struct used to differentiate between Sonar Sensor types (HCSR04 and US100)
typedef struct
{
	const uint16_t pulse_width;
	const float32 cm_conversion_factor;
}sonar_module;

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

//Struct to manage Sonar Data
typedef struct
{
	float32 average[NUM_SONAR_SENSORS];
	float32 data[NUM_SONAR_SENSORS][NUM_SONAR_SAMPLES];
	uint8_t index;
}sonar_sample;

//	Struct used to keep track of all sensors in an Array
typedef struct
{
	sonar_sensor * array;
	uint16_t number_sensors;
	sonar_sample sonarSampler;
}sonar_array;

extern sonar_array Sonar_Array;

typedef struct
{
	uint8_t average[NUM_ADC_SENSORS];
	uint8_t data[NUM_ADC_SENSORS][NUM_ADC_SAMPLES];
	uint8_t index;
}ir_sample;

extern ir_sample irArray;


void addSonarSample(uint8_t sensor_index);

typedef enum eqep_motor_state {LEFT_MOTOR, RIGHT_MOTOR} EQEP_STATE;

typedef struct
{
	uint32 left_motor_period;
	uint32 right_motor_period;
	EQEP_STATE current_motor;
}motor_periods;

extern motor_periods motorPeriods;

#endif
