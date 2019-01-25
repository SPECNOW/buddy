/*
 * defines.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Charanpreet
 */
#include "defines.h"

volatile DeviceMode deviceMode = normal;

volatile SerialPacket   serialPacketWrite   = {0xFF, 0, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}},//{0xFF, 1, 2, 3, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15, 16, 17}},
                        serialPacketRead    = {0xFF, 0, 0, 0, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};

bool transmitPacket = false;

Triggers TRIGGER_ARRAY[2];

void EQEP_Init() {

}

void ADC_Init() {

}

__interrupt void gpioEchoATimerISR(void);
__interrupt void gpioEchoBTimerISR(void);

void GPIO_Init() {
    Interrupt_register(INT_XINT1, &gpioEchoATimerISR);
    Interrupt_register(INT_XINT2, &gpioEchoBTimerISR);

    GPIO_setPadConfig(86, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO9
    GPIO_writePin(86, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_86_GPIO86);                // GPIO9 = GPIO9
    GPIO_setDirectionMode(86, GPIO_DIR_MODE_OUT);    // GPIO9 = output

    GPIO_setPadConfig(87, GPIO_PIN_TYPE_PULLUP);     // Enable pullup on GPIO9
    GPIO_writePin(87, 0);                            // Load output latch
    GPIO_setPinConfig(GPIO_87_GPIO87);                // GPIO9 = GPIO9
    GPIO_setDirectionMode(87, GPIO_DIR_MODE_OUT);    // GPIO9 = output


    // Ultrasonic Triggers
    GPIO_setPadConfig(Ultrasonic_A_TRIG_Pin, GPIO_PIN_TYPE_PULLUP);  
    GPIO_writePin(Ultrasonic_A_TRIG_Pin, 0);                         
    GPIO_setPinConfig(Ultrasonic_A_TRIG_PinConfig);               
    GPIO_setDirectionMode(Ultrasonic_A_TRIG_Pin, GPIO_DIR_MODE_OUT); 

    GPIO_setPadConfig(Ultrasonic_B_TRIG_Pin, GPIO_PIN_TYPE_PULLUP);
    GPIO_writePin(Ultrasonic_B_TRIG_Pin, 0);
    GPIO_setPinConfig(Ultrasonic_B_TRIG_PinConfig);
    GPIO_setDirectionMode(Ultrasonic_B_TRIG_Pin, GPIO_DIR_MODE_OUT);

    // Ultrasonic Echoes
    //GPIO_setPadConfig(Ultrasonic_A_ECHO_Pin, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(Ultrasonic_A_ECHO_PinConfig);
    GPIO_setDirectionMode(Ultrasonic_A_ECHO_Pin, GPIO_DIR_MODE_IN);
    GPIO_setInterruptPin(Ultrasonic_A_ECHO_Pin, GPIO_INT_XINT1);
    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_BOTH_EDGES);

    //GPIO_setPadConfig(Ultrasonic_B_ECHO_Pin, GPIO_PIN_TYPE_PULLUP);
    GPIO_setPinConfig(Ultrasonic_B_ECHO_PinConfig);
    GPIO_setDirectionMode(Ultrasonic_B_ECHO_Pin, GPIO_DIR_MODE_IN);
    GPIO_setInterruptPin(Ultrasonic_B_ECHO_Pin, GPIO_INT_XINT2);
    GPIO_setInterruptType(GPIO_INT_XINT2, GPIO_INT_TYPE_BOTH_EDGES);

    GPIO_enableInterrupt(GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT2);

    Interrupt_enable(INT_XINT1);
    Interrupt_enable(INT_XINT2);

}

void EPWM_Init() {

}


__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
__interrupt void cpuTimer2ISR(void);
void configCPUTimer(uint32_t cpuTimer, float period);

void TMR_Init() {

    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);
    Interrupt_register(INT_TIMER2, &cpuTimer2ISR);

    //
    // Initialize timer period to maximum
    //
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFF);

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    //
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);

    //
    // Make sure timer is stopped
    //
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER1_BASE);
    CPUTimer_stopTimer(CPUTIMER2_BASE);

    //
    // Reload all counter register with period value
    //
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);

    //
    // Reset interrupt counter
    //
    /*cpuTimer0IntCount = 0;
    cpuTimer1IntCount = 0;
    cpuTimer2IntCount = 0;*/

    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 1 second Period (in uSeconds)
    //
    configCPUTimer(CPUTIMER0_BASE, 10); // 10 uS
    configCPUTimer(CPUTIMER1_BASE, 1000000);
    configCPUTimer(CPUTIMER2_BASE, 1000000);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in configCPUTimer and initCPUTimers, the below settings must also
    // be updated.
    //
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    //
    // Enables CPU int1, int13, and int14 which are connected to CPU-Timer 0,
    // CPU-Timer 1, and CPU-Timer 2 respectively.
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);
    Interrupt_enable(INT_TIMER2);

    //
    // Starts CPU-Timer 0, CPU-Timer 1, and CPU-Timer 2.
    //
    CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);
}

//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
void
configCPUTimer(uint32_t cpuTimer, float period)
{
    uint32_t temp;

    //
    // Initialize timer period:
    //
    temp = (uint32_t)(200 * period);
    CPUTimer_setPeriod(cpuTimer, temp);

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CPUTimer_setPreScaler(cpuTimer, 0);

    //
    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    //
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);
}


__interrupt void
cpuTimer0ISR(void)
{
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    uint8_t i = 0;
    for(i = 0; i < 2; i++) {
        if(TRIGGER_ARRAY[i].trigger) {
            TRIGGER_ARRAY[i].counter++;
            TRIGGER_ARRAY[i].timeout = 0;
            if (0 == TRIGGER_ARRAY[i].counter%2) {
                GPIO_togglePin(i == 0 ? Ultrasonic_A_TRIG_Pin: Ultrasonic_B_TRIG_Pin);
                if (0 == TRIGGER_ARRAY[i].counter%4) {
                    TRIGGER_ARRAY[i].counter = 0;
                    TRIGGER_ARRAY[i].trigger = false;
                }
            }
        } else {
            TRIGGER_ARRAY[i].timeout++;
            TRIGGER_ARRAY[i].counter = 0;
            if(TRIGGER_ARRAY[i].timeout > TRIGGER_TIMEOUT) {
                TRIGGER_ARRAY[i].trigger = true;
            }
        }
    }
}

//
// cpuTimer1ISR - Counter for CpuTimer1
//
__interrupt void
cpuTimer1ISR(void)
{
    //
    // The CPU acknowledges the interrupt.
    //
}

//
// cpuTimer2ISR - Counter for CpuTimer2
//
__interrupt void
cpuTimer2ISR(void)
{
    //
    // The CPU acknowledges the interrupt.
    //
}


__interrupt void
gpioEchoATimerISR(void)
{
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    static uint32_t start_time = 0, delta = 0;
    if (GPIO_readPin(Ultrasonic_A_ECHO_Pin)) {
        start_time = CPUTimer_getTimerCount(CPUTIMER1_BASE);
    } else {
        delta = start_time - CPUTimer_getTimerCount(CPUTIMER1_BASE);
        TRIGGER_ARRAY[0].trigger = true;
    }
}
__interrupt void
gpioEchoBTimerISR(void)
{
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    static uint32_t start_time = 0, delta = 0;
        if (GPIO_readPin(Ultrasonic_B_ECHO_Pin)) {
            start_time = CPUTimer_getTimerCount(CPUTIMER1_BASE);
        } else {
            delta = start_time - CPUTimer_getTimerCount(CPUTIMER1_BASE);
            TRIGGER_ARRAY[1].trigger = true;
        }
}

