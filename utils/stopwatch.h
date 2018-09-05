/*
 * stopwatch.h
 *
 *  Created on: Aug 28, 2018
 *      Author: Rosh
 */

#ifndef UTILS_STOPWATCH_H_
#define UTILS_STOPWATCH_H_

#include "stdint.h"
#include <msp430.h>
#include "../common.h"

#define SW_ENABLE              1

// you can use the divider/prescaler to control the granularity of the stopwatch
#define SW_DIVIDER                    TIMER_A_CLOCKSOURCE_DIVIDER_1 // used in the timer config
#define SW_PRESCALER                  1                             // used to calc compare value

#define TIMER_MAX_COMPARE             0xFFFF

#define TEN_POW_NEG_3                 1.0/1000.0
#define TEN_POW_SIX                   1000000


#define TICK_OVERHEAD                 0 // function call overhead (assuming 1MHz SMCLK)

#define SW_TIMER_BASE                 TIMER_A1_BASE
#define SW_TIMER_VECTOR               TIMER1_A0_VECTOR

// public functions
extern int Stopwatch_init(void);
extern void Stopwatch_start(void);
extern void Stopwatch_stop(void);
extern void Stopwatch_reset(void);
extern uint32_t Stopwatch_getElapsedClockCycles(void);
//extern uint32_t Stopwatch_getElapsedRealTime(void); // in ms

uint32_t Stopwatch_getClockFreq(void);

// testbench
#if TB_STOPWATCH
void testStopwatch(void);
#endif



/* debug printing */
#define STOPWATCH_DEBUG 1
#ifdef STOPWATCH_DEBUG
# define DPRINTF(fmt, args...) printf(fmt, ## args)
#else
# define DPRINTF(fmt, args...)
#endif


#endif /* UTILS_STOPWATCH_H_ */
