/*
 * ServerMessage.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Admin
 */
#include "stm32f1xx_hal.h"
#include "ServerMessage.h"
#include "linkedlist.h"
#include "string.h"
#include "ds3231.h"
#include "time.h"
#include "stdio.h"
#include "Serial_log.h"
#include "crc32.h"
#include "main.h"

#define PACKBUFF_MAXLEN		1024
#define DATABUFF_MAXLEN		1024
uint8_t Pack_buff [PACKBUFF_MAXLEN];
void FourbytenumbertoBuff(uint32_t inputNumber, uint8_t *Buff)
{
    uint32_t tmp =inputNumber;
	for (uint8_t i=0 ;i< 4;i++)
	{
		Buff[3-i] = tmp & (0xFF);
 		tmp = tmp >>8;
	}
}
uint16_t Serialize_SSnodedata(s_list *list, uint8_t* Serial_buff, DATA_t dataType)
{

	uint16_t buff_len=0;
	uint8_t len =list->length;
	Node * current =list->head->next;
	switch ( dataType ) {
		case DATA_REGISTER:
			for (uint8_t i = 0;i < len; i++)
			{
				Serial_buff[3*i] = current->SSnode.SSnode_ID;
				Serial_buff[3*i+1]= current->SSnode.Battery;
				Serial_buff[3*i+2]= current->SSnode.V_type;
				buff_len = 3*i+3;
			}
			break;
		case DATA_PERIOD:
			for (uint8_t i = 0;i < len; i++)
			{
				Serial_buff[4*i] = current->SSnode.SSnode_ID;
				Serial_buff[4*i+1]= current->SSnode.Battery;
				Serial_buff[4*i+2]= ( current->SSnode.V_value >> 8 ) & 0xFF;
				Serial_buff[4*i+3]= current->SSnode.V_value & 0xFF;
				buff_len = 4*i+4;
			}
			break;
		case DATA_NETWREADY:
			for (uint8_t i = 0;i < len; i++)
			{
				Serial_buff[2*i] = current->SSnode.SSnode_ID;
				Serial_buff[2*i+1]= current->SSnode.Sensor_state;
				buff_len = 2*i+2;
			}
			break;
		case DATA_CALIB:
			break;
		default:
			break;
	}

	Serial_buff[buff_len] = '\0';
	return buff_len;
}
uint8_t addCRCtoPack(uint8_t *dataBuff, uint8_t bufflen)
{
	uint32_t crc;
	MQTT_publish( (uint8_t*)TOPIC_PUB, dataBuff, bufflen);
	crc= crc32(dataBuff, bufflen);

	uint8_t crc_buff[4];
	FourbytenumbertoBuff( crc, crc_buff);
////	Serial_log_string("crc: ");
//	uint8_t tmp[10];
//	uint8_t len = sprintf( (char*)tmp, "%ld", crc);
	MQTT_publish( (uint8_t*)TOPIC_PUB, crc_buff, 4);
//	Serial_log_buffer(tmp, len);
	memcpy(dataBuff+bufflen,crc_buff,4);
	memcpy (dataBuff+bufflen+4,"}",1);
	return 5;    // crc + EOF length
}
uint8_t addTimetobuff(uint8_t *buff, uint16_t position)
{
	_RTC RTCtime ;
//	RTCtime.Year= 23;
//	RTCtime.Month=11;
//	RTCtime.Date = 15;
//	RTCtime.Hour = 7;
//	RTCtime.Min = 56;
//	RTCtime.Sec = 35;
	if (DS3231_GetTime(&RTCtime))
	{
		struct tm t;
		time_t epochtime;
		t.tm_year = 2000 + RTCtime.Year -1900;
		t.tm_mon = RTCtime.Month;
		t.tm_mday = RTCtime.Date;
		t.tm_hour = RTCtime.Hour;
		t.tm_min = RTCtime.Min;
		t.tm_sec = RTCtime.Sec;
		t.tm_isdst = -1;
		epochtime = mktime(&t);
		uint8_t time_buff[4];
		FourbytenumbertoBuff((uint32_t)epochtime,time_buff);
		memcpy(buff+position,time_buff,4);
		return 1;
	}
	return 0;
}
void ClearBuff(uint8_t *buff)
{
	memset(buff,0,PACKBUFF_MAXLEN);
}
uint8_t createPack( uint8_t StationID, PACK_t PackType, DATA_t DataType, CMD_t CMDType )
{
	ClearBuff( Pack_buff );
	uint8_t pack_len = 0;
	Pack_buff[pack_len++] = MQTT_FRAME_SOF;
	Pack_buff[pack_len++] = StationID;
	Pack_buff[pack_len++] = PackType;

	if ( DataType != DATA_NONE )
	{
		Pack_buff[pack_len++] = DataType;
	}
	else if ( CMDType != CMD_NONE )
	{
		Pack_buff[pack_len++] = CMDType;
	}
	else return 0;
	return pack_len;
}

uint8_t Serialize_Stationdata(uint8_t stationID, uint16_t stCurrent, uint8_t stVoltage, uint8_t *Buffer, DATA_t dataType)
{
	uint8_t buff_len = 0;
	switch (dataType) {
		case DATA_REGISTER:
			Buffer[buff_len++] = stationID;
			Buffer[buff_len++] = (uint8_t)( ( stCurrent >> 8 ) & 0xFF);
			Buffer[buff_len++] = (uint8_t)( stCurrent & 0xFF );
			Buffer[buff_len++] = (uint8_t)( ( stVoltage >> 8 ) & 0xFF );
			Buffer[buff_len++] = (uint8_t)( stVoltage & 0xFF );
			break;
		case DATA_NETWREADY:
			Buffer[buff_len++] = stationID;
			Buffer[buff_len++] = (uint8_t) ACTIVE;
			break;
		case DATA_PERIOD:
			Buffer[buff_len++] = stationID;
			Buffer[buff_len++] = (uint8_t)( ( stCurrent >> 8 ) & 0xFF);
			Buffer[buff_len++] = (uint8_t)( stCurrent & 0xFF );
			Buffer[buff_len++] = (uint8_t)( ( stVoltage >> 8 ) & 0xFF );
			Buffer[buff_len++] = (uint8_t)( stVoltage & 0xFF );
			break;
		case DATA_CALIB:
			Buffer[buff_len++] = stationID;
			Buffer[buff_len++] = (uint8_t)( ( stCurrent >> 8 ) & 0xFF);
			Buffer[buff_len++] = (uint8_t)( stCurrent & 0xFF );
			Buffer[buff_len++] = (uint8_t)( ( stVoltage >> 8 ) & 0xFF );
			Buffer[buff_len++] = (uint8_t)( stVoltage & 0xFF );
			break;
		default:
			break;
	}

	return buff_len;
}
uint8_t Serialize_Data( uint8_t stationID, uint16_t stCurrent, uint8_t stVoltage, s_list *ssNodelist, uint8_t *databuff, DATA_t dataType)
{
	uint8_t data_len =0 ;


	data_len = Serialize_Stationdata( stationID, stCurrent, stVoltage, databuff, dataType );
	data_len += Serialize_SSnodedata( ssNodelist, databuff + data_len, DATA_REGISTER);
	if ( !addTimetobuff( databuff, data_len ) )		return 0;
	data_len += 4;      // 4 bytes of time + buffer length
	return data_len;
}

uint8_t addDatatoPack(uint8_t *Pack, uint16_t Pack_len, uint8_t *Databuff, uint16_t dataLen)
{
	Pack[Pack_len] = (uint8_t)dataLen ;
	memcpy(Pack+ Pack_len + 1,Databuff, dataLen);
	return dataLen + 1;
}

uint8_t Serialize_addtionaldata( CMD_t CMDtype, uint8_t *Databuffer, uint8_t *phone_numb, uint16_t time_delay,
							uint8_t *StationorSensorIDbuff, MBA_state_t MBAstate, Stepmotor_dir_t Stepm_DIR,
							Stepmotor_change_mode_t Stepm_changeMode, uint8_t Stepm_changeValue )
{
	ClearBuff(Databuffer);
	uint8_t buff_len = 0;
	switch ( CMDtype ) {
		case CMD_SMS_CALIB:
			// Time delays (2byte)
			Databuffer[buff_len+1] = time_delay & 0xFF ;
			Databuffer[buff_len] = ( time_delay >>8 ) & 0xFF ;
			buff_len += 2;
			// Phone number (11byte)
			strcpy( (char*)Databuffer + buff_len , (char*)phone_numb );
			buff_len += strlen( (char*)phone_numb );
			break;
		case CMD_SMS_GETSTATUS:
			// Phone number	(11 byte)
			strcpy( (char*)Databuffer + buff_len , (char*)phone_numb );
			buff_len += strlen( (char*)phone_numb );
			break;
		case CMD_CTRL_MBA:
			// First byte: ON/OFF
			Databuffer[buff_len++] = MBAstate;
			// N next byte: Station ID want to switch
			strcpy( (char*)Databuffer, (char*)StationorSensorIDbuff );
			buff_len += strlen( (char*)StationorSensorIDbuff );
			// Time delays ( 2 bytes )
			Databuffer[buff_len+1] = time_delay & 0xFF ;
			Databuffer[buff_len] = ( time_delay >>8 ) & 0xFF ;
			buff_len += 2;
			// Phone number ( 11 bytes)
			strcpy( (char*)Databuffer + buff_len , (char*)phone_numb );
			buff_len += strlen( (char*)phone_numb );
			break;
		case CMD_CTRL_STEP_MOTOR:
			// First byte: Increase/Decrease (1 byte)
			Databuffer[buff_len++] = Stepm_DIR ;
			// Second byte: Change Percentage/Step (1 byte)
			Databuffer[buff_len++] = Stepm_changeMode;
			// Third byte : Value
			Databuffer[buff_len++] = Stepm_changeValue;
			// N next byte : Station ID
			strcpy( (char*)Databuffer, (char*)StationorSensorIDbuff );
			buff_len += strlen( (char*)StationorSensorIDbuff );
			// Time delays (2 bytes)
			Databuffer[buff_len+1] = time_delay & 0xFF ;
			Databuffer[buff_len] = ( time_delay >>8 ) & 0xFF ;
			buff_len += 2;
			// Phone number (11 bytes)
			strcpy( (char*)Databuffer, (char*)phone_numb );
			buff_len += strlen( (char*)phone_numb );
			break;
		case CMD_GET_LATEST_DATA_SENSOR:
			// N byte Station ID
			strcpy( (char*)Databuffer, (char*)StationorSensorIDbuff );
			buff_len += strlen( (char*)StationorSensorIDbuff );
			// Phone number ( 11 bytes)
			strcpy( (char*)Databuffer, (char*)phone_numb );
			buff_len += strlen( (char*)phone_numb );
			break;
		case CMD_GET_LASTEST_DATA_STATION:
			// N byte Sensor ID
			strcpy( (char*)Databuffer, (char*)StationorSensorIDbuff );
			buff_len += strlen( (char*)StationorSensorIDbuff );
			// Phone number (11 byte)
			strcpy( (char*)Databuffer, (char*)phone_numb );
			buff_len += strlen( (char*)phone_numb );
			break;
		default:
			break;
	}
	return buff_len;
}

uint8_t Register2Server(uint8_t stationID, uint16_t stCurrent, uint16_t stVoltage, s_list *ssNodelist)
{
	uint8_t tmp_databuff [256];
	uint16_t pack_len = 0;
	// Create package
	pack_len = createPack( stationID, PACKT_REGISTER, DATA_REGISTER, CMD_NONE );
	if ( !pack_len )	return 0;
	 //Convert Register data to temp buffer
	uint8_t datalen= Serialize_Data( stationID, stCurrent, stVoltage, ssNodelist, tmp_databuff, DATA_REGISTER );
	if ( !datalen )	return 0;
	// Add Register data  temp buffer to package
	pack_len += addDatatoPack( Pack_buff, pack_len, tmp_databuff, datalen );
	// Add CRC to package
	pack_len += addCRCtoPack( Pack_buff, pack_len );
	// Publish
	if (MQTT_publish((uint8_t*)TOPIC_PUB, Pack_buff, pack_len))	return 1;
	return 0;
}

uint8_t sendCMDtoServer( uint8_t stationID, CMD_t CMDtype, uint8_t *phone_numb,
		uint16_t time_delay, uint8_t *StationorSensorIDbuff, MBA_state_t MBAstate,
		Stepmotor_dir_t Stepm_DIR, Stepmotor_change_mode_t Stepm_changeMode,
		uint8_t Stepm_changeValue )
{
	uint8_t  pack_len = 0;
	pack_len = createPack( stationID, PACKT_CMD, DATA_NONE, CMDtype );
	if ( !pack_len )	return 0; // Create package

	uint8_t tmpAbuff [256];
	uint8_t Abuff_len = Serialize_addtionaldata(CMDtype, tmpAbuff, phone_numb, time_delay, StationorSensorIDbuff, MBAstate, Stepm_DIR, Stepm_changeMode, Stepm_changeValue);
	pack_len += addDatatoPack(Pack_buff, pack_len, tmpAbuff, Abuff_len);    // Add additional data to package

	pack_len += addCRCtoPack( Pack_buff, pack_len );		// Add CRC to package

	if ( !MQTT_publish( (uint8_t*)TOPIC_PUB, Pack_buff , pack_len ) )	return 0;
	return 1;
}


uint8_t sendData2Server(uint8_t stationID, uint16_t stCurrent, uint16_t stVoltage, s_list *ssNodelist, DATA_t dataType)
{
	uint8_t databuff[DATABUFF_MAXLEN];
	uint16_t data_len = 0;
	ClearBuff(databuff);
	ClearBuff( Pack_buff );
	// Creat package
	uint16_t pack_len = createPack( stationID, PACKT_DATA, dataType, CMD_NONE );
	// Add data to package
	data_len = Serialize_Data( stationID, stCurrent, stVoltage, ssNodelist, databuff, dataType );
	pack_len += addDatatoPack(Pack_buff, pack_len, databuff, data_len);
	// Add CRC to package
	pack_len += addCRCtoPack(Pack_buff, pack_len);
	// Publish
	if ( !MQTT_publish( (uint8_t*)TOPIC_PUB, Pack_buff, pack_len) ) return 0;
	return 1;
}
