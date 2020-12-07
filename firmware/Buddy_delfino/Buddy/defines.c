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
    GPIO_setPadConfig(71, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO71
    GPIO_writePin(71, 0);                           // Load output latch
    GPIO_setPinConfig(GPIO_71_GPIO71);              // GPIO71 = GPIO71
    GPIO_setDirectionMode(71, GPIO_DIR_MODE_OUT);   // GPIO71 = output

    GPIO_setPadConfig(20, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO9
    GPIO_setPinConfig(GPIO_20_GPIO20);              // GPIO20 = GPIO20
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_IN);    // GPIO20 = input

    GPIO_setPadConfig(78, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO78
    GPIO_setPinConfig(GPIO_78_GPIO78);              // GPIO78 = GPIO78
    GPIO_setDirectionMode(78, GPIO_DIR_MODE_IN);    // GPIO78 = input

    GPIO_setPadConfig(21, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO21
    GPIO_setPinConfig(GPIO_21_GPIO21);              // GPIO21 = GPIO21
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_IN);    // GPI21 = input

    GPIO_setPadConfig(61, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO61
    GPIO_setPinConfig(GPIO_61_GPIO61);              // GPIO61 = GPIO61
    GPIO_setDirectionMode(61, GPIO_DIR_MODE_IN);    // GPIO61 = input

    GPIO_setPadConfig(43, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO43
    GPIO_setPinConfig(GPIO_43_GPIO43);              // GPIO43 = GPIO43
    GPIO_setDirectionMode(43, GPIO_DIR_MODE_IN);    // GPIO43 = input

    GPIO_setPadConfig(42, GPIO_PIN_TYPE_PULLUP);    // Enable pullup on GPIO42
    GPIO_writePin(42, 0);                           // Load output latch
    GPIO_setPinConfig(GPIO_42_GPIO42);              // GPIO42 = GPIO42
    GPIO_setDirectionMode(42, GPIO_DIR_MODE_OUT);   // GPIO42 = output
}
