/*
 * App_MQTT.c
 *
 *  Created on: Dec 15, 2023
 *      Author: Admin
 */

#include "App_MQTT.h"
#include "SIM.h"
#include "MQTT.h"
//#include "ServerMessage.h"

#include "Serial_log.h"
#include "stationCMD.h"
#include <stdio.h>
#include "Task.h"

//static Station_t *myStation;
//static SIM_t *mySim;
//uint8_t buffer [10];
//uint8_t stIDbuffer[] = {1,2,3,4,5};
uint32_t tickconnect =0;
uint32_t tickreg = 0;
uint32_t ticksend = 0;
void initApp_MQTT()
{
//	myStation = station;
//	mySim = sim;
	init_MQTT();
	initServerMsg();
	SIM_sendCMD((uint8_t*)"ATE0",(uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000);
}

uint8_t dataCalibavailble()
{
	Node * current =myStation.ssNode_list->head->next;
		while (current != myStation.ssNode_list->tail)	{
			if (current->SSnode.dataCalibAvailable) 	{
				triggerTaskflag(TASK_SEND_DATACALIB, FLAG_EN);
				return 1;
			}
			current = current->next;
		}
	return 0;
}

uint8_t checkSensorState(sensor_state_t state)
{
	uint8_t count = 0;
	Node * current =myStation.ssNode_list->head->next;
		while (current != myStation.ssNode_list->tail)	{
			if (current->SSnode.Sensor_state == state ) 	{
				count++;
			}
			current = current->next;
		}
	return count;
}

uint8_t checkSensorMode(sensor_mode_t mode)
{
	uint8_t count = 0;
	Node * current =myStation.ssNode_list->head->next;
		while (current != myStation.ssNode_list->tail)	{
			if (current->SSnode.sensorMode == mode ) 	{
				count++;
			}
			current = current->next;
		}
	return count;
}
void sendNWready()
{
	if ( checkSensorMode(WAKE) + checkSensorState(SENSOR_DEACTIVE) >= myStation.ssNode_list->length  )	{
		sendData2Server(DATA_NETWREADY);
	}
}
void processApp_MQTT(void)
{
//	 Check for connection to MQTT broker
	if (HAL_GetTick() - tickconnect > 30000)	{
		MQTT_connected();
		if (!checkSubcribe((uint8_t*)TOPIC_SUB))	{
			triggerTaskflag(TASK_SUBSCRIBE, FLAG_EN);
		}
		tickconnect = HAL_GetTick();
	}
	if ( !mySIM.mqttServer.connect  ) {
			if (MQTT_connect())	{
				triggerTaskflag(TASK_SUBSCRIBE, FLAG_EN);
			}
		}
	if (mySIM.mqttServer.connect)	{

		// Check for subscribe to broker
		if (  checkTaskflag(TASK_SUBSCRIBE) ) {

			if ( MQTT_subcribe( (uint8_t*)TOPIC_SUB ) ) {
				triggerTaskflag(TASK_SUBSCRIBE, FLAG_DIS);
			}
		}
		// Check for register to Server
		if ( checkTaskflag(TASK_REGISTER) )	{
			if (HAL_GetTick() - tickreg > 10000)	{
				Register2Server();
				tickreg = HAL_GetTick();
			}
		}
		// Send NETWORK READY message to Server
		if ( checkTaskflag(TASK_SEND_NWREADY))	{
			sendNWready();
		}
		// Send DATA CALIB
		if(checkTaskflag(TASK_SEND_DATACALIB) && dataCalibavailble())	{
			sendData2Server(DATA_CALIB);
		}
		// Send DATA AFTER CALIB
		if (checkTaskflag(TASK_SEND_DATAAFTERCALIB) )	{
			triggerTaskflag(TASK_SEND_DATACALIB, FLAG_DIS);
			sendData2Server(DATA_AFTERCALIB);
		}

		// Send DATA PERIOD
		if (checkTaskflag(TASK_SEND_DATAPERIOD))	{
			if (HAL_GetTick() - ticksend > 2000)	{
				sendData2Server(DATA_PERIOD);
				ticksend = HAL_GetTick();
			}

		}
		// Send MBA status
		if (checkTaskflag(TASK_SEND_MBA_STATUS))	{
			sendData2Server(DATA_MBA_STATE);
			triggerTaskflag(TASK_SEND_MBA_STATUS, FLAG_DIS);
		}
		// Send step Reach Limit
		if (checkTaskflag(TASK_SEND_STEP_LIMIT))	{
			sendData2Server(DATA_STEP_REACH_LIMIT);
			triggerTaskflag(TASK_SEND_STEP_LIMIT, FLAG_DIS);
		}
		// SMS control MBA
		if(checkTaskflag(TASK_SEND_CTRLMBA))	{
			if (checkSMSrequest(SMS_CMD_CTRL_ON) )	{
				CMD_SMS_Ctrl_MBA(mySIM.sms.CtrlON.data, mySIM.sms.CtrlON.datalength, MBA_ON);
				triggerSMSrequest(SMS_CMD_CTRL_ON, SMS_CMD_DISABLE);
			}
			else if (checkSMSrequest(SMS_CMD_CTRL_OFF))	{
				CMD_SMS_Ctrl_MBA(mySIM.sms.CtrlOFF.data, mySIM.sms.CtrlOFF.datalength, MBA_OFF);
				triggerSMSrequest(SMS_CMD_CTRL_OFF, SMS_CMD_DISABLE);
			}
			triggerTaskflag(TASK_SEND_CTRLMBA, FLAG_DIS);
		}
		// SMS control step motor
		if(checkTaskflag(TASK_SEND_CTRLSTEPMOR))	{
			if (checkSMSrequest(SMS_CMD_CTRL_INC) )	{
				CMD_SMS_Ctrl_StepM(mySIM.sms.CtrlINC.data, mySIM.sms.CtrlINC.datalength, STEPM_DIR_INC, STEPM_MODE_STEP, 2);
				triggerSMSrequest(SMS_CMD_CTRL_INC, SMS_CMD_DISABLE);
			}
			else if (checkSMSrequest(SMS_CMD_CTRL_DEC))	{
				CMD_SMS_Ctrl_StepM(mySIM.sms.CtrlDEC.data, mySIM.sms.CtrlDEC.datalength, STEPM_DIR_DEC, STEPM_MODE_STEP, 2);
				triggerSMSrequest(SMS_CMD_CTRL_DEC, SMS_CMD_DISABLE);
			}
			triggerTaskflag(TASK_SEND_CTRLSTEPMOR, FLAG_DIS);
		}
		// SMS calib
		if(checkTaskflag(TASK_SEND_CALIB))	{
			CMD_SMS_calib(mySIM.sms.CtrlCALIB.data, mySIM.sms.CtrlCALIB.datalength);
			triggerSMSrequest(SMS_CMD_CTRL_CALIB, SMS_CMD_DISABLE);
			triggerTaskflag(TASK_SEND_CALIB, FLAG_DIS);
		}
		// SMS get status
		if(checkTaskflag(TASK_SEND_GETSTATUS))	{
			CMD_SMS_getStatus(mySIM.sms.GetStatus.data, mySIM.sms.GetStatus.datalength);
			triggerSMSrequest(SMS_CMD_GET_STATUS, SMS_CMD_DISABLE);
			triggerTaskflag(TASK_SEND_GETSTATUS, FLAG_DIS);
		}
		//SMS get station latest data
		if(checkTaskflag(TASK_SEND_GETSTATION))	{
			CMD_SMS_getStationLatestData(mySIM.sms.GetStation.data, mySIM.sms.GetStation.datalength);
			triggerSMSrequest(SMS_CMD_GET_STATION, SMS_CMD_DISABLE);
			triggerTaskflag(TASK_SEND_GETSTATION, FLAG_DIS);
		}
		// SMS get sensor lastest data
		if(checkTaskflag(TASK_SEND_GETSENSOR))	{
			CMD_SMS_getSensorLatestData(mySIM.sms.GetSensor.data, mySIM.sms.GetSensor.datalength);
			triggerSMSrequest(SMS_CMD_GET_SENSOR, SMS_CMD_DISABLE);
			triggerTaskflag(TASK_SEND_GETSENSOR, FLAG_DIS);
		}
	}
	// Processing new coming MQTT message
	if ( mySIM.mqttReceive.newEvent == 1)
	{
		mySIM.mqttReceive.newEvent = 0;
		// call process Server message function
		processingComingMsg(mySIM.mqttReceive.payload, mySIM.mqttReceive.payloadLen,
				myStation.stID );
	}
}
