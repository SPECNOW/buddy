/*
 * ultrasonic.h
 *
 *  Created on: Jan 29, 2019
 *      Author: Charanpreet
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

extern const uint8_t MAX_SONAR;

#define TRIGGER_TIMEOUT 6000    // TRIGGER_TIMEOUT/10 000 = Trigger Time Period in Seconds

void UltraSonic_GPIO_Init();
void UltraSonic_TMR_Init();

#endif /* ULTRASONIC_H_ */
