/*
 * serial.c
 *
 *  Created on: Feb 27, 2018
 *      Author: Charanpreet
 */

#include "includes.h"

extern volatile DeviceMode deviceMode;
// NOTE: This should be 1 for test_rx/test_tx
#define RX_BUFFER_SIZE 2

void sciRXisr(uint32_t sciBase, uint16_t* dataBuffer)
{
    SCI_readCharArray(sciBase, dataBuffer, RX_BUFFER_SIZE);
    SCI_clearOverflowStatus(sciBase);
    SCI_clearInterruptStatus(sciBase, SCI_INT_RXFF);
    //
    // Issue PIE ack
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

void sciRxProcessData(uint16_t* data)
{
    switch(deviceMode) {
    case normal:
        // Do something?
        break;
    case test_rx:
    case test_tx:
        SCI_writeCharArray(ToPC_Uart, data, RX_BUFFER_SIZE);
        SCI_writeCharArray(ToSabertooth_Uart, data, RX_BUFFER_SIZE);
        break;
    default:
        break;
    }
}

__interrupt void sciSabertoothRxIsr(void)
{
    // Received data for Sabertooth-SCI
    uint16_t FromSabertoothRxData[RX_BUFFER_SIZE] = {0};

    sciRXisr(ToSabertooth_Uart, FromSabertoothRxData);
    sciRxProcessData(FromSabertoothRxData);
}

__interrupt void sciPCRxIsr(void)
{
    GPIO_writePin(87, 1);
    // Received data for PC-SCI
    uint16_t FromPCRxData[RX_BUFFER_SIZE] = {0};

    sciRXisr(ToPC_Uart, FromPCRxData);
    sciRxProcessData(FromPCRxData);
    handleCommand(FromPCRxData);
    GPIO_writePin(87, 0);
}

void setupSci(uint32_t sciBase, uint32_t rxPin, uint32_t rxConfig, uint32_t txPin,
              uint32_t txConfig, uint32_t rxInterruptVector, void (*rxInterruptFunction)(void)) {
    //
    // Set the SCI Rx pin.
    //
    GPIO_setMasterCore(rxPin, GPIO_CORE_CPU1);
    GPIO_setPinConfig(rxConfig);
    GPIO_setDirectionMode(rxPin, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(rxPin, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(rxPin, GPIO_QUAL_ASYNC);

    //
    // Set the SCI Tx pin.
    //
    GPIO_setMasterCore(txPin, GPIO_CORE_CPU1);
    GPIO_setPinConfig(txConfig);
    GPIO_setDirectionMode(txPin, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(txPin, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(txPin, GPIO_QUAL_ASYNC);

    //
    // RX and TX FIFO Interrupts Enabled
    //
    SCI_enableInterrupt(sciBase, SCI_INT_RXFF);
    SCI_disableInterrupt(sciBase, SCI_INT_RXERR);

    // Interrupt FIFO when it has one 1
    SCI_setFIFOInterruptLevel(sciBase, (SCI_FIFO_TX0 + RX_BUFFER_SIZE), (SCI_FIFO_RX0+ RX_BUFFER_SIZE));

    //
    // Initialize SCI BASE and its FIFO.
    //
    SCI_performSoftwareReset(sciBase);

    //
    // Configure SCI.
    // 8-Bit, No Parity, One stop Bit
    //
    SCI_setConfig(
        sciBase,
        DEVICE_LSPCLK_FREQ,
        115200,
        (SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE)
    );

    Interrupt_register(rxInterruptVector, rxInterruptFunction);
    Interrupt_enable(rxInterruptVector);
    SCI_resetChannels(sciBase);
    SCI_resetRxFIFO(sciBase);
    SCI_resetTxFIFO(sciBase);
    SCI_clearInterruptStatus(sciBase, SCI_INT_TXFF | SCI_INT_RXFF);

    SCI_enableFIFO(sciBase);
    SCI_enableModule(sciBase);
    SCI_performSoftwareReset(sciBase);
}

void SCI_Init() {
    //
    // Initialize interrupt controller and vector table.
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();

    setupSci(
        ToSabertooth_Uart,
        ToSabertooth_Uart_Rx_Pin,
        ToSabertooth_Uart_Rx_PinConfig,
        ToSabertooth_Uart_Tx_Pin,
        ToSabertooth_Uart_Tx_PinConfig,
        ToSabertooth_Uart_Int_Rx,
        sciSabertoothRxIsr
    );
    setupSci(
        ToPC_Uart,
        ToPC_Uart_Rx_Pin,
        ToPC_Uart_Rx_PinConfig,
        ToPC_Uart_Tx_Pin,
        ToPC_Uart_Tx_PinConfig,
        ToPC_Uart_Int_Rx,
        sciPCRxIsr
    );
}

void handleCommand(uint16_t* rawCommand)
{
    uint16_t command = (rawCommand[0] | rawCommand[1] << 8);

    switch(command)
    {
    case getData:
        transmitPacket = true;
        break;
    default:
        break;
    }
}

void copySerialData(void* data, serial_data_type type)
{
    switch(type)
    {
    case encoderLeft:
        serialPacketWrite.validData |=  0x80 >> encoderLeft;
        // Memory alignment is weird so we need to do this instead of memcpy
        serialPacketWrite.encoderLeft[3] = ((*((uint8_t*)data+0) & 0x00ff) >> 0);
        serialPacketWrite.encoderLeft[2] = ((*((uint8_t*)data+0) & 0xff00) >> 8);
        serialPacketWrite.encoderLeft[1] = ((*((uint8_t*)data+1) & 0x00ff) >> 0);
        serialPacketWrite.encoderLeft[0] = ((*((uint8_t*)data+1) & 0xff00) >> 8);
        return;
    case encoderRight:
        serialPacketWrite.validData |=  0x80 >> encoderRight;
        // Memory alignment is weird so we need to do this instead of memcpy
        serialPacketWrite.encoderRight[3] = ((*((uint8_t*)data+0) & 0x00ff) >> 0);
        serialPacketWrite.encoderRight[2] = ((*((uint8_t*)data+0) & 0xff00) >> 8);
        serialPacketWrite.encoderRight[1] = ((*((uint8_t*)data+1) & 0x00ff) >> 0);
        serialPacketWrite.encoderRight[0] = ((*((uint8_t*)data+1) & 0xff00) >> 8);
        return;
    case ultrasonicFront:
        serialPacketWrite.validData |=  0x80 >> ultrasonicFront;
        memcpy(
            (uint8_t*)&(serialPacketWrite.ultrasonicFront),
            (uint8_t*)data,
            sizeof(uint8_t)
        );
        return;
    case ultrasonicBack:
        serialPacketWrite.validData |=  0x80 >> ultrasonicBack;
        memcpy(
            (uint8_t*)&(serialPacketWrite.ultrasonicBack),
            (uint8_t*)data,
            sizeof(uint8_t)
        );
        return;
    case infraredArray:
        serialPacketWrite.validData |= 0x80 >> infraredArray;
        memcpy(
            (uint8_t*)&(serialPacketWrite.infraredArray),
            (uint8_t*)data,
            sizeof(uint8_t)*NUM_ADC_SENSORS
        );
        return;
    default:
        break;
    };
}

void sendBuddyData()
{
    long encL = 0xc0d0e0f0;
    long encR = 0xf0e0d0c0;
    copySerialData(&encL, encoderLeft);
    copySerialData(&encR, encoderRight);
    memcpy((void*)&serialPacketRead, (void*)&serialPacketWrite, sizeof(SerialPacket));    
    serialPacketWrite.validData = 0;
    GPIO_writePin(86, 1);
    SCI_writeCharArray(ToPC_Uart, (void*)&serialPacketRead, sizeof(SerialPacket));
    GPIO_writePin(86, 0);
}
