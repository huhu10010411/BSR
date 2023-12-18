/*
 * SIM.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Admin
 */

#include <stdio.h>
#include "SIM.h"
#include <string.h>
#include <stdlib.h>
#include "MQTT.h"
#include "Serial_log.h"

#define SIM_BUFF_SIZE        512
#define SIM_RXBUFF_SIZE      256
#define SIM_SMS_RXSIZE       128
#define MQTT_RXBUFF_SIZE     1024

#define SMS_CMD_STA			"SMSCMD STATE"
#define SMS_CMD_CALIB		"SMSCMD CALIB"
UART_HandleTypeDef *__SIM_UART;
DMA_HandleTypeDef  *__SIM_DMA_UART;
SIM_t	*__SIM;

uint8_t SIM_DataAvailable =0;
uint16_t oldPos =0, newPos =0;
uint8_t SIMbuff[SIM_BUFF_SIZE];
uint8_t SIMRxbuff[SIM_RXBUFF_SIZE];
uint8_t SIM_Txbuff[100];
uint8_t SMS_Rxbuff [SIM_SMS_RXSIZE];
uint8_t MQTT_Rxbuff [MQTT_RXBUFF_SIZE];
uint16_t head =0, tail=0;
uint16_t mqtttail=0, mqtthead= 0;
volatile uint8_t newSMS =0;
volatile uint8_t newMQTTmsg =0;
uint8_t newMQTTmsgReceiving=0;

uint8_t getSMS = 0;

void initSIM(UART_HandleTypeDef *huart, DMA_HandleTypeDef  *hdma , SIM_t *mySIM)
{
	__SIM = mySIM;
	__SIM_UART = huart;
	__SIM_DMA_UART = hdma;
	enableReceiveDMAtoIdle_SIM();
//	SIM_sendCMD((uint8_t*)"ATE0",(uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000);
}
void enableReceiveDMAtoIdle_SIM(void)
{
	  HAL_UARTEx_ReceiveToIdle_DMA(__SIM_UART, SIMRxbuff, SIM_RXBUFF_SIZE);
	  __HAL_DMA_DISABLE_IT(__SIM_DMA_UART,DMA_IT_HT);
}

/*
 *  @Return : if the "word" is presented in the "databuff",
 *  			function return the pointer point to the first element
 *  			of "word" in "databuff"
 *  		else return NULL
 */
uint8_t* isWordinBuff(uint8_t *databuff,uint16_t buff_size, uint8_t *word)
{
	uint8_t wordlen = strlen((char*)word);
	for (uint16_t i = 0; i< buff_size - wordlen; i++)
	{
		if (!memcmp(databuff +i,word,wordlen))
		{
			return databuff+ i;
		}
	}
	return NULL;
}
uint16_t cpyBuff2Buff(uint8_t *Destbuff, uint8_t *SourceBuff,uint16_t Sourcebuff_maxsize, uint16_t head, uint16_t tail)
{
	uint16_t size = 0;
	if (head == tail )
	{
		return 0;
	}
	else if (head > tail)
	{
		size = head -tail;
		memcpy(Destbuff, SourceBuff+tail, size);
	}
	else
	{
		size = Sourcebuff_maxsize - tail + head;
		memcpy(Destbuff, SourceBuff + tail, Sourcebuff_maxsize - tail);
		memcpy (Destbuff+ Sourcebuff_maxsize - tail, SourceBuff, head);
	}
	return size;
}
void SIM_callback(uint16_t Size)
{

	SIM_DataAvailable = 1;

		oldPos = newPos;  // Update the last position before copying new data

		/* If the data in large and it is about to exceed the buffer size, we have to route it to the start of the buffer
		 * This is to maintain the circular buffer
		 * The old data in the main buffer will be overlapped
		 */
		if (oldPos+Size > SIM_BUFF_SIZE)  // If the current position + new data size is greater than the main buffer
		{
			uint16_t datatocopy = SIM_BUFF_SIZE-oldPos;  // find out how much space is left in the main buffer
			memcpy ((uint8_t *)SIMbuff+oldPos, (uint8_t *)SIMRxbuff, datatocopy);  // copy data in that remaining space

			oldPos = 0;  // point to the start of the buffer
			memcpy ((uint8_t *)SIMbuff, (uint8_t *)SIMRxbuff+datatocopy, (Size-datatocopy));  // copy the remaining data
			newPos = (Size-datatocopy);  // update the position
		}

		/* if the current position + new data size is less than the main buffer
		 * we will simply copy the data into the buffer and update the position
		 */
		else
		{
			memcpy ((uint8_t *)SIMbuff+oldPos, (uint8_t *)SIMRxbuff, Size);
			newPos = Size+oldPos;
		}

//		Serial_log_string("Rx: ");
//		Serial_log_buffer(SIMRxbuff, Size);

		/* Update the position of the Head
		 * If the current position + new size is less then the buffer size, Head will update normally
		 * Or else the head will be at the new position from the beginning
		 */
		if (head + Size < SIM_BUFF_SIZE ) head = head + Size;
		else head = head + Size - SIM_BUFF_SIZE;

		/* start the DMA again */
		enableReceiveDMAtoIdle_SIM();

		/* Check OK within incoming data*/
		if (isWordinBuff(SIMRxbuff, Size, (uint8_t*)"+CMTI:SM") !=NULL)
		{
			newSMS =1;
		}
		if ( newMQTTmsgReceiving || ( isWordinBuff(SIMRxbuff, Size, (uint8_t*) "+CMQTTRXSTART:") != NULL) )
		{
			if( !newMQTTmsgReceiving )
			{
				mqtttail = head- Size;
			}
			//receive MQTT sub message
			newMQTTmsgReceiving = 1;
		}
		if ( newMQTTmsgReceiving && ( isWordinBuff(SIMRxbuff, Size,(uint8_t*) "+CMQTTRXEND:") != NULL ) )
		{
			newMQTTmsgReceiving = 0;
//			newMQTTmsg =1;
			mqtthead = head;
			memset(MQTT_Rxbuff, 0, MQTT_RXBUFF_SIZE);
			uint16_t cpy_len = cpyBuff2Buff (MQTT_Rxbuff, SIMbuff, SIM_BUFF_SIZE, mqtthead, mqtttail);
			MQTT_receive(MQTT_Rxbuff, cpy_len);
		}

//		for (int i=0; i<Size; i++)
//		{
//			if ((SIMRxbuff[i] == '+') && (SIMRxbuff[i+1] == 'C')&& (SIMRxbuff[i+2] == 'M')&& (SIMRxbuff[i+3] == 'T')&& (SIMRxbuff[i+4] == 'I')&& (SIMRxbuff[i+5] == ':')&& (SIMRxbuff[i+8] == 'S')&& (SIMRxbuff[i+9] == 'M'))
//			{
//				newSMS = 1;
//			}
//		}
}

SIM_res_t SIM_sendCMD(uint8_t *cmd, uint8_t *checkResMsg, uint8_t CheckResENorDIS, uint8_t ENorDISmarkasread, uint32_t timeout)
{
	uint8_t len = sprintf( (char*)SIM_Txbuff, "%s\r\n", cmd);
	HAL_UART_Transmit(__SIM_UART, SIM_Txbuff, len, 0xFFFF);
	if (CheckResENorDIS == ENABLE_SIM_CHECKRES)
	{
		SIM_res_t res = SIM_checkMsg(checkResMsg, timeout);
		if (ENorDISmarkasread == ENABLE_MARKASREAD)
		{
			MarkAsReadData_SIM();
		}
//		Serial_log_string("Res of CMD: ");
//		Serial_log_buffer(cmd, len);
//		Serial_log_number(res);
		return res;
	}
	return SIM_NO_RES;
}
void RemoveNULLinbuff(uint8_t *buff, uint8_t bufflen)
{
	for (uint8_t i =0; i< bufflen; i ++)
	{
		if (buff[i]== '\0')
		{
			buff[i]= '_' ;
		}
	}
}

SIM_res_t SIM_checkMsg(uint8_t *Msg, uint16_t timeout)
{
	SIM_res_t res= SIM_NO_RES;
	timeout /= 500;
	uint8_t dataSize;
	uint8_t tmpdbuff[256];

	for (uint16_t i = 0; i < 500 ; i++)
	{
		HAL_Delay(timeout);

		if (head > tail)
		{
			dataSize = head - tail ;
			memset( (char*)tmpdbuff, 0, 256 );
			memcpy(tmpdbuff, SIMbuff + tail, dataSize);
		}
		else if ( head < tail )
		{
			dataSize = SIM_BUFF_SIZE - tail + head ;
			memset( (char*)tmpdbuff, 0, 256 );
			memcpy(tmpdbuff, SIMbuff + tail, SIM_BUFF_SIZE - tail);
			memcpy(tmpdbuff + SIM_BUFF_SIZE - tail, SIMbuff, head);
		}

//		RemoveNULLinbuff(tmpbuff, Size);

		if ( isWordinBuff(tmpdbuff, dataSize, Msg) != NULL )	{
//			Serial_log_string("msg: ");
//			Serial_log_buffer(tmpdbuff, dataSize);
//			Serial_log_string(";");
//			Serial_log_string("\ntail: ");
//			Serial_log_number(tail);
//			Serial_log_string(" ");
//			Serial_log_string("\nhead: ");
//			Serial_log_number(head);
//			Serial_log_string(" ");
			return SIM_RES_MSG ;
		}

		if ( isWordinBuff(tmpdbuff, dataSize, (uint8_t*)"ERROR") != NULL )	{
//			Serial_log_string("err: ");
//			Serial_log_buffer(tmpdbuff, dataSize);
//			Serial_log_string(";");
			return SIM_ERROR ;
		}
	}

//	Serial_log_string("no res: ");
//	Serial_log_buffer(tmpdbuff, dataSize);
//	Serial_log_string(";");
//	Serial_log_string("\ntail: ");
//	Serial_log_number(tail);
//	Serial_log_string(" ");
//	Serial_log_string("\nhead: ");
//	Serial_log_number(head);
//	Serial_log_string(" ");

 	return res;

}

void MarkAsReadData_SIM(void)
{
	tail = head;
}
uint8_t getCMDnNumber(SMS_t *SMS)
{
	uint8_t res = 0;
	uint8_t i =0;
	uint8_t tmpCMDbuff [13];
	uint8_t *p;

		if ((p=(uint8_t*) strstr((char*)SMS_Rxbuff,"REC UNREAD")) != NULL)
		{
			i+=14;
			uint8_t j =0;
			while  (*(p+i+j) != '\"' && j<12)
			{
				SMS->Number[j] = *(p+i+j);
				j++;
			}
			SMS->Number[11] = '\0';
			while (*(p+i) != '\n' && i <SIM_SMS_RXSIZE)
			{
				i++;
			}
			i++;
			memcpy((char*)tmpCMDbuff,(char*)(p+i),12);
			if (strstr((char*)tmpCMDbuff,SMS_CMD_STA) != NULL)
			{
				SMS->cmd =SMS_GET_STATUS;
				res= 1;
			}
			if (strstr((char*)tmpCMDbuff,SMS_CMD_CALIB) != NULL)
			{
				SMS->cmd= SMS_CALIBRATION;
				res= 1;
			}
		}

	return res;

}
uint8_t processingSMS(SMS_t *mySMS)
{
	getSMS =1;
	if (!SIM_sendCMD((uint8_t*)"AT+CMGL=\"REC UNREAD\"", (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, 1000))		return 0;
	getSMS =0;

	if (!getCMDnNumber(mySMS))		return 0;

	return 1;
}

uint8_t SIM_checkCMD (SIM_CMD_t cmd)
{
	uint8_t res = 0;
//	SIM_res_t check;
	switch (cmd) {
		case SIM_CMD_SIMCARD_PIN:
			if ( SIM_sendCMD( (uint8_t*)"AT+CPIN?", (uint8_t*)"+CPIN: READY", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, SIM_TIMEOUT_LONG) == SIM_RES_MSG ) {
				res = 1;
				Serial_log_string("SIM card READY\r\n");
			}
			else {
				Serial_log_string("SIM card not READY\r\n");
			}
			break;
		case SIM_CMD_NW_CPSI:
			if ( SIM_sendCMD( (uint8_t*)"AT+CPSI?", (uint8_t*)"+CPSI: NO SERVICE", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, SIM_TIMEOUT_LONG) == SIM_RES_MSG ) {
				Serial_log_string("NO SERVICE, network status has some problem");
			}
			else {
				res = 1;
				Serial_log_string("SERVICE available\r\n");
			}
			break;
		case SIM_CMD_NW_CREG:
			if ( SIM_sendCMD( (uint8_t*)"AT+CREG?", (uint8_t*)"+CREG: 0,1", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, SIM_TIMEOUT_LONG) == SIM_RES_MSG ) {
				res = 1;
				Serial_log_string("Module is registered to CS domain\r\n");
			}
			else {
				Serial_log_string("Module is not registered to CS domain, reboot the module\r\n");
			}
			break;
		case SIM_CMD_PACKDOM_CGREG:

			break;
		case SIM_CMD_STA_CSQ:
			if ( SIM_sendCMD( (uint8_t*)"AT+CSQ", (uint8_t*)"+CSQ: 99", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, SIM_TIMEOUT_LONG) == SIM_RES_MSG ) {
				Serial_log_string("Signal quality is bad, please check SIM card or reboot the module\r\n");
			}
			else {
				res = 1;
				Serial_log_string("Signal quality is good\r\n");
			}
			break;
		default :
			break;
	}
	return res;
}

void SIM_checkOperation(void)
{
	SIM_sendCMD( (uint8_t*)"AT\r\n", (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, SIM_TIMEOUT_LONG);
	 HAL_Delay(500);
//	SIM_checkCMD(SIM_CMD_SIMCARD_PIN);
	SIM_checkCMD(SIM_CMD_NW_CREG);

	//	Serial_log_string("send AT\r\n");
//	Serial_log_string("respond AT: ");
//	switch (rescode) {
//		case SIM_NO_RES:
//			Serial_log_string("SIM not respond AT\r\n");
//			SIM_sendCMD( (uint8_t*)"AT+CRESET\r\n", (uint8_t*)"OK", ENABLE_SIM_CHECKRES, ENABLE_MARKASREAD, SIM_TIMEOUT_LONG);
//			break;
//		case SIM_RES_MSG:
//			Serial_log_string("SIM respond AT: OK\r\n");
//			break;
//		case SIM_ERROR:
//			Serial_log_string("SIM ERROR\r\n");
//			break;
//		default:
//			break;
//	}
//	SIM_checkNetworkavailable();
}
