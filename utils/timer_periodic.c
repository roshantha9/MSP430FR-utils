/*
 * timer_periodic.c
 *
 *  Created on: Aug 28, 2018
 *      Author: Rosh
 */


#include "driverlib.h"
#include <stdio.h>
#include <msp430.h>
#include "stdint.h"

#include "timer_periodic.h"


/*
 * Globals
 */

uint16_t compare_value = 0;

/*
 * Initialize Timer A
 * - Uses SMCLK
 * - period_ms : timer periodicity in miliseconds
 */
int TimerPeriodic_init(unsigned int period_ms, void (*CallBackArg)(void)){
    printf("TimerPeriodic_init::Enter\n");

    // calc the clock cycles for given period, return error if in error
    uint32_t clk_cycles = getClockCycles(period_ms);
    printf("TimerPeriodic_init::clk_cycles = %lu\n", clk_cycles);
    compare_value = (uint16_t)clk_cycles;

    if ((clk_cycles > TP_MAX_COMPARE) || (clk_cycles < TP_MIN_COMPARE)) {
        return 0;
    } else {
            //Start timer in continuous mode sourced by SMCLK
            Timer_A_initContinuousModeParam initContParam = {0};
            initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
            initContParam.clockSourceDivider = DIVIDER; //granularity
            initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
            initContParam.timerClear = TIMER_A_DO_CLEAR;
            initContParam.startTimer = false;
            Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

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

            // save callback function - used for ISR
            TimerPeriodic_Callback = CallBackArg;

            printf("TimerPeriodic_init::Exit\n");

            return 1;
    }
}

/*
 * Start the timer
 */
void TimerPeriodic_start(){
    printf("TimerPeriodic_start::Enter\n");
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);

    //Enter LPM0, enable interrupts
    //__bis_SR_register(LPM0_bits + GIE);
    __enable_interrupt();

    printf("TimerPeriodic_start::Exit\n");
}


void TimerPeriodic_stop(){
    Timer_A_stop(TIMER_A1_BASE);
    //__bic_SR_register(GIE); // disable interrupt
    __disable_interrupt();
}

#if TIMER_PERIODIC_ENABLE
#pragma vector=TIMER1_A0_VECTOR  // timer 0 interrupt vector
__interrupt void TimerPeriodic_ISR(void){ // interrupt function

    uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A1_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0)
                + compare_value;

    TimerPeriodic_Callback(); // execute the ISR

    //Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_0,
        compVal
        );
}
#endif


/*
 * get clock cycles
 * p : period (milliseconds)
 * clk : clock speed (Hz)
 */
uint32_t getClockCycles(uint16_t p){
    uint32_t curSMCLK = CS_getSMCLK();
    float T = (float) p / (float)TEN_POW_NEG_3; // in seconds
    uint32_t cc =  T * ((float)curSMCLK/(float)PRESCALER);
    return cc;
}

/*
 * get prescaler size (division)
 * p : period (milliseconds)
 * clk : clock speed (Hz)
 */
uint8_t getPrescaler(uint16_t p, uint32_t clk){
    // TODO: write a dynamic prescaler which decides on the division based on the period and clk
    return 1;
}



/*******************************************************
 * TESTBENCH: Timer_Periodic
 *******************************************************/
#if TB_TIMER
void testTimerPeriodic(){
    printf("testTimerPeriodic:: Enter\n");
    TimerPeriodic_init(1, &testTimerPeriodic_ISR);
    TimerPeriodic_start();
}

void testTimerPeriodic_ISR(){
    //printf("--testTimerPeriodic_ISR:: Enter\n");

    LED_OUT ^= LED1 + LED2; // Toggle the LEDs
    P6OUT ^= BIT0; // toggle port output

    //printf("--testTimerPeriodic_ISR:: Exit\n");
}
#endif



