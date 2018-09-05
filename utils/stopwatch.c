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


/**********************************************
 * Globals
 **********************************************/

uint16_t timer_start_value = 0;
uint32_t timer_elapsed_cc = 0;
uint16_t timer_overflow_cnt = 0;

/*********************************************
 * Initialize Stopwatch (1ms granularity)
 * - Uses SMCLK (assume 1MHz)
 *********************************************/
int Stopwatch_init(){
    DPRINTF("Stopwatch_init::Enter\n");

    // reset globals
    timer_start_value = 0;
    timer_elapsed_cc = 0;
    timer_overflow_cnt = 0;

    //Start timer in continuous mode sourced by SMCLK
    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider = SW_DIVIDER; //granularity
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_A_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_A_initContinuousMode(SW_TIMER_BASE, &initContParam);

    //Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(SW_TIMER_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0
                                        );

    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue = 0xFFFF; // interrupt when overflows
    Timer_A_initCompareMode(SW_TIMER_BASE, &initCompParam);

    DPRINTF("Stopwatch_init::Exit\n");

    return 1;
}



/********************************
 * Stopwatch control
 ********************************/
void Stopwatch_start(){
    //DPRINTF("Stopwatch_start::Enter\n");
    timer_start_value = Timer_A_getCounterValue(SW_TIMER_BASE);
    Timer_A_enableCaptureCompareInterrupt(SW_TIMER_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    __enable_interrupt();
    Timer_A_startCounter(SW_TIMER_BASE, TIMER_A_CONTINUOUS_MODE);
    //__bis_SR_register(LPM0_bits + GIE); //Enter LPM0, enable interrupts
    //DPRINTF("Stopwatch_start::Exit\n");
}

void Stopwatch_stop(){
    //DPRINTF("Stopwatch_stop::Enter\n");
    Timer_A_stop(SW_TIMER_BASE);
    Timer_A_disableInterrupt(SW_TIMER_BASE);
    //DPRINTF("Stopwatch_stop::Exit\n");
}

void Stopwatch_reset(){
    Timer_A_clear(SW_TIMER_BASE);
    timer_start_value = 0;
    timer_elapsed_cc = 0;
    timer_overflow_cnt = 0;
}


/************************************
 * Calculate Elapsed time
 ************************************/

// clock ticks
uint32_t Stopwatch_getElapsedClockCycles(){

    //printf("timer_overflow_cnt = %u\n", timer_overflow_cnt);
    //printf("Timer_A_getCounterValue() = %u\n", Timer_A_getCounterValue(SW_TIMER_BASE));
    //printf("Timer_A_getCounterValue() = %lu\n", (timer_overflow_cnt * (uint32_t)TIMER_MAX_COMPARE));

    if (timer_overflow_cnt > 0){

        timer_elapsed_cc = (timer_overflow_cnt * (uint32_t)TIMER_MAX_COMPARE) +
                                (Timer_A_getCounterValue(SW_TIMER_BASE)-(uint16_t)TICK_OVERHEAD);
    }else{
        timer_elapsed_cc = (Timer_A_getCounterValue(SW_TIMER_BASE) - TICK_OVERHEAD);
    }

    //printf("timer_elapsed_cc= %lu\n", timer_elapsed_cc);

    return timer_elapsed_cc;
}

/* TODO: needs fixing
// real time - in miliseconds
uint32_t Stopwatch_getElapsedRealTime(){
    uint32_t t =   (uint32_t)(
                                (float)Stopwatch_getElapsedClockCycles() /
                                (float)(CS_getSMCLK() / SW_PRESCALER)
                                ) * TEN_POW_SIX;
    return t;
}
*/

uint32_t Stopwatch_getClockFreq(){
    return CS_getSMCLK();
}



/************************************
 * Timer ISR - incr overflow count
 ************************************/
#if SW_ENABLE
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(SW_TIMER_VECTOR)))
#endif
void Stopwatch_overflow_ISR (void){
    timer_overflow_cnt++;
}
#endif




/*******************************************************
 * TESTBENCH: Stopwatch
 *******************************************************/
#if TB_STOPWATCH
void testStopwatch(){
    printf("testStopwatch:: Enter\n");
    Stopwatch_init();

    // short delay
    Stopwatch_start();
    __delay_cycles(50);
    Stopwatch_stop();
    printf("testStopwatch:: elapsed clock cycles: %lu\n", Stopwatch_getElapsedClockCycles());
    Stopwatch_reset();

    // med delay
    Stopwatch_start();
    __delay_cycles(5000);
    Stopwatch_stop();
    printf("testStopwatch:: elapsed clock cycles: %lu\n", Stopwatch_getElapsedClockCycles());
    Stopwatch_reset();

    // long delay
    Stopwatch_start();
    __delay_cycles(50000);
    Stopwatch_stop();
    printf("testStopwatch:: elapsed clock cycles: %lu\n", Stopwatch_getElapsedClockCycles());
    Stopwatch_reset();


    // very-long delay
    Stopwatch_start();
    __delay_cycles(100000);
    Stopwatch_stop();
    printf("testStopwatch:: elapsed clock cycles: %lu\n", Stopwatch_getElapsedClockCycles());
    Stopwatch_reset();


    // very-long delay
    Stopwatch_start();
    __delay_cycles(500000);
    Stopwatch_stop();
    printf("testStopwatch:: elapsed clock cycles: %lu\n", Stopwatch_getElapsedClockCycles());
    Stopwatch_reset();

    printf("testStopwatch:: Exit\n");
}
#endif

