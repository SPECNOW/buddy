/*
 * dac_buddy.c
 *
 *  Created on: Mar 2, 2015
 *      Author: Charanpreet
 */

#include "dac_buddy.h"

void initDAC(MotorDAC * motor_dac, uint8_t serial_clock, uint8_t serial_data, uint8_t load_pin, uint8_t dac_num)
{
	motor_dac->SCLK = serial_clock;
	motor_dac->SDATA = serial_data;
	motor_dac->LDPIN = load_pin;
	motor_dac->DACNUM = dac_num;

	gioSetDirection(gioPORTA, gioPORTA->DIR | 1 << motor_dac->SCLK | 1 << motor_dac->SDATA | 1 << motor_dac->LDPIN);
	gioSetBit(gioPORTA, motor_dac->SCLK, LOW);
	gioSetBit(gioPORTA, motor_dac->SDATA, LOW);
	gioSetBit(gioPORTA, motor_dac->LDPIN, HIGH);
}

void sendDAC(MotorDAC * motor_dac, unsigned char val)
{
	signed char x = 1;
	for(x = 1; x >= 0; x--)
	{
		gioSetBit(gioPORTA, motor_dac->SCLK, HIGH);
		gioSetBit(gioPORTA, motor_dac->SDATA, motor_dac->DACNUM & ( 1 << x) );
		delay(1);
		gioSetBit(gioPORTA, motor_dac->SCLK, LOW);
		delay(1);
	}
	//RNG
	gioSetBit(gioPORTA, motor_dac->SCLK, HIGH);
	gioSetBit(gioPORTA, motor_dac->SDATA, LOW ); //RNG VALUE
	delay(1);
	gioSetBit(gioPORTA, motor_dac->SCLK, LOW);
	delay(1);
	//
	signed char i = 7;
	for(i = 7; i >= 0; i--)
	{
		gioSetBit(gioPORTA, motor_dac->SCLK, HIGH);
		gioSetBit(gioPORTA, motor_dac->SDATA, val &( 1 << i) );
		delay(1);
		gioSetBit(gioPORTA, motor_dac->SCLK, LOW);
		delay(1);
	}

	gioSetBit(gioPORTA, motor_dac->LDPIN, LOW);
	delay(1);
	gioSetBit(gioPORTA, motor_dac->LDPIN, HIGH);
}
