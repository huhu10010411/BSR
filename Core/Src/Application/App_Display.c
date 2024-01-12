/*
 * App_Display.c
 *
 *  Created on: Jan 10, 2024
 *      Author: Admin
 */


#include "user_screen.h"
#include "App_Display.h"
#include "button.h"
#include "Step_motor.h"

static DISPLAY_MODE_t *__MY_DISPLAY_MODE;
static _RTC *__MY_RTC;
static Station_t *__MY_STATION;

static uint16_t countdowntime;
static _RTC switchtime;

void initApp_Display (DISPLAY_MODE_t *myDisplayMode, _RTC *myRTC, Station_t *myStation)
{
	__MY_DISPLAY_MODE = myDisplayMode;
	__MY_RTC = myRTC;
	__MY_STATION = myStation;
	Screen_Init(__MY_RTC);
}

void display_SensorX(uint8_t sensorIndex)
{
	if (sensorIndex > __MY_STATION->ssNode_list->length)	{
		Screen_Monitor_Station(__MY_STATION->stID, __MY_STATION->stCurrent);
		return ;
	}

	Node * current = __MY_STATION->ssNode_list->head;
	for (uint8_t i = 0; i < sensorIndex; i++)
	{
		current = current->next;
	}
	Screen_Monitor_Node(current->SSnode.SSnode_ID, current->SSnode.Sensor_state,
			current->SSnode.Battery, current->SSnode.V_type, current->SSnode.V_value);

}

void setSwitchtime()
{
	DS3231_GetTime(&switchtime);
}
_RTC getSwitchtime()
{
	return switchtime;
}
void processingApp_display()
{

	if (getClearflag()) 	{
		LCD_Clear();
		setClearflag(DISABLE);
	}
	switch(*__MY_DISPLAY_MODE)	{
		case HOME:
			Screen_Home_Origin(__MY_STATION->stID);
			break;
		case COUNTDOWN_SW_OFF:
			countdowntime = getCountdowntime(__MY_STATION->calibTime.hour,
												__MY_STATION->calibTime.min, __MY_STATION->calibTime.sec);
			Screen_Home_Sync(countdowntime);
			break;
		case AFTER_SW_OFF:
			Screen_Home_Calib();
			break;
		case MONITOR:

			if (get_curMonitor() == 0) 	{
				Screen_Monitor_Station(__MY_STATION->MBAstate, __MY_STATION->stCurrent);
			}
			else {
				display_SensorX(get_curMonitor());
			}
			break;
		case MENU_CONTROL:
			Screen_Function(getcurControl());
			break;
		case VOLTAGE_CONTROL:
			Screen_Voltage_Control(getLimit());
			Screen_Voltage_Control_Control_Motor(getStepchange());
			break;
		case ON_OFF_CONTROL:
			Screen_Control_Relay(1);
			Screen_Control_Relay_Change_Mode(getCurswitch());
			break;
		case COMPLETE_CONTROL:
			Screen_Accept_Relay(getSwitchtime(), getCurswitch());
			break;
		default:
			break;
		}
//	Screen_Home_Origin(__MY_STATION->stID);

//	Screen_Home_Sync(countdowntime);
}

