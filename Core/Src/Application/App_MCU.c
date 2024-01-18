/*
 * App_MCU.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */


#include "App_MCU.h"
#include "ds3231.h"
//#include "Serial_CFG.h"
#include "Task.h"
#include "main.h"
#include "Lora.h"
#include "ServerMessage.h"
#include "App_MQTT.h"

//static SIM_t *mySIM;


uint32_t tick = 0;
uint32_t NWRDtick = 0;
//uint8_t wake_sensorflag = 0 ;

void initApp_MCU()
{
//	mySIM = mySIM;
}

//uint8_t countNUMBofactiveSensor()
//{
//	uint8_t count = 0;
//	Node * current =myStation.ssNode_list->head->next;
//		while (current != myStation.ssNode_list->tail)	{
//			if (current->SSnode.Sensor_state == SENSOR_ACTIVE ) 	{
//				count++;
//			}
//			current = current->next;
//		}
//	return count;
//}
void processApp_MCU(void)
{
		/*Check for task*/
		if ( checkTaskflag(TASK_PREPARE_CALIB) )	{

			//Synchronize RTC time for Station and Sensor
			triggerTaskflag(TASK_GET_GPS_TIME, FLAG_EN);

			// Send WAKEUP command for Sensor
			Lora_Setmode(WAKE , 0);

			// This flag for check Sensor ready
			myStation.prepare_flag = 1;

			// Start timer 100s
			NWRDtick = HAL_GetTick();

			// Trigger send NETWORK READY message to Server
			triggerTaskflag(TASK_SEND_NWREADY, FLAG_EN);

			triggerTaskflag(TASK_PREPARE_CALIB, FLAG_DIS);
		}

		if ((HAL_GetTick() - NWRDtick > 100000) && checkTaskflag(TASK_SEND_NWREADY))	{
			sendData2Server(DATA_NETWREADY);
		}

		if( checkTaskflag(TASK_START_CALIB) ) {

			// Send DATA CALIBRATION to Server
			triggerTaskflag(TASK_SEND_DATACALIB, FLAG_EN);
			triggerTaskflag(TASK_START_CALIB, FLAG_DIS);
		}

		// Send data period
		if (HAL_GetTick() - tick >= UPDATE_DATA_PERIOD *1000)	{
			triggerTaskflag(TASK_SEND_DATAPERIOD, FLAG_EN);
			tick = HAL_GetTick();
		}

}
