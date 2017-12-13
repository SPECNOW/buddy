/*
 * defines.h
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include "driverlib.h"
#include "device.h"
#include "pin_configuration.h"

typedef enum DEVICE_MODE {
    normal,
    test_rx,
    test_tx
} DeviceMode;

volatile extern DeviceMode deviceMode;


void SCI_Init();
void EQEP_Init();
void ADC_Init();
void GPIO_Init();
void EPWM_Init();



#endif /* DEFINES_H_ */
