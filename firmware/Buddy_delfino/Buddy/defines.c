/*
 * defines.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */
#include "defines.h"

volatile DeviceMode deviceMode = normal;

volatile SerialPacket   serialPacketWrite   = {0xFF, 1, 2, 3, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15, 16, 17}},
                        serialPacketRead    = {0xFF, 0, 0, 0, {0}, {0}, {0}};

bool transmitPacket = false;


void EQEP_Init() {

}

void ADC_Init() {

}

void GPIO_Init() {

}

void EPWM_Init() {

}
