/*
 * defines.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */
#include "defines.h"
#include "serial.h"

volatile DeviceMode deviceMode = normal;

volatile SerialPacket   serialPacketWrite   = {0xFF, 0, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},//{0xFF, 1, 2, 3, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15, 16, 17}},
                        serialPacketRead    = {0xFF, 0, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};

bool transmitPacket = false;

uint8_t movingAverage(uint8_t *samples, uint8_t *pos, int num_samples, int new_sample)
{
    // Sum up samples
    int sum = 0;
    uint8_t i = 0;
    for(i=0; i < num_samples; i++)
    {
        sum += samples[i];
    }

    //Assign the nextNum to the position in the array
    samples[*pos] = new_sample;
    *pos = (*pos+1) % num_samples;

    //return the average
    return sum/num_samples;
}

void EQEP_Init() {

}

void ADC_Init() {

}

void GPIO_Init() {
    GPIO_setPadConfig(86, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO9
    GPIO_writePin(86, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_86_GPIO86);                // GPIO9 = GPIO9
    GPIO_setDirectionMode(86, GPIO_DIR_MODE_OUT);    // GPIO9 = output

    GPIO_setPadConfig(87, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO9
    GPIO_writePin(87, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_87_GPIO87);                // GPIO9 = GPIO9
    GPIO_setDirectionMode(87, GPIO_DIR_MODE_OUT);    // GPIO9 = output
}
