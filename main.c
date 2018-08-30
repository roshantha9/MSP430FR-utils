#include <stdint.h>
#include <stdio.h>
#include <msp430.h> 
#include "driverlib.h"

#include "utils/timer_periodic.h"
#include "utils/stopwatch.h"

/*******************************************************
 * DEFINES
 *******************************************************/
#define LED1        BIT0
#define LED2        BIT1
#define LED_OUT     P1OUT
#define LED_DIR     P1DIR


/*******************************************************
 * Globals
 *******************************************************/
int cnt=0;


/*******************************************************
 * FUNC DEFS
 *******************************************************/
void testTimerPeriodic(void);
void testTimerPeriodic_ISR(void);
void testStopwatch(void);

void boardSetup(void);
void debugLEDSetup(void);
void outputClocksToPins(void);

void clockSetup(void);

/*******************************************************
 * TESTBENCH: Timer_Periodic
 *******************************************************/
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

void testStopwatch(){
    printf("testStopwatch:: Enter\n");
    //Stopwatch_init();
    //Stopwatch_start();

    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1; //granularity
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_A_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

    //__disable_interrupt();

    uint16_t timer_start_value = Timer_A_getCounterValue(TIMER_A1_BASE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);

    __delay_cycles(50000);

    Timer_A_stop(TIMER_A1_BASE);
    uint16_t elapsed_time = Timer_A_getCounterValue(TIMER_A1_BASE) - timer_start_value;


    //Stopwatch_stop();
    //printf("testStopwatch:: elapsed time: %d\n", Stopwatch_getElapsedTime());
    printf("testStopwatch:: elapsed time: %d\n", elapsed_time);

    printf("testStopwatch:: Exit\n");
}



/*******************************************************
 * SETUP: board, debug LED, clocks, etc.
 *******************************************************/
void boardSetup(){

    WDTCTL = WDTPW | WDTHOLD;               // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
}


void clockSetup(){

    //Set DCO Frequency to 8MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_6);

    //configure MCLK, SMCLK to be source by DCOCLK
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_8);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_8);

    /*
    // set master clock frequency to 8MHz
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);
    // set smclk
    CS_initClockSignal( CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    */

    // report clock freqs
    printf("SMLK= %lu ; MCLK= %lu ; ACLK=%lu \n", CS_getSMCLK(), CS_getMCLK(), CS_getACLK());
}



void debugLEDSetup(){
    P1SEL0 &= ~(LED1 + LED2); // Select I/O direction for P1.0 and P1.1
    P1SEL1 &= ~(LED1 + LED2);
    LED_DIR |= LED1 + LED2; // Set P1.0 and P1.1 to output direction
    LED_OUT &= ~(LED1 + LED2); // Clear the LEDs


    P6SEL0 &= ~BIT0; // select I/O direction for P6.0
    P6DIR |= BIT0;   // Set P6.0 to output direction
    P6OUT &= ~BIT0;  // Clear port

}


// Output Clocks onto the board for debugging
void outputClocksToPins(){

    /* output SMCLK to Pin P3.4 */
    P3DIR |= BIT4;
    P3SEL0 |= BIT4;
    P3SEL1 |= BIT4;
    P3OUT &= ~BIT4;

    /* output MCLK */

    /* output ACLK */
}





/**
 * main.c
 */
int main(void)
{
	
    printf("-- START --\n");

    /* setup sequence */
    boardSetup();
    clockSetup();
    debugLEDSetup();
    outputClocksToPins();


    //testTimerPeriodic(); // test the periodic timer

    testStopwatch();



	//For debugger
	while(1){
	    __no_operation();
	}



	printf("-- END --\n");

	return 0;
}
