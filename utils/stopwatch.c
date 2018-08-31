/*
 * stopwatch.c
 *
 *  Created on: Aug 28, 2018
 *      Author: Rosh
 */


#include "driverlib.h"
#include <stdio.h>
#include <msp430.h>
#include "stdint.h"

#include "stopwatch.h"


/*
 * Globals
 */

uint16_t timer_start_value = 0;

/*
 * Initialize Stopwatch (1ms granularity)
 * - Uses SMCLK (assume 1MHz)
 */
int Stopwatch_init(){
    DPRINTF("Stopwatch_init::Enter\n");

            //Start timer in continuous mode sourced by SMCLK
            Timer_A_initContinuousModeParam initContParam = {0};
            initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
            initContParam.clockSourceDivider = SW_DIVIDER; //granularity
            initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
            initContParam.timerClear = TIMER_A_DO_CLEAR;
            initContParam.startTimer = false;
            Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

            __disable_interrupt();

            DPRINTF("Stopwatch_init::Exit\n");

            return 1;

}



/*
 * Start/Stop the Timer
 */
void Stopwatch_start(){
    Timer_A_clear (TIMER_A1_BASE);
    timer_start_value = Timer_A_getCounterValue(TIMER_A1_BASE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
    //__bis_SR_register(LPM0_bits + GIE); //Enter LPM0, enable interrupts
    __enable_interrupt();
}

void Stopwatch_stop(){
    Timer_A_stop(TIMER_A1_BASE);
    //__bic_SR_register(GIE); // disable interrupt
    __disable_interrupt();
}

void Stopwatch_reset(){
    Stopwatch_stop();
}

/*
 * Calculate Elapsed time
 */
uint16_t Stopwatch_getElapsedClockCycles(){
    //return swObj;
    return Timer_A_getCounterValue(TIMER_A1_BASE) - timer_start_value;
}



uint16_t calcRealTime(uint16_t cc){
    __no_operation();
    return 1;
}





