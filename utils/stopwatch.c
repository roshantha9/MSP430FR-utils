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

//uint16_t compare_value = 0;
//Stopwatch swObj;

uint16_t timer_start_value = 0;

/*
 * Initialize Stopwatch (1ms granularity)
 * - Uses SMCLK (assume 1MHz)
 */
int Stopwatch_init(){
    DPRINTF("Stopwatch_init::Enter\n");

    // calc the clock cycles for given period, return error if in error
    //uint32_t clk_cycles = getClockCycles(1, ); // 1 ms timer
    //DPRINTF("Stopwatch_init::clk_cycles = %lu\n", clk_cycles);
    //compare_value = (uint16_t)clk_cycles;

    //if ((clk_cycles > TP_MAX_COMPARE) || (clk_cycles < TP_MIN_COMPARE)) {
    //    return 0;
    //} else {
            //Start timer in continuous mode sourced by SMCLK
            Timer_A_initContinuousModeParam initContParam = {0};
            initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
            initContParam.clockSourceDivider = SW_DIVIDER; //granularity
            initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
            initContParam.timerClear = TIMER_A_DO_CLEAR;
            initContParam.startTimer = false;
            Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

            __disable_interrupt();

            /*
            //Initiaze compare mode
            Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0
                );

            Timer_A_initCompareModeParam initCompParam = {0};
            initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
            initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
            initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
            initCompParam.compareValue = compare_value;
            Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);


            // clear stopwatch object
            swObj.ms=0;
            swObj.s=0;
            */




            DPRINTF("Stopwatch_init::Exit\n");

            return 1;
    //}
}



/*
 * Start/Stop the Timer
 */
void Stopwatch_start(){
    //DPRINTF("Stopwatch_start::Enter - start value = %d\n", timer_start_value);

    Timer_A_clear (TIMER_A1_BASE);
    timer_start_value = Timer_A_getCounterValue(TIMER_A1_BASE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
    //__bis_SR_register(LPM0_bits + GIE); //Enter LPM0, enable interrupts
    __enable_interrupt();
    //DPRINTF("Stopwatch_start::Exit\n");
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





