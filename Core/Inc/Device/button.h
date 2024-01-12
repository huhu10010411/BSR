/*
 * button.h
 *
 *  Created on: Jan 10, 2024
 *      Author: Admin
 */

#ifndef INC_DEVICE_BUTTON_H_
#define INC_DEVICE_BUTTON_H_

#include "App_Display.h"
#include "linkedlist.h"



void initButton(DISPLAY_MODE_t *displaymode, s_list *mylist, uint16_t *myStepposition);

void buttonMENU_handler();

void buttonOK_handler();

void buttonDOWN_handler();

void buttonUP_handler();

void SW_LIMIT_MAX_handler();

void SW_LIMIT_MIN_handler();

uint8_t get_curMonitor(void);



void setClearflag(uint8_t ENorDIS);

uint8_t getClearflag();

CONTROL_t getcurControl();



SWITCH_t getCurswitch();

void setCurswitch(SWITCH_t ONorOFF);



#endif /* INC_DEVICE_BUTTON_H_ */
