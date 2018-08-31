/*
 * timer_periodic.h
 *
 *  Created on: Aug 28, 2018
 *      Author: Rosh
 */

#ifndef UTILS_TIMER_PERIODIC_H_
#define UTILS_TIMER_PERIODIC_H_

#include "stdint.h"
#include <msp430.h>


#define TIMER_PERIORIC_ENABLE        0

#define DIVIDER                       TIMER_A_CLOCKSOURCE_DIVIDER_8 // used in the timer config
#define PRESCALER                     8                             // used to calc compare value
#define TEN_POW_NEG_3                 1.0/1000.0
#define TP_MAX_COMPARE                (0xFFFF-1)
#define TP_MIN_COMPARE                0x0002


extern int TimerPeriodic_init(unsigned int period_ms, void (*CallBackArg)(void));
extern void TimerPeriodic_start(void);
extern void TimerPeriodic_stop(void);

void (*TimerPeriodic_Callback)(void);



uint32_t getClockCycles(uint16_t p);
uint8_t getPrescaler(uint16_t p, uint32_t clk);


#endif /* UTILS_TIMER_PERIODIC_H_ */
