/*
 * defines.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */
#include "defines.h"

volatile DeviceMode deviceMode = test_rx;

void sciRXisr(uint32_t sciBase, uint16_t* dataBuffer)
{
    SCI_readCharArray(sciBase, dataBuffer, 1);
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
        break;
    case test_rx:
    case test_tx:
        SCI_writeCharArray(ToPC_Uart, data, 1);
        SCI_writeCharArray(ToSabertooth_Uart, data, 1);
        break;
    default:
        break;
    }
}

__interrupt void sciSabertoothRxIsr(void)
{
    // Received data for Sabertooth-SCI
    uint16_t ToSabertoothRxData[1];

    sciRXisr(ToSabertooth_Uart, ToSabertoothRxData);
    sciRxProcessData(ToSabertoothRxData);
}

__interrupt void sciPCRxIsr(void)
{
    // Received data for PC-SCI
    uint16_t ToPCRxData[1];

    sciRXisr(ToPC_Uart, ToPCRxData);
    sciRxProcessData(ToPCRxData);
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
    SCI_setFIFOInterruptLevel(sciBase, SCI_FIFO_TX1, SCI_FIFO_RX1);

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
        9600,
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

void EQEP_Init() {

}

void ADC_Init() {

}

void GPIO_Init() {

}

void EPWM_Init() {

}
