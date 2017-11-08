/*
 * defines.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */
#include "defines.h"

void setupSci(uint32_t sciBase, uint32_t rxPin, uint32_t rxConfig, uint32_t txPin, uint32_t txConfig) {
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
    // Initialize interrupt controller and vector table.
    //
    Interrupt_initModule();
    Interrupt_initVectorTable();

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
    SCI_resetChannels(sciBase);
    SCI_resetRxFIFO(sciBase);
    SCI_resetTxFIFO(sciBase);
    SCI_clearInterruptStatus(sciBase, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(sciBase);
    SCI_enableModule(sciBase);
    SCI_performSoftwareReset(sciBase);
}

void SCI_Init() {
    setupSci(
        ToSabertooth_Uart,
        ToSabertooth_Uart_Rx_Pin,
        ToSabertooth_Uart_Rx_PinConfig,
        ToSabertooth_Uart_Tx_Pin,
        ToSabertooth_Uart_Tx_PinConfig
    );
    setupSci(
        ToPC_Uart,
        ToPC_Uart_Rx_Pin,
        ToPC_Uart_Rx_PinConfig,
        ToPC_Uart_Tx_Pin,
        ToPC_Uart_Tx_PinConfig
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
