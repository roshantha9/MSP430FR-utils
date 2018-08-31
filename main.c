#include <stdint.h>
#include <stdio.h>
#include <msp430.h> 
#include "driverlib.h"


// test bench options
#define TB_TIMER            0 // testbench timer
#define TB_STOPWATCH        1 // testbench stopwatch

#if TB_TIMER
    #include "utils/timer_periodic.h"
#endif

#if TB_STOPWATCH
    #include "utils/stopwatch.h"
#endif


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
#if TB_TIMER
void testTimerPeriodic(void);
void testTimerPeriodic_ISR(void);
#endif

void testStopwatch(void);

void boardSetup(void);
void debugLEDSetup(void);
void outputClocksToPins(void);

void clockSetup(void);

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
