/*
 * button.c
 *
 *  Created on: Sep 4, 2018
 *      Author: Rosh
 */

#include <msp430.h>
#include <stdio.h>
#include "driverlib.h"

#include "button.h"

void Button_S1_init(void (*CallBackArg)(void)){

    printf("Button_S1_init::Enter\n");

    Button_S1_Callback = CallBackArg;                                                   // save callback function - used for ISR

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN6);                      // S1 P5.6: PxDIR, PxOUT and PxREN registers
    GPIO_selectInterruptEdge(GPIO_PORT_P5, GPIO_PIN6,GPIO_HIGH_TO_LOW_TRANSITION);      // S1 P5.6: PxIES register
    P5IFG = 0x00;
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN6);                                      // S1 P5.6: PxIE register

    __bis_SR_register(GIE);  // Enable all interrupts

    printf("Button_S1_init::Exit\n");

}

void Button_S2_init(void (*CallBackArg)(void)){

    Button_S2_Callback = CallBackArg;                                                   // save callback function - used for ISR

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);                      // S2 P5.5: PxDIR, PxOUT and PxREN registers
    GPIO_selectInterruptEdge(GPIO_PORT_P5, GPIO_PIN5,GPIO_HIGH_TO_LOW_TRANSITION);      // S2 P5.5: PxIES
    P5IFG = 0x00;
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN5);                                      // S2 P5.5: PxIE register

    __bis_SR_register(GIE);  // Enable all interrupts
}


/*
 * Button interrupt ISR
 */
#pragma vector=PORT5_VECTOR
__interrupt void Button_S1_ISR(void)
{
    //printf("Button_S1_ISR::Enter\n");

    switch (P5IFG){
        // S1 P5.6 = 64
        case 0b01000000:{

            Button_S1_Callback();       // execute the ISR
            P5IFG &= ~BIT6;             // P5.6 (S1) interrupt flag cleared
        }
        break;
        // S2 P5.5 = 32
        case 0b00100000:{

            Button_S2_Callback();       // execute the ISR
            P5IFG &= ~BIT5;             // P5.5 (S2) interrupt flag cleared
        }
        break;
        default: // should not be here!
        {
          printf("Button_S1_ISR:: ERROR !\n");
        }
        break;
    }
}



/*******************************************************
 * TESTBENCH: Button
 *******************************************************/
#if TB_BUTTON
void testButton_S1(){
    Button_S1_init(&testButton_S1_ISR);
}

void testButton_S1_ISR(){
    LED_OUT ^= LED1 + LED2; // Toggle the LEDs
    P6OUT ^= BIT0; // toggle port output
}
#endif
