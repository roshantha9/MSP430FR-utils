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



#define SW_DIVIDER                    TIMER_A_CLOCKSOURCE_DIVIDER_1 // used in the timer config
#define SW_PRESCALER                  1                             // used to calc compare value
#define TEN_POW_NEG_3                 1.0/1000.0
#define TP_MAX_COMPARE                (0xFFFF-1)
#define TP_MIN_COMPARE                0x0002




typedef struct stopwatch {
    // timer register count
    uint16_t reg_count;

    // real time units
    uint16_t s;
    uint16_t ms;
    //uint16_t us; // 16MHz clock may not be fast enough;
} Stopwatch;



// public functions
extern int Stopwatch_init(void);
extern void Stopwatch_start(void);
extern void Stopwatch_stop(void);
extern uint16_t Stopwatch_getElapsedTime(void);


// private functions
uint32_t calcClockCycles(uint16_t ms, uint32_t clk, uint16_t p);
uint16_t calcRealTime(uint16_t cc);




/* debug printing */
#define STOPWATCH_DEBUG 1
#ifdef STOPWATCH_DEBUG
# define DPRINTF(fmt, args...) printf(fmt, ## args)
#else
# define DPRINTF(fmt, args...)
#endif


#endif /* UTILS_STOPWATCH_H_ */
