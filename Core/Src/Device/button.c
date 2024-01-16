/*
 * button.c
 *
 *  Created on: Jan 10, 2024
 *      Author: Admin
 */


#include "button.h"
#include "Task.h"
#include  "user_lcd1604.h"
#include "user_a4988.h"
#include "Step_motor.h"
#include "main.h"



static DISPLAY_MODE_t *__MY_DISPLAY_MODE;
static CONTROL_t curControl = CONTROL_STEP;
static SWITCH_t curSwitch = SW_ON;
static uint8_t volatile curMonitor = 0;

static uint8_t volatile clearflag = 0;

//static uint8_t clearPosX = 0;
//static uint8_t clearPosY = 0;
//static uint8_t clearPosflag = 0;

static STEP_t stepValorDir = STEP_VAL;

//, s_list *mylist, Station_t *myStation

void initButton(DISPLAY_MODE_t *displaymode)
{
	__MY_DISPLAY_MODE = displaymode;
//	myStation.ssNode_list = mylist;
//	myStation = myStation;
}

void setStepValorDir (STEP_t ValorDir)
{
	stepValorDir = ValorDir;
}

STEP_t getStepValorDir()
{
	return stepValorDir;
}

//void setClearPosflag()
//{
//	clearPosflag = 1;
//}
///* @func : getClearPosflag
// * @return : 1 flag is set
// * 			0 flag is not set
// */
//uint8_t getClearPosflag()
//{
//	return clearPosflag;
//}
///*
// * @func: getClearPos
// * @argument: XorY
// * @return:	XorY == 0 : Xpos
// * 			XorY == other Y
// *
// */
//uint8_t getClearPos(uint8_t XorY)
//{
//	if (XorY == 0 )	{
//		return clearPosX;
//	}
//	else {
//		return clearPosY;
//	}
//}

SWITCH_t getCurswitch()
{
	return curSwitch;
}
void setCurswitch(SWITCH_t ONorOFF)
{
	curSwitch = ONorOFF;
}


uint8_t getClearflag()
{
	return clearflag;
}

void setClearflag(uint8_t ENorDIS)
{
	clearflag = ENorDIS;
}


void switchcurControl()
{
	switch (curControl)	{
	case CONTROL_TRANS:
		curControl = CONTROL_STEP;
		break;
	case CONTROL_STEP:
		curControl = CONTROL_TRANS;
		break;
	default:
		break;
	}
}

CONTROL_t getcurControl()
{
	return curControl;
}

void nextMonitor(uint8_t volatile  * curMonitor, uint8_t max)	{
	if (*curMonitor >= max )		{
		*curMonitor = 0;
	}
	else {
		(*curMonitor)++;
	}
}

void previousMonitor(uint8_t volatile  * curMonitor, uint8_t max)	{
	if (*curMonitor == 0 )		{
		*curMonitor = max ;
	}
	else {
		(*curMonitor)--;
	}
}

uint8_t get_curMonitor(void)
{
	return curMonitor;
}
void setcurMonitor(uint8_t index)
{
	curMonitor = index;
}



void buttonMENU_handler()
{
	switch(*__MY_DISPLAY_MODE)	{
	case HOME:
		*__MY_DISPLAY_MODE = MONITOR;
		setcurMonitor(0);
		setClearflag(ENABLE);
		break;
	case COUNTDOWN_SW_OFF:
		*__MY_DISPLAY_MODE = MONITOR;
		setClearflag(ENABLE);
		break;
	case AFTER_SW_OFF:
		*__MY_DISPLAY_MODE = MONITOR;
		setClearflag(ENABLE);
		break;
	case MONITOR:
		*__MY_DISPLAY_MODE = MENU_CONTROL;
		setClearflag(ENABLE);
		break;
	case MENU_CONTROL:
		*__MY_DISPLAY_MODE = HOME;
		setClearflag(ENABLE);
		break;
	case VOLTAGE_CONTROL:
		*__MY_DISPLAY_MODE = HOME;
		setClearflag(ENABLE);
		break;
	case STEP_VAL_CONTROL:
		*__MY_DISPLAY_MODE = HOME;
		setClearflag(ENABLE);
		break;
	case STEP_DIR_CONTROL:
		*__MY_DISPLAY_MODE = HOME;
		setClearflag(ENABLE);
			break;
	case ON_OFF_CONTROL:
		*__MY_DISPLAY_MODE = HOME;
		setClearflag(ENABLE);
		break;
	case COMPLETE_CONTROL:
		*__MY_DISPLAY_MODE = HOME;
		setClearflag(ENABLE);
		break;
	default:
		break;
	}
}
void buttonOK_handler()
{
	switch(*__MY_DISPLAY_MODE)	{
	case HOME:
		break;
	case COUNTDOWN_SW_OFF:
		break;
	case AFTER_SW_OFF:
		break;
	case MONITOR:
		break;
	case MENU_CONTROL:
		if (getcurControl() == CONTROL_TRANS)	{
			*__MY_DISPLAY_MODE = ON_OFF_CONTROL;
			setClearflag(ENABLE);
		}
		else if (getcurControl() == CONTROL_STEP)	{
			*__MY_DISPLAY_MODE = VOLTAGE_CONTROL;
			setClearflag(ENABLE);
		}
		break;
	case VOLTAGE_CONTROL:
		switch (getStepValorDir())	{
		case STEP_VAL:
			*__MY_DISPLAY_MODE = STEP_VAL_CONTROL;
			break;
		case STEP_DIR:
			*__MY_DISPLAY_MODE = STEP_DIR_CONTROL;
			break;
		case STEP_CONFIRM:
			triggerTaskflag(TASK_CTRL_STEPMOR, FLAG_EN);
			break;
		default:
			break;
		}
		setClearflag(ENABLE);
		break;
	case STEP_VAL_CONTROL:
		*__MY_DISPLAY_MODE = VOLTAGE_CONTROL;
		setClearflag(ENABLE);
		break;
	case STEP_DIR_CONTROL:
		*__MY_DISPLAY_MODE = VOLTAGE_CONTROL;
		setClearflag(ENABLE);
		break;
	case ON_OFF_CONTROL:
		// switch MBA
		if(getCurswitch() == SW_ON)	{
			myStation.MBAstate = switchContactor(MBA_ON);


		}
		else {
			myStation.MBAstate = switchContactor(MBA_OFF);
		}

		setSwitchtime();
		*__MY_DISPLAY_MODE = COMPLETE_CONTROL;
		setClearflag(ENABLE);
		break;
	case COMPLETE_CONTROL:
		*__MY_DISPLAY_MODE = ON_OFF_CONTROL;
		setClearflag(ENABLE);
		break;
	default:
		break;
	}
}
void buttonDOWN_handler()
{
	switch(*__MY_DISPLAY_MODE)	{
	case HOME:
		break;
	case COUNTDOWN_SW_OFF:
		break;
	case AFTER_SW_OFF:
		break;
	case MONITOR:
		// move to next sensor ID
		nextMonitor(&curMonitor, myStation.ssNode_list->length);
		setClearflag(ENABLE);
		break;
	case MENU_CONTROL:
		switchcurControl();
		setClearflag(ENABLE);
		break;
	case VOLTAGE_CONTROL:
		switch(getStepValorDir())	{
		case STEP_VAL:
			setStepValorDir(STEP_DIR);
			break;
		case STEP_DIR:
			setStepValorDir(STEP_CONFIRM);
			break;
		case STEP_CONFIRM:
			setStepValorDir(STEP_VAL);
			break;
		default:
			break;
		}
		setClearflag(ENABLE);
		break;
	case STEP_VAL_CONTROL:
		// decrease step value
		decreaseStepchange();
		setClearflag(ENABLE);
		break;
	case STEP_DIR_CONTROL:
		Step_toggleDir();
		setClearflag(ENABLE);
		break;
	case ON_OFF_CONTROL:
		//  switch ON/OFF
		switch (curSwitch) {
		case SW_ON:
			curSwitch = SW_OFF;
			break;
		case SW_OFF:
			curSwitch = SW_ON;
			break;
		default:
			break;
		}
		setClearflag(ENABLE);
		break;
	case COMPLETE_CONTROL:
		break;
	default:
		break;
	}
}
void buttonUP_handler()
{
	switch(*__MY_DISPLAY_MODE)	{
	case HOME:
		break;
	case COUNTDOWN_SW_OFF:
		break;
	case AFTER_SW_OFF:
		break;
	case MONITOR:
		previousMonitor(&curMonitor, myStation.ssNode_list->length);
		setClearflag(ENABLE);
		break;
	case MENU_CONTROL:
		switchcurControl();
		setClearflag(ENABLE);
		break;
	case VOLTAGE_CONTROL:
		switch (getStepValorDir())	{
		case STEP_DIR:
			setStepValorDir(STEP_VAL);
			break;
		case STEP_VAL:
			setStepValorDir(STEP_CONFIRM);
			break;
		case STEP_CONFIRM:
			setStepValorDir(STEP_DIR);
			break;
		default:
			break;
		}
		setClearflag(ENABLE);
		break;
	case STEP_VAL_CONTROL:
		// increase step value
		increaseStepchange();
		setClearflag(ENABLE);
		break;
	case STEP_DIR_CONTROL:
		Step_toggleDir();
		break;
	case ON_OFF_CONTROL:
		switch (curSwitch) {
		case SW_ON:
			curSwitch = SW_OFF;
			break;
		case SW_OFF:
			curSwitch = SW_ON;
			break;
		default:
			break;
		}
		setClearflag(ENABLE);
		break;
	case COMPLETE_CONTROL:
		break;
	default:
		break;
	}
}
void SW_LIMIT_MAX_handler()
{
	setClearflag(ENABLE);
	A4988_DisableDriver();
	setLimit(LIMIT_MAX);
	triggerTaskflag(TASK_SEND_STEP_LIMIT, FLAG_EN);
}
void SW_LIMIT_MIN_handler()
{
	setClearflag(ENABLE);
	A4988_DisableDriver();
	setLimit(LIMIT_MIN);
	triggerTaskflag(TASK_SEND_STEP_LIMIT, FLAG_EN);
}
