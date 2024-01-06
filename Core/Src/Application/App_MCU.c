/*
 * App_MCU.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */


#include "App_MCU.h"
#include "ds3231.h"
#include "Serial_CFG.h"
#include "Task.h"

static Station_t *__MY_STATION_MCU;
static SIM_t *__MY_SIM_MCU;


uint32_t tick = 0;

void initApp_MCU(Station_t *station, SIM_t *mySIM)
{
	__MY_STATION_MCU = station;
	__MY_SIM_MCU = mySIM;
}

void processApp_MCU(void)
{
		/*Check for task*/
		if ( checkTaskflag(TASK_PREPARE_CALIB) )	{

			//Synchronize RTC time for Station and Sensor
			triggerTaskflag(TASK_GET_GPS_TIME, FLAG_EN);

			// Send WAKEUP command for Sensor
	//		Lora_Setmode(WAKE);
			//Check for sensor Ready

			// Trigger send NETWORK READY message to Server
			triggerTaskflag(TASK_SEND_NWREADY, FLAG_EN);

			triggerTaskflag(TASK_PREPARE_CALIB, FLAG_DIS);
		}

		// Calibration Mode
		if (checkStationMode() == STATION_MODE_CALIB)	{
			if( checkTaskflag(TASK_START_CALIB) ) {
				// Receive Sensor DATA CALIBRATION

				// Send DATA CALIBRATION to Server
				triggerTaskflag(TASK_SEND_DATACALIB, FLAG_EN);
				triggerTaskflag(TASK_START_CALIB, FLAG_DIS);
			}

			if (checkTaskflag(TASK_CTRL_STEPMOR))	{
				// Control step motor for desire output

				// Send data after calibration
				triggerTaskflag(TASK_SEND_DATAAFTERCALIB, FLAG_EN);
				setStationMode(STATION_MODE_NORMAL);
				triggerTaskflag(TASK_CTRL_STEPMOR, FLAG_DIS);
			}
		}

	// Normal Mode
	if (checkStationMode() == STATION_MODE_NORMAL)	{

		// Send data period
		if (HAL_GetTick() - tick >= UPDATE_DATA_PERIOD *1000)	{
			triggerTaskflag(TASK_SEND_DATAPERIOD, FLAG_EN);
			tick = HAL_GetTick();
		}
	}



}
