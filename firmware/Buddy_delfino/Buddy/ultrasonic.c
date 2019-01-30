/*
 * ultrasonic.c
 *
 *  Created on: Jan 29, 2019
 *      Author: Charanpreet
 */
#include "includes.h"

const uint8_t MAX_SONAR = 0xFF;

Triggers TRIGGER_ARRAY[2]; // {Ultra_A, Ultra_B};

/*
 * This is the interrupt used to trigger the ultrasonic pulses.
 * If there is a timeout, the value of the distance is set to 0xFF.
 */
__interrupt void
cpuTimer0ISR(void)
{
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    uint8_t i = 0;
    // A = 0, B = 1
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
                copySerialData(&MAX_SONAR, i == 0 ? ultrasonicFront: ultrasonicBack);
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

/* 4084 counts ~= 20 uS => 1 uS = 204.2 counts (measured from oscilloscope)
*  distance cm = X uS/58 (from datasheet)
*
*  distance = counts/(counts/uS)/(uS/cm)
*           = counts/(204.2*58)
*           = counts/11843.6
*/

/*
 * NOTE: CPUTimer_getTimerCount counts downwards to 0 and then resets
 */

void gpioEchoTimerISR(uint8_t trig_num) {
    //
     // Acknowledge this interrupt to receive more interrupts from group 1
     //
     Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
     static uint32_t start_count[NUM_SONAR_SENSORS] = {0},
             end_count[NUM_SONAR_SENSORS] = {0};
     if (GPIO_readPin(Ultrasonic_B_ECHO_Pin)) {
         start_count[trig_num] = CPUTimer_getTimerCount(CPUTIMER1_BASE);
     } else {
         uint8_t distance = 0;
         uint32_t delta = 0;
         end_count[trig_num] = CPUTimer_getTimerCount(CPUTIMER1_BASE);
         // handle wrap-around case
         delta = start_count[trig_num] > end_count[trig_num] ? start_count[trig_num] - end_count[trig_num]: start_count[trig_num] + (0xFFFFFFFF - end_count[trig_num]);
         distance = (uint8_t)(delta/11843);
         copySerialData(&distance, 0 == trig_num ? ultrasonicFront : ultrasonicBack);
         TRIGGER_ARRAY[trig_num].trigger = true;
     }
}

/*
 * This is the interrupt used for the ultrasonic A (Front) Echo pulse
 */
__interrupt void
gpioEchoATimerISR(void)
{
    gpioEchoTimerISR(0);
}

/*
 * This is the interrupt used for the ultrasonic B (Back) Echo pulse
 */
__interrupt void
gpioEchoBTimerISR(void)
{
    gpioEchoTimerISR(1);
}

/*
 * Initialize the GPIO pins and functions used by the Ultrasonic Module(s)
 */
void UltraSonic_GPIO_Init() {
    Interrupt_register(INT_XINT1, &gpioEchoATimerISR);
    Interrupt_register(INT_XINT2, &gpioEchoBTimerISR);

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

/*
 * This function initializes the selected timer to the
 * period specified by the "freq" and "period" parameters. The "freq" is
 * entered as Hz and the period in uSeconds. The timer is held in the stopped
 * state after configuration.
 */
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

/*
 * Enable the timer used for measuring/triggering Ultrasonic signals
 */
void UltraSonic_TMR_Init() {

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

    //
    // Configure CPU-Timer 0 to interrupt every 10 uS
    // and 1, and 2 to interrupt every second:
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


