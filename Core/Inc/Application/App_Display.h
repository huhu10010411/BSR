/*
 * App_Display.h
 *
 *  Created on: Jan 10, 2024
 *      Author: Admin
 */

#ifndef INC_APPLICATION_APP_DISPLAY_H_
#define INC_APPLICATION_APP_DISPLAY_H_

#include "ds3231.h"
#include "main.h"
typedef enum	{
	HOME,
	COUNTDOWN_SW_OFF,
	AFTER_SW_OFF,
	MONITOR,
	MENU_CONTROL,
	VOLTAGE_CONTROL,
	ON_OFF_CONTROL,
	COMPLETE_CONTROL
}DISPLAY_MODE_t;

typedef enum {
	CONTROL_STEP,
	CONTROL_TRANS

}CONTROL_t;

typedef enum {
	SW_OFF,
	SW_ON
}SWITCH_t;


void initApp_Display (DISPLAY_MODE_t *myDisplayMode, _RTC *myRTC, Station_t *myStation);

void processingApp_display();

void setSwitchtime();

_RTC getSwitchtime();
#endif /* INC_APPLICATION_APP_DISPLAY_H_ */
