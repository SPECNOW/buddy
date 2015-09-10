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

bool adc_data_is_ready = false;

uint8_t left_motor_speed = 193;
uint8_t right_motor_speed = 193;
uint8_t switch_position = 0;

uint16 deltaT = 0;
float current_speed = 0;

//adc_data = NULL;
