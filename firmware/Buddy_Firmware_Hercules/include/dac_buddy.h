/*
 * dac_buddy.h
 *
 *  Created on: Mar 2, 2015
 *      Author: Charanpreet
 */

#ifndef DAC_BUDDY_H_
#define DAC_BUDDY_H_

#include "gio.h"
#define HIGH 1
#define LOW 0

typedef struct MotorDAC
{
	uint8_t SCLK;
	uint8_t SDATA;
	uint8_t LDPIN;
	uint8_t DACNUM;
} MotorDAC;

// GIOA Pin Numbers for DAC Chip. LDAC is connected to GND
static MotorDAC leftDAC = {4, 6, 7, 0}; // CLK, DATA, LOAD, DACNUM
static MotorDAC rightDAC= {4, 6, 7, 1}; // CLK, DATA, LOAD, DACNUM

void delay(int del);
void initDAC(MotorDAC * motor_dac, uint8_t serial_clock, uint8_t serial_data, uint8_t load_pin, uint8_t dac_num);
void sendDAC(MotorDAC * motor_dac, unsigned char val);

#endif /* DAC_BUDDY_H_ */
