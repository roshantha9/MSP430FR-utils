/*
 * button.h
 *
 *  Created on: Sep 4, 2018
 *      Author: Rosh
 */

#include "../common.h"

#ifndef UTILS_BUTTON_H_
#define UTILS_BUTTON_H_


void Button_S1_init(void (*CallBackArg)(void));
void Button_S2_init(void (*CallBackArg)(void));


void (*Button_S1_Callback)(void);
void (*Button_S2_Callback)(void);

#if TB_BUTTON
void testButton_S1(void);
void testButton_S1_ISR(void);
#endif


#endif /* UTILS_BUTTON_H_ */
