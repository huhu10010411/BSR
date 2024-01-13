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
#include "MQTT.h"
#include "main.h"

static DISPLAY_MODE_t *__MY_DISPLAY_MODE;
static _RTC *__MY_RTC;
//static Station_t *myStation;

static uint16_t countdowntime;
static _RTC switchtime;

void initApp_Display (DISPLAY_MODE_t *myDisplayMode, _RTC *myRTC)
{
	LCD_Init();
	__MY_DISPLAY_MODE = myDisplayMode;
	__MY_RTC = myRTC;
//	myStation = myStation;
	Screen_Init(__MY_RTC);

}

void display_SensorX(uint8_t sensorIndex)
{
	if (sensorIndex > myStation.ssNode_list->length)	{
		Screen_Monitor_Station(myStation.stID, myStation.stCurrent);
		return ;

	}

	Node * current = myStation.ssNode_list->head;
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
			Screen_Home_Origin(myStation.stID, MQTT_getConnectflag());
			break;
		case COUNTDOWN_SW_OFF:
			countdowntime = getCountdowntime(myStation.calibTime.hour,
												myStation.calibTime.min, myStation.calibTime.sec);
			Screen_Home_Sync(countdowntime);
			break;
		case AFTER_SW_OFF:
			Screen_Home_Calib();
			break;
		case MONITOR:

			if (get_curMonitor() == 0) 	{
				Screen_Monitor_Station(myStation.MBAstate, myStation.stCurrent);
			}
			else {
				display_SensorX(get_curMonitor());
			}
			break;
		case MENU_CONTROL:
			Screen_Function(getcurControl());
			break;
		case VOLTAGE_CONTROL:
			Screen_Voltage_Control(getLimit(), getStepValorDir());
			Screen_Voltage_Control_Control_Motor(getStepchange(), Step_getDir());
			break;
		case STEP_DIR_CONTROL:
			Screen_Dir_Control(getLimit());
			Screen_Voltage_Control_Control_Motor(getStepchange(), Step_getDir());
			break;
		case STEP_VAL_CONTROL:
			Screen_StepVal_Control(getLimit());
			Screen_Voltage_Control_Control_Motor(getStepchange(), Step_getDir());
			break;
		case ON_OFF_CONTROL:
			Screen_Control_Relay(1);
			Screen_Control_Relay_Change_Mode(myStation.MBAstate, getCurswitch());
			break;
		case COMPLETE_CONTROL:
			Screen_Accept_Relay(getSwitchtime(), getCurswitch());
			break;
		default:
			break;
		}
//	Screen_Home_Origin(myStation.stID);

//	Screen_Home_Sync(countdowntime);
}

