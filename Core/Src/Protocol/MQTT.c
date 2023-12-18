/*
 * SIM_MQTT.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Admin
 */

#include "MQTT.h"
#include "SIM.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Serial_log.h"

#define MQTT_TXBUFF_SIZE   1024

SIM_t *__MY_SIM;

uint8_t MQTT_Txbuff [MQTT_TXBUFF_SIZE];

uint16_t getRemainsize(uint8_t *Remainbuff, uint8_t *Originbuff, uint16_t originSize)
{
	return originSize-(Remainbuff - Originbuff)/sizeof(uint8_t);
}
uint8_t SumofOccurances(uint8_t * word, uint8_t *buffer, uint16_t buffer_size)
{
	uint8_t sum =0;
	uint8_t wordlen = strlen( (char*)word );
	uint8_t *p = isWordinBuff(buffer, buffer_size, word);
	while (p != NULL)
	{
		sum++;
		uint16_t remain_size = getRemainsize(p, buffer, buffer_size);
		p = isWordinBuff(buffer+wordlen, remain_size, word);
	}
	return sum;

}
uint8_t getBetween(uint8_t *firstWord,uint8_t *lastWord, uint8_t *buff, uint16_t size, uint8_t *getBuff)
{
	uint8_t fWlen = strlen((char*)firstWord);

	uint8_t *Fp =isWordinBuff(buff, size, firstWord);

	if (Fp == NULL ) return 0;
	Fp += fWlen;
	uint8_t remainSize = getRemainsize(Fp, buff, size);
	uint8_t *Lp = isWordinBuff(Fp, remainSize, lastWord);
	if (Lp == NULL || Fp >= Lp) return 0;

	uint8_t getBuffindex =0;
	while (Fp != Lp)
	{
		getBuff[getBuffindex++] = *Fp;
		Fp ++;
	}
	getBuff[getBuffindex++]= '\0';
	return getBuffindex;           // length of getBuff
}

uint8_t getContent(content_t contentType, uint8_t *databuffer, uint16_t datalen, uint8_t *getbuffer, uint16_t contentlen)
{
	uint16_t gotcontent_len = 0;

	uint8_t *currentPOS ;
	uint16_t databuffer_Remainlen = datalen;

	uint16_t subcontent_len;
	uint8_t subContentlen_buff[10];
	uint8_t contentsignature[20];
	if (contentType == CONTENT_PAYLOAD) {
		strcpy( (char*)contentsignature, "+CMQTTRXPAYLOAD:");
 	}
	else if (contentType == CONTENT_TOPIC) {
		strcpy( (char*)contentsignature, "+CMQTTRXTOPIC:");
	}
	else return 0;

	while ( gotcontent_len < contentlen )
	{
		currentPOS = isWordinBuff(databuffer, databuffer_Remainlen, contentsignature);
		if (currentPOS == NULL)		return  0;
		databuffer_Remainlen = getRemainsize( currentPOS, databuffer, databuffer_Remainlen );

		// Get subcontent length
		getBetween( (uint8_t*)",", (uint8_t*) "\r", currentPOS, databuffer_Remainlen, subContentlen_buff);
		subcontent_len = atoi( (char*)subContentlen_buff );

		// Get subcontent
		while ( *currentPOS != '\n'){
			currentPOS++;
		}
		currentPOS++;
		for (uint16_t i = 0; i< subcontent_len; i++)
		{
			getbuffer[gotcontent_len++] = currentPOS[i];
		}

		// Update databuffer
		databuffer = ++currentPOS;
	}
	return 1;
}

uint8_t MQTT_receiveGetTopicandPayloadLength( uint8_t *MQTTbuff, uint16_t buffsize)
{
	uint8_t topiclen_buff[10];
	uint8_t payloadlen_buff[10];
	Serial_log_string("MQTTbuffer: ");
	Serial_log_buffer(MQTTbuff, buffsize);
	Serial_log_string(" ");
	uint8_t * currentPos = isWordinBuff(MQTTbuff, buffsize, (uint8_t*)"+CMQTTRXSTART:");

	uint16_t remainlen = getRemainsize(currentPos, MQTTbuff, buffsize);

	getBetween( (uint8_t*)",", (uint8_t*)",", currentPos, remainlen, topiclen_buff);
	__MY_SIM->mqttReceive.topicLen = atoi( (char*)topiclen_buff );

	currentPos = isWordinBuff(currentPos, remainlen,(uint8_t*)",");
	currentPos ++;

	getBetween( (uint8_t*)",", (uint8_t*)"\r", currentPos, remainlen, payloadlen_buff);
	__MY_SIM->mqttReceive.payloadLen = atoi( (char*)payloadlen_buff );

	return 1;
}

uint8_t MQTT_receiveGetTopic (uint8_t *MQTTbuff,uint16_t buffsize)
{
	return getContent(CONTENT_TOPIC, MQTTbuff, buffsize, __MY_SIM->mqttReceive.topic, __MY_SIM->mqttReceive.topicLen);
}

uint8_t MQTT_receiveGetPayload (uint8_t *MQTTbuff,uint16_t buffsize)
{
	return getContent(CONTENT_PAYLOAD, MQTTbuff, buffsize, __MY_SIM->mqttReceive.payload, __MY_SIM->mqttReceive.payloadLen);
}
uint8_t MQTT_receive(uint8_t *MQTTbuff,uint16_t buffsize)
{
	if ( !MQTT_receiveGetTopicandPayloadLength(MQTTbuff, buffsize) ) return 0;

	memset( __MY_SIM->mqttReceive.payload, 0, MAX_PAYLOAD_LEN );
	memset( __MY_SIM->mqttReceive.topic, 0, MAX_TOPIC_LEN );

	if (! MQTT_receiveGetTopic(MQTTbuff, buffsize))		return 0;

	if (! MQTT_receiveGetPayload(MQTTbuff, buffsize))	return 0;

	__MY_SIM->mqttReceive.newEvent = 1;
	return 1;
}

uint8_t MQTT_connected(void)
{
	if ( SIM_sendCMD((uint8_t*)"AT+CMQTTDISC?", (uint8_t*)"+CMQTTDISC: 0,0", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000) == SIM_RES_MSG )
	{
		__MY_SIM->mqttServer.connect = 1;
		Serial_log_string("MQTT is connected to server\r\n");
		return 1;
	}
	Serial_log_string("MQTT is disconnected to server\r\n");
	__MY_SIM->mqttServer.connect = 0;
	return 0;
}

//uint8_t networkAvailable() {
//    // Check network availability
//    // Implement your network check logic here
//    return 1; // Replace with your logic
//}
//
uint8_t startMQTT(void) {
    // Implement starting MQTT logic
	// Return 1 on success, 0 on failure

	uint8_t check = SIM_sendCMD((uint8_t*)"AT+CMQTTSTART", (uint8_t*)"+CMQTTSTART: 0", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000);
	if ( check == SIM_RES_MSG )	{
		Serial_log_string("start MQTT success\r\n");
		return 1;
	}

	check = SIM_sendCMD((uint8_t*)"AT+CMQTTSTART", (uint8_t*)"ERROR", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000);
	if ( check == SIM_RES_MSG )	{
		Serial_log_string("start MQTT success\r\n");
		return 1;
	}
	Serial_log_number(check);
	Serial_log_string("\r\nstart MQTT fail\r\n");
	return 0;
}

uint8_t acquireMQTTclient (uint8_t *clientID){
	sprintf((char*)MQTT_Txbuff,"AT+CMQTTACCQ=0,\"%s\"", clientID);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000)== SIM_RES_MSG)	{
		Serial_log_string("acquire MQTT client success\r\n");
		return 1;
	}
	Serial_log_string("acquire MQTT client fail\r\n");
    return 0;
}
uint8_t releaseMQTTclient (void)
{
	if (SIM_sendCMD((uint8_t*)"AT+CMQTTREL=0", (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)) {
		Serial_log_string("release MQTT client success\r\n");
		return 1;
	}
	Serial_log_string("release MQTT client fail\r\n");
	return 0;
}
uint8_t configureMQTT() {
    // Implement MQTT configuration logic
    // Return 1 on success, 0 on failure
	uint8_t wtplen = strlen(__MY_SIM->mqttServer.willtopic);
	sprintf((char*)MQTT_Txbuff,"AT+CMQTTWILLTOPIC=0,%d",wtplen);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)'>', ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000) != SIM_RES_MSG)	return 0;

	sprintf((char*)MQTT_Txbuff,"%s",__MY_SIM->mqttServer.willtopic);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000) != SIM_RES_MSG)	return 0;

	uint8_t wmsglen = strlen((char*)__MY_SIM->mqttServer.willmsg);
	sprintf((char*)MQTT_Txbuff,"AT+CMQTTWILLMSG=0,%d,1",wmsglen);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)'>', ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)	return 0;

	sprintf((char*)MQTT_Txbuff,"%s",__MY_SIM->mqttServer.willmsg);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)	return 0;

	sprintf((char*)MQTT_Txbuff,"AT+CMQTTCFG=\"checkUTF8\",0,0");
	if (! SIM_sendCMD(MQTT_Txbuff, (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000))	return 0;

	Serial_log_string("config MQTT success\r\n");
    return 1;
}

uint8_t sendConnectMessage() {
    // Implement sending the CONNECT message
    // Return 1 on success, 0 on failure
	sprintf((char*)MQTT_Txbuff,"AT+CMQTTCONNECT=0,\"%s:%d\",%d,1",__MY_SIM->mqttServer.host,__MY_SIM->mqttServer.port,__MY_SIM->mqttClient.keepAliveInterval);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)"+CMQTTCONNECT: 0,0", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000) == SIM_RES_MSG)
	{
		Serial_log_string("Connect to broker success\r\n");
		return 1;
	}
	Serial_log_string("Connect to broker fail\r\n");
    return 0;
}
void init_MQTT(SIM_t *SIM)
{
	__MY_SIM = SIM;
}

uint8_t MQTT_checkNWavailable (void)
{
	if ( !SIM_checkCMD(SIM_CMD_SIMCARD_PIN) ) {

		return 0;
	}

	if ( !SIM_checkCMD(SIM_CMD_STA_CSQ) ) {
		return 0;
	}

	if ( !SIM_checkCMD(SIM_CMD_NW_CREG) ) {
		return 0;
	}

	if ( !SIM_checkCMD(SIM_CMD_NW_CPSI) ) {
		return 0;
	}
	return 1;
}
uint8_t MQTT_connect()
{
	if ( !MQTT_checkNWavailable() )  return 0;

	if ( !startMQTT() )		return 0;

	if ( !acquireMQTTclient( (uint8_t*)__MY_SIM->mqttClient.clientID) )
	{
		if (MQTT_connected())
		{
			MQTT_disconnect();
		}
		else
		{
			releaseMQTTclient();
		}
		return 0;
	}
	if ( !(configureMQTT()) )		return 0;

	if ( !sendConnectMessage() )	return 0;

	__MY_SIM->mqttServer.connect = 1;
	return 1;

}

uint8_t MQTT_disconnect ()
{
	if (SIM_sendCMD((uint8_t*)"AT+CMQTTDISC=0,120", (uint8_t*)"+CMQTTDISC: 0,0",
			ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000) != SIM_RES_MSG )		return 0;

	if ( !releaseMQTTclient() )		return 0;

	if (SIM_sendCMD((uint8_t*)"AT+CMQTTSTOP", (uint8_t*)"+CMQTTSTOP: 0",
			ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000) != SIM_RES_MSG )		return 0;

	__MY_SIM->mqttServer.connect = 0;
	return 1;
}
uint8_t MQTT_publish(uint8_t *topic, uint8_t *msg, uint8_t msglen)
{
	uint8_t check =0;
	uint8_t topiclen= (uint8_t) strlen((char*)topic);
	sprintf((char*)MQTT_Txbuff,"AT+CMQTTTOPIC=0,%d",topiclen);

	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)'>', ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)	return 0;

	if (SIM_sendCMD(topic, (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)		return 0;

	sprintf((char*)MQTT_Txbuff,"AT+CMQTTPAYLOAD=0,%d",msglen);

	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)'>', ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)	return 0;

	HAL_UART_Transmit(__SIM_UART, msg, msglen, 0xFFFF);
	check = SIM_checkMsg((uint8_t*)"OK", 2000);
	MarkAsReadData_SIM();
	if (check != SIM_RES_MSG)	return 0;

	if (SIM_sendCMD((uint8_t*)"AT+CMQTTPUB=0,1,60", (uint8_t*)"+CMQTTPUB: 0,0",
			ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)	return 0;

	return 1;
}
uint8_t MQTT_subcribe (uint8_t *topic)
{
	uint8_t topiclen = strlen ((char*)topic);

	sprintf((char*)MQTT_Txbuff,"AT+CMQTTSUBTOPIC=0,%d,1",topiclen);
	if (SIM_sendCMD(MQTT_Txbuff, (uint8_t*)'>', ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)	return 0;

	if (SIM_sendCMD(topic, (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG)		return 0;

	if (SIM_sendCMD((uint8_t*)"AT+CMQTTSUB=0", (uint8_t*)"+CMQTTSUB: 0,0", ENABLE_SIM_CHECKRES,ENABLE_MARKASREAD, 2000)!= SIM_RES_MSG) 	return 0;

	return 1;
}

void MQTT_testReceive (void)
{

//	MQTT_receive( MQTT_RECEIVE_TEST_MSG, 98 );
	Serial_log_string("\r\nTopic len:");
	Serial_log_number(__MY_SIM->mqttReceive.topicLen);
	Serial_log_string("\r\nTopic:");
	Serial_log_buffer(__MY_SIM->mqttReceive.topic, __MY_SIM->mqttReceive.topicLen);

	Serial_log_string("\r\nPayload len:");
	Serial_log_number(__MY_SIM->mqttReceive.payloadLen);
	Serial_log_string("\r\nPayload:");
	Serial_log_buffer(__MY_SIM->mqttReceive.payload, __MY_SIM->mqttReceive.payloadLen);
}
