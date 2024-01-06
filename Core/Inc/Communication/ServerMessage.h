/*
 * ServerMessage.h
 *
 *  Created on: Nov 7, 2023
 *      Author: Admin
 */

#ifndef INC_SERVERMESSAGE_H_
#define INC_SERVERMESSAGE_H_

#include "MQTT.h"
#include "linkedlist.h"
#include "main.h"




#define BROADCAST_ID 	0xFF
#define NODEID_POS		0
#define PACKT_POS		1
#define DATAT_POS		2
#define CMD_POS			2
#define DATA_POS		3
#define RESSTATUS_POS	2
#define DATAREST_POS	3
#define ADDDATA_POS		3

#define USER_MSG_HEADER_NUMBOF_ACT_STATION		(char*)"So luong station node dang hoat dong"
#define USER_MSG_HEADER_NUMBOF_ACT_SENSOR		(char*)"So luong sensor node dang hoat dong"
#define USER_MSG_HEADER_NUMBOF_FAIL_STATION		(char*)"So luong station node bi loi"
#define USER_MSG_HEADER_NUMBOF_FAIL_SENSOR		(char*)"So luong sensor node bi loi"



typedef enum {
	RES_OK = 0x01,
	RES_ERROR
}RES_STATUS_t;
typedef enum
{
	CMD_NONE,
	CMD_PREPARE_CALIB,
	CMD_START_CALIB ,
	CMD_SMS_CALIB,
	CMD_SMS_GETSTATUS,
	CMD_SLEEP,
	CMD_WAKEUP,
	CMD_CTRL_MBA,
	CMD_CTRL_STEP_MOTOR,
	CMD_SWITCH_MODE,
	CMD_GET_LATEST_DATA_SENSOR,
	CMD_GET_LASTEST_DATA_STATION
}CMD_t;

typedef enum
{
	PACKT_DATA= 0xF1,
	PACKT_CMD,
	PACKT_RESDATA,
	PACKT_RESCALIB,
	PACKT_REGISTER,
	PACKT_RESREGISTER
}PACK_t;

typedef enum
{
	DATA_NONE,
	DATA_PERIOD,
	DATA_NETWREADY,
	DATA_STATUS,
	DATA_REGISTER,
	DATA_CALIB,
	DATA_LATEST,
	DATA_AFTERCALIB
}DATA_t;

typedef enum {
	MBA_NULL= 0x00,
	MBA_ON = 0x01,
	MBA_OFF
}MBA_state_t;

typedef enum {
	STEPM_DIR_INC = 0x01,
	STEPM_DIR_DEC,
	STEPM_DIR_DEFAULT
}Stepmotor_dir_t;

typedef enum {
	STEPM_MODE_PERCENTAGE = 0x01,
	STEPM_MODE_STEP,
	STEPM_MODE_DEFAULT
}Stepmotor_change_mode_t;

void initServerMsg (Station_t *Station, SMS_t *mySMS);

void triggerCMD (CMD_t cmdType);

uint8_t Register2Server(void);

uint8_t sendCMDtoServer( CMD_t CMDtype,uint8_t *SMSdatabuffer, uint16_t datalen, MBA_state_t MBAstate,
		Stepmotor_dir_t Stepm_DIR, Stepmotor_change_mode_t Stepm_changeMode,
		uint8_t Stepm_changeValue );
uint8_t sendData2Server( DATA_t dataType);

void processingComingMsg(uint8_t *Msg, uint16_t Msg_len, uint8_t stID);

void testProcessingMsg(void);
#endif /* INC_SERVERMESSAGE_H_ */
