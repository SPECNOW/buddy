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

void EPWM_Init() {

}


__interrupt void cpuTimer0ISR(void);
__interrupt void cpuTimer1ISR(void);
__interrupt void cpuTimer2ISR(void);
void configCPUTimer(uint32_t cpuTimer, float freq, float period);

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
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 1000000);
    configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 1000000);
    configCPUTimer(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ, 1000000);

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
configCPUTimer(uint32_t cpuTimer, float freq, float period)
{
    uint32_t temp;

    //
    // Initialize timer period:
    //
    temp = (uint32_t)(freq / 1000000 * period);
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

    //
    // Resets interrupt counters for the three cpuTimers
    //
    /*if (cpuTimer == CPUTIMER0_BASE)
    {
        cpuTimer0IntCount = 0;
    }
    else if(cpuTimer == CPUTIMER1_BASE)
    {
        cpuTimer1IntCount = 0;
    }
    else if(cpuTimer == CPUTIMER2_BASE)
    {
        cpuTimer2IntCount = 0;
    }*/
}


__interrupt void
cpuTimer0ISR(void)
{
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
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
