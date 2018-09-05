/*
 * button.h
 *
 *  Created on: Sep 4, 2018
 *      Author: Rosh
 */

#ifndef UTILS_BUTTON_H_
#define UTILS_BUTTON_H_


void Button_S1_init(void (*CallBackArg)(void));
void Button_S2_init(void (*CallBackArg)(void));


void (*Button_S1_Callback)(void);
void (*Button_S2_Callback)(void);

#endif /* UTILS_BUTTON_H_ */
