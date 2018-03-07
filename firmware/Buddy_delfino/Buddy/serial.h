/*
 * serial.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Charanpreet
 */

#ifndef SERIAL_H_
#define SERIAL_H_

typedef enum command
{
    getData=0xffff, leftMotor, rightMotor
} command_type;

typedef enum serial_data
{
    encoderLeft, encoderRight, ultrasonicFront, ultrasonicBack, infraredArray
} serial_data_type;

void sciRXisr(uint32_t sciBase, uint16_t* dataBuffer);
void sciRxProcessData(uint16_t* data);
__interrupt void sciSabertoothRxIsr(void);
__interrupt void sciPCRxIsr(void);
void setupSci(uint32_t sciBase, uint32_t rxPin, uint32_t rxConfig, uint32_t txPin,
              uint32_t txConfig, uint32_t rxInterruptVector, void (*rxInterruptFunction)(void));
void SCI_Init();

void handleCommand(uint16_t* command);
void copySerialData(void* data, serial_data_type type);
void sendBuddyData();

#endif /* SERIAL_H_ */
