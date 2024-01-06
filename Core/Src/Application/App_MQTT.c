/*
 * App_MQTT.c
 *
 *  Created on: Dec 15, 2023
 *      Author: Admin
 */

#include "App_MQTT.h"
#include "SIM.h"
#include "MQTT.h"
#include "ServerMessage.h"
//#include "main.h"
#include "Serial_log.h"
#include "stationCMD.h"
#include <stdio.h>
#include "Task.h"

static Station_t *__MY_STATION_MQTT;
static SIM_t *__MY_SIM_MQTT;
uint8_t buffer [10];
uint8_t stIDbuffer[] = {1,2,3,4,5};
void initApp_MQTT(Station_t *station, SIM_t *sim)
{
	__MY_STATION_MQTT = station;
	__MY_SIM_MQTT = sim;
	init_MQTT(__MY_SIM_MQTT);
	initServerMsg(__MY_STATION_MQTT, &__MY_SIM_MQTT->sms);
	SIM_sendCMD((uint8_t*)"ATE0",(uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000);
}

uint8_t dataCalibavailble()
{
	Node * current =__MY_STATION_MQTT->ssNode_list->head->next;
		while (current != __MY_STATION_MQTT->ssNode_list->tail)	{
			if (current->SSnode.dataCalibAvailable) 	{
				return 1;
			}
			current = current->next;
		}
	return 0;
}
void processApp_MQTT(void)
{
	// Check for connection to MQTT broker
	if ( !MQTT_connected() ) {
		triggerTaskflag(TASK_SUBSCRIBE, FLAG_EN);
		MQTT_connect();
	}
	// Check for subscribe to broker
	if ( __MY_SIM_MQTT->mqttServer.connect && checkTaskflag(TASK_SUBSCRIBE) ) {

		if ( MQTT_subcribe( (uint8_t*)TOPIC_SUB ) ) {
			triggerTaskflag(TASK_SUBSCRIBE, FLAG_DIS);
		}
	}
	// Check for register to Server
	if ( checkTaskflag(TASK_REGISTER) )	{
		Register2Server();
	}
	// Send NETWORK READY message to Server
	if ( checkTaskflag(TASK_SEND_NWREADY))	{
		sendData2Server(DATA_NETWREADY);
	}
	// Send DATA CALIB
	if(checkTaskflag(TASK_SEND_DATACALIB) && dataCalibavailble())	{
		sendData2Server(DATA_CALIB);
	}
	// Send DATA AFTER CALIB
	if (checkTaskflag(TASK_SEND_DATAAFTERCALIB) )	{
		sendData2Server(DATA_AFTERCALIB);
	}

	// Send DATA PERIOD
	if (checkTaskflag(TASK_SEND_DATAPERIOD))	{
		sendData2Server(DATA_PERIOD);
	}
	// SMS control MBA
	if(checkTaskflag(TASK_SEND_CTRLMBA))	{
		if (checkSMSrequest(SMS_CMD_CTRL_ON) )	{
			CMD_SMS_Ctrl_MBA(__MY_SIM_MQTT->sms.CtrlON.data, __MY_SIM_MQTT->sms.CtrlON.datalength, MBA_ON);
			triggerSMSrequest(SMS_CMD_CTRL_ON, SMS_CMD_DISABLE);
		}
		else if (checkSMSrequest(SMS_CMD_CTRL_OFF))	{
			CMD_SMS_Ctrl_MBA(__MY_SIM_MQTT->sms.CtrlOFF.data, __MY_SIM_MQTT->sms.CtrlOFF.datalength, MBA_OFF);
			triggerSMSrequest(SMS_CMD_CTRL_OFF, SMS_CMD_DISABLE);
		}
		triggerTaskflag(TASK_SEND_CTRLMBA, FLAG_DIS);
	}
	// SMS control step motor
	if(checkTaskflag(TASK_SEND_CTRLSTEPMOR))	{
		if (checkSMSrequest(SMS_CMD_CTRL_INC) )	{
			CMD_SMS_Ctrl_StepM(__MY_SIM_MQTT->sms.CtrlINC.data, __MY_SIM_MQTT->sms.CtrlINC.datalength, STEPM_DIR_INC, STEPM_MODE_STEP, 1);
			triggerSMSrequest(SMS_CMD_CTRL_INC, SMS_CMD_DISABLE);
		}
		else if (checkSMSrequest(SMS_CMD_CTRL_DEC))	{
			CMD_SMS_Ctrl_StepM(__MY_SIM_MQTT->sms.CtrlDEC.data, __MY_SIM_MQTT->sms.CtrlDEC.datalength, STEPM_DIR_DEC, STEPM_MODE_STEP, 1);
			triggerSMSrequest(SMS_CMD_CTRL_DEC, SMS_CMD_DISABLE);
		}
		triggerTaskflag(TASK_SEND_CTRLSTEPMOR, FLAG_DIS);
	}
	// SMS calib
	if(checkTaskflag(TASK_SEND_CALIB))	{
		CMD_SMS_calib(__MY_SIM_MQTT->sms.CtrlCALIB.data, __MY_SIM_MQTT->sms.CtrlCALIB.datalength);
		triggerSMSrequest(SMS_CMD_CTRL_CALIB, SMS_CMD_DISABLE);
		triggerTaskflag(TASK_SEND_CALIB, FLAG_DIS);
	}
	// SMS get status
	if(checkTaskflag(TASK_SEND_GETSTATUS))	{
		CMD_SMS_getStatus(__MY_SIM_MQTT->sms.GetStatus.data, __MY_SIM_MQTT->sms.GetStatus.datalength);
		triggerSMSrequest(SMS_CMD_GET_STATUS, SMS_CMD_DISABLE);
		triggerTaskflag(TASK_SEND_GETSTATUS, FLAG_DIS);
	}
	//SMS get station latest data
	if(checkTaskflag(TASK_SEND_GETSTATION))	{
		CMD_SMS_getStationLatestData(__MY_SIM_MQTT->sms.GetStation.data, __MY_SIM_MQTT->sms.GetStation.datalength);
		triggerSMSrequest(SMS_CMD_GET_STATION, SMS_CMD_DISABLE);
		triggerTaskflag(TASK_SEND_GETSTATION, FLAG_DIS);
	}
	// SMS get sensor lastest data
	if(checkTaskflag(TASK_SEND_GETSENSOR))	{
		CMD_SMS_getSensorLatestData(__MY_SIM_MQTT->sms.GetSensor.data, __MY_SIM_MQTT->sms.GetSensor.datalength);
		triggerSMSrequest(SMS_CMD_GET_SENSOR, SMS_CMD_DISABLE);
		triggerTaskflag(TASK_SEND_GETSENSOR, FLAG_DIS);
	}
	// Processing new coming MQTT message
	if ( __MY_SIM_MQTT->mqttReceive.newEvent == 1)
	{
		__MY_SIM_MQTT->mqttReceive.newEvent = 0;
		// call process Server message function
		processingComingMsg(__MY_SIM_MQTT->mqttReceive.payload, __MY_SIM_MQTT->mqttReceive.payloadLen,
				__MY_STATION_MQTT->stID );
	}
}
