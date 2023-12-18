/*
 * SIM.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Admin
 */

#ifndef INC_SIM_H_
#define INC_SIM_H_

#include "stm32f1xx.h"

#define SIM_STA_CSQ_CMD				"AT+CSQ"
#define SIM_NW_CREG_CMD				"AT+CREG?"
#define SIM_NW_CPSI_CMD				"AT+CGREG?"
#define SIM_PACKDOM_CGREG_CMD		"AT+CPSI?"

#define ENABLE_SIM_CHECKRES	 	1
#define DISABLE_SIM_CHECKRES  	0
#define ENABLE_MARKASREAD		1
#define DISABLE_MARKASREAD		0

#define SIM_TIMEOUT_SHORT		500
#define SIM_TIMEOUT_MEDIUM		1000
#define SIM_TIMEOUT_LONG		2000

#define MAX_PAYLOAD_LEN		1024
#define MAX_TOPIC_LEN		128

extern UART_HandleTypeDef *__SIM_UART;
extern DMA_HandleTypeDef  *__SIM_DMA_UART;

typedef enum {
	SIM_CMD_SIMCARD_PIN,
	SIM_CMD_STA_CSQ,
	SIM_CMD_NW_CREG,
	SIM_CMD_NW_CPSI,
	SIM_CMD_PACKDOM_CGREG,
}SIM_CMD_t;
typedef enum {
	SIM_ERROR,
	SIM_RES_MSG,
	SIM_NO_RES
}SIM_res_t;

typedef enum
{
	SMS_NO_CMD,
	SMS_GET_STATUS,
	SMS_CALIBRATION
}SMS_CMD_t;

typedef struct
{
	SMS_CMD_t  cmd;
	unsigned char Number[12];
}SMS_t;

typedef struct {
    char *host;
    char *willtopic;
    uint8_t *willmsg;
    uint16_t port;
    uint8_t connect;
} mqttServer_t;

typedef struct {
    char *username;
    char *pass;
    char *clientID;
    unsigned short keepAliveInterval;
} mqttClient_t;

typedef struct {
    uint8_t newEvent;
    unsigned char dup;
    int qos;
    unsigned char retained;
    unsigned short msgId;
    uint8_t payload[MAX_PAYLOAD_LEN];
    uint16_t payloadLen;
    uint8_t topic[MAX_TOPIC_LEN];
    uint16_t topicLen;
} mqttReceive_t;

typedef struct {
    char *apn;
    char *apn_user;
    char *apn_pass;
} sim_t;

typedef struct {
    sim_t sim;
    mqttServer_t mqttServer;
    mqttClient_t mqttClient;
    mqttReceive_t mqttReceive;
} SIM_t;



extern volatile uint8_t newSMS ;
extern volatile uint8_t newMQTTmsg ;
/****************************************************/
void initSIM(UART_HandleTypeDef *huart, DMA_HandleTypeDef  *hdma , SIM_t *mySIM);

void SIM_callback(uint16_t Size);

void enableReceiveDMAtoIdle_SIM(void);

SIM_res_t SIM_sendCMD(uint8_t *cmd, uint8_t *checkResMsg, uint8_t CheckResENorDIS, uint8_t ENorDISmarkasread,uint32_t timeout);

SIM_res_t SIM_checkMsg(uint8_t *Msg, uint16_t timeout);

SIM_res_t SIM_checkMQTTmess(uint8_t *Msg, uint8_t *mqttRxbuff);

void MarkAsReadData_SIM(void);

uint8_t processingSMS(SMS_t *mySMS);

uint8_t getCMDnNumber(SMS_t *SMS);

uint8_t* isWordinBuff(uint8_t *databuff,uint16_t buff_size, uint8_t *word);

void SIM_checkOperation(void);

uint8_t SIM_checkCMD (SIM_CMD_t cmd);


#endif /* INC_SIM_H_ */
