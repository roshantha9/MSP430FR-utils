/*
 * config.h
 *
 *  Created on: Sep 5, 2018
 *      Author: Rosh
 */

#ifndef COMMON_H_
#define COMMON_H_





/* Testbench options */
#define TB_TIMER            0 // testbench timer
#define TB_STOPWATCH        0 // testbench stopwatch
#define TB_BUTTON           1


/* LED for MSP430FR5994 launchpad */
#define LED1        BIT0
#define LED2        BIT1
#define LED_OUT     P1OUT
#define LED_DIR     P1DIR


#endif /* COMMON_H_ */
