#include <common.h>
#include <stdint.h>
#include <stdio.h>
#include <msp430.h> 
#include "driverlib.h"




#if TB_TIMER
    #include "utils/timer_periodic.h"
#endif

#if TB_STOPWATCH
    #include "utils/stopwatch.h"
#endif


#if TB_BUTTON
    #include "utils/button.h"
#endif


/*******************************************************
 * DEFINES
 *******************************************************/





/*******************************************************
 * Globals
 *******************************************************/
int cnt=0;


/*******************************************************
 * FUNC DEFS
 *******************************************************/





void boardSetup(void);
void debugLEDSetup(void);
void outputClocksToPins(void);

void clockSetup(void);










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





/*******************************************************
 * MAIN
 *******************************************************/
int main(void)
{
	
    printf("-- START --\n");

    /* setup sequence */
    boardSetup();
    clockSetup();
    debugLEDSetup();
    outputClocksToPins();


    //testTimerPeriodic(); // test the periodic timer module
    //testStopwatch();      // test the stopwatch module
    testButton_S1();


    /* Disable the GPIO power-on default high-impedance mode. */
    PMM_unlockLPM5();


	//For debugger
	while(1){
	    __no_operation();
	}



	printf("-- END --\n");

	return 0;
}
