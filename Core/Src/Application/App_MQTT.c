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
#include "main.h"
#include "Serial_log.h"
#include "stationCMD.h"

void initApp_MQTT(void)
{
//	initSIM(__SIM_UART, __SIM_DMA_UART, __MY_SIM);
	init_MQTT(&mySIM);

	SIM_sendCMD((uint8_t*)"ATE0",(uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000);
}

void processApp_MQTT(void)
{
	// Check for connection to MQTT broker
	if ( !MQTT_connected() ) {
		myStation.subcribeStatus = 0;
		MQTT_connect();
	}

//	if ( mySIM.mqttServer.connect && !myStation.subcribeStatus ) {
//
//		if ( MQTT_subcribe( (uint8_t*)TOPIC_SUB ) ) {
//			myStation.subcribeStatus = 1 ;
//		}
//
//	}

	Register2Server(myStation.stID, myStation.stCurrent, myStation.stVoltage, SSnode_list);

//	CMD_SMS_getStatus(myStation.stID, (uint8_t*)"0123456789");
//	MQTT_publish( (uint8_t*)TOPIC_PUB, (uint8_t*)"hello bsr\r\n", 11);

	if ( mySIM.mqttReceive.newEvent == 1)
	{
		mySIM.mqttReceive.newEvent = 0;
		// call process Server message function
		Serial_log_string("Received\r\n");
		Serial_log_string("Topic: ");
		Serial_log_buffer(mySIM.mqttReceive.topic, mySIM.mqttReceive.topicLen);
		Serial_log_string("\r\nPayload: ");
		Serial_log_buffer(mySIM.mqttReceive.payload, mySIM.mqttReceive.payloadLen);
	}
}
