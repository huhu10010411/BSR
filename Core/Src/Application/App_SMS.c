/*
 * App_SMS.c
 *
 *  Created on: Dec 30, 2023
 *      Author: Admin
 */

#include "App_SMS.h"
#include "SIM.h"
#include "ServerMessage.h"
#include "Task.h"

static SMS_t *__MY_SMS;

void initApp_SMS(SMS_t *mySMS)
{
	__MY_SMS = mySMS;
}

uint8_t SMSreturn (SMS_CMD_t smsCMD)
{
	SMS_request_t *tmpSMScmd;
	switch (smsCMD) 	{
	case SMS_CMD_CTRL_ON:
		tmpSMScmd = &__MY_SMS->CtrlON;
		break;
	case SMS_CMD_CTRL_OFF:
		tmpSMScmd = &__MY_SMS->CtrlOFF;
		break;
	case SMS_CMD_CTRL_INC:
		tmpSMScmd = &__MY_SMS->CtrlINC;
		break;
	case SMS_CMD_CTRL_DEC:
		tmpSMScmd = &__MY_SMS->CtrlDEC;
		break;
	case SMS_CMD_CTRL_CALIB:
		tmpSMScmd = &__MY_SMS->CtrlCALIB;
		break;
	case SMS_CMD_GET_STATUS:
		tmpSMScmd = &__MY_SMS->GetStatus;
		break;
	case SMS_CMD_GET_STATION:
		tmpSMScmd = &__MY_SMS->GetStation;
		break;
	case SMS_CMD_GET_SENSOR:
		tmpSMScmd = &__MY_SMS->GetSensor;
		break;
	default:
		break;
	}
	if (tmpSMScmd == NULL) 	return 0;
	SMS_sendMsg(tmpSMScmd->data, tmpSMScmd->datalength, tmpSMScmd->phonenumb);
	return 1;
}
void processApp_SMS(void)
{
	processingSMS();
	// check for new SMS request
	if (checkSMSrequest(SMS_CMD_CTRL_ON) || checkSMSrequest(SMS_CMD_CTRL_OFF) )	{
		triggerTaskflag(TASK_SEND_CTRLMBA, FLAG_EN);
	}
	if (checkSMSrequest(SMS_CMD_CTRL_INC) || checkSMSrequest(SMS_CMD_CTRL_DEC) ) {
		triggerTaskflag(TASK_SEND_CTRLSTEPMOR, FLAG_EN);
	}
	if (checkSMSrequest(SMS_CMD_CTRL_CALIB))	{
		triggerTaskflag(TASK_SEND_CALIB, FLAG_EN);
	}
	if (checkSMSrequest(SMS_CMD_GET_STATUS))	{
		triggerTaskflag(TASK_SEND_GETSTATUS, FLAG_EN);
	}
	if (checkSMSrequest(SMS_CMD_GET_STATION))	{
		triggerTaskflag(TASK_SEND_GETSTATION, FLAG_EN);
	}
	if (checkSMSrequest(SMS_CMD_GET_SENSOR))	{
		triggerTaskflag(TASK_SEND_GETSENSOR, FLAG_EN);
	}
// Check for return SMS
	if (checkSMSreturn(SMS_CMD_CTRL_ON))	{
		SMSreturn(SMS_CMD_CTRL_ON);
		triggerSMSreturn(SMS_CMD_CTRL_ON, SMS_CMD_DISABLE);
	}
//	if (checkSMSreturn(SMS_CMD_CTRL_OFF))	{
//		SMSreturn(SMS_CMD_CTRL_OFF);
//	}
//	if (checkSMSreturn(SMS_CMD_CTRL_INC))	{
//		SMSreturn(SMS_CMD_CTRL_INC);
//	}
	if (checkSMSreturn(SMS_CMD_CTRL_DEC))	{
		SMSreturn(SMS_CMD_CTRL_DEC);
		triggerSMSreturn(SMS_CMD_CTRL_DEC, SMS_CMD_DISABLE);
	}
//	if (checkSMSreturn(SMS_CMD_CTRL_CALIB))	{
//		SMSreturn(SMS_CMD_CTRL_CALIB);
//	}
	if (checkSMSreturn(SMS_CMD_GET_STATUS))	{
		SMSreturn(SMS_CMD_GET_STATUS);
		triggerSMSreturn(SMS_CMD_GET_STATUS, SMS_CMD_DISABLE);
	}
	if (checkSMSreturn(SMS_CMD_GET_STATION))	{
		SMSreturn(SMS_CMD_GET_STATION);
		triggerSMSreturn(SMS_CMD_GET_STATION, SMS_CMD_DISABLE);
	}
	if (checkSMSreturn(SMS_CMD_GET_SENSOR))	{
		SMSreturn(SMS_CMD_GET_SENSOR);
		triggerSMSreturn(SMS_CMD_GET_SENSOR, SMS_CMD_DISABLE);
	}
}

