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

/*
 * @Define Package type here
 */
//#define PACKT_DATA      0xF1
//#define PACKT_CMD		0xF2
//#define PACKT_RESDATA	0xF3
//#define PACKT_RESCALIB	0xF4
//#define PACKT_REG		0xF5

/*
 *  @Define Data type here
 */
//#define DATAT_IN4		0x01
//#define DATAT_NETWREADY	0x02
//#define DATAT_STATUS	0x03
//#define DATAT_REG		0x04
/*
 * @Define Start/End of frame
 */
#define MQTT_FRAME_SOF		0x7B
#define MQTT_FRAME_EOF		0x7D

#define TOPIC_PUB		"BSR/STATION2SERVER"
#define TOPIC_SUB		"BSR/SERVER2STATION"

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
	PACKT_CMD ,
	PACKT_RESDATA,
	PACKT_RESCALIB,
	PACKT_REGISTER
}PACK_t;

typedef enum
{
	DATA_NONE,
	DATA_PERIOD,
	DATA_NETWREADY,
	DATA_STATUS,
	DATA_REGISTER,
	DATA_CALIB,
	DATA_LATEST
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

uint8_t Register2Server(uint8_t stationID, uint16_t stCurrent, uint16_t stVoltage, s_list *ssNodelist);
uint8_t sendCMDtoServer( uint8_t stationID, CMD_t CMDtype, uint8_t *phone_numb,
		uint16_t time_delay, uint8_t *StationorSensorIDbuff, MBA_state_t MBAstate,
		Stepmotor_dir_t Stepm_DIR, Stepmotor_change_mode_t Stepm_changeMode,
		uint8_t Stepm_changeValue );
uint8_t sendData2Server(uint8_t stationID, uint16_t stCurrent, uint16_t stVoltage, s_list *ssNodelist, DATA_t dataType);
#endif /* INC_SERVERMESSAGE_H_ */
