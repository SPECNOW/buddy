/*
 * pin_maps.h
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */

#ifndef PIN_CONFIGURATION_H_
#define PIN_CONFIGURATION_H_

#include "driverlib.h"


/************** SCI/UART **********/
// To PC
#define ToPC_Uart SCIA_BASE
#define ToPC_Uart_Int_Rx INT_SCIA_RX
#define ToPC_Uart_Tx_Pin 84
#define ToPC_Uart_Tx_PinConfig GPIO_84_SCITXDA

#define ToPC_Uart_Rx_Pin 85
#define ToPC_Uart_Rx_PinConfig GPIO_85_SCIRXDA

// To Sabertooth
#define ToSabertooth_Uart SCIB_BASE
#define ToSabertooth_Uart_Int_Rx INT_SCIB_RX
#define ToSabertooth_Uart_Tx_Pin 18
#define ToSabertooth_Uart_Tx_PinConfig GPIO_18_SCITXDB

#define ToSabertooth_Uart_Rx_Pin 19
#define ToSabertooth_Uart_Rx_PinConfig GPIO_19_SCIRXDB

/************** EQEP **********/
#define LeftMotor_EQUEP_A_Pin 10
#define LeftMotor_EQEPP_A_PinConfig GPIO_10_EQEP1A

#define LeftMotor_EQUEP_B_Pin 11
#define LeftMotor_EQUEP_B_PinConfig GPIO_11_EQEP1B

#define RightMotor_EQUEP_A_Pin 62
#define RightMotor_EQEPP_A_PinConfig GPIO_62_EQEP3A

#define RightMotor_EQUEP_B_Pin 63
#define RightMotor_EQUEP_B_PinConfig GPIO_63_EQEP3B

/************** GPIO **********/
/********  Ultrasonic  ********/
// Pin 39
#define Ultrasonic_A_TRIG_Pin 13
#define Ultrasonic_A_TRIG_PinConfig GPIO_13_GPIO13
// Pin 38
#define Ultrasonic_B_TRIG_Pin 14
#define Ultrasonic_B_TRIG_PinConfig GPIO_14_GPIO14
// Pin 80
#define Ultrasonic_A_ECHO_Pin 2
#define Ultrasonic_A_ECHO_PinConfig GPIO_2_GPIO2
// Pin 79
#define Ultrasonic_B_ECHO_Pin 3
#define Ultrasonic_B_ECHO_PinConfig GPIO_3_GPIO3


#endif /* PIN_CONFIGURATION_H_ */
