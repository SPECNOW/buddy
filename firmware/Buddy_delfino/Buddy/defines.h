/*
 * defines.h
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdlib.h>

#include "driverlib.h"
#include "device.h"
#include "pin_configuration.h"

typedef signed char     int8_t;
typedef unsigned char   uint8_t;

extern bool transmitPacket;

#define NUM_ADC_SENSORS 6
#define NUM_ADC_SAMPLES 10

#define NUM_SONAR_SENSORS 2
#define NUM_SONAR_SAMPLES 10

typedef enum DEVICE_MODE {
    normal,
    test_rx,
    test_tx
} DeviceMode;

typedef struct serial_packet
{
    const uint8_t header;
    uint8_t validData;
    uint8_t ultrasonicFront;
    uint8_t ultrasonicBack;
    uint8_t encoderLeft[4];
    uint8_t encoderRight[4];
    uint8_t infraredArray[NUM_ADC_SENSORS];
} SerialPacket;

volatile extern DeviceMode deviceMode;
volatile extern SerialPacket serialPacketRead, serialPacketWrite;

void SCI_Init();
void EQEP_Init();
void ADC_Init();
void GPIO_Init();
void EPWM_Init();
void TMR_Init();

#endif /* DEFINES_H_ */
