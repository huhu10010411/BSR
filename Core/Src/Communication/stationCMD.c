/*
 * stationCMD.c
 *
 *  Created on: Dec 10, 2023
 *      Author: Admin
 */

#include "stationCMD.h"


uint8_t CMD_SMS_calib (uint8_t stationID, uint16_t timedelay, uint8_t *phone_numb)
{
	if (sendCMDtoServer(stationID, CMD_SMS_CALIB, phone_numb, timedelay, NULL, MBA_NULL,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
		return 1;
	return 0;
}

uint8_t CMD_SMS_getStatus (uint8_t stationID, uint8_t *phone_numb)
{
	if (sendCMDtoServer(stationID, CMD_SMS_GETSTATUS, phone_numb, 0, NULL, MBA_NULL,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
		return 1;
	return 0;
}

uint8_t CMD_SMS_Ctrl_MBA (uint8_t stationID, uint8_t *phone_numb, uint16_t timedelay, uint8_t *StationIDlist, MBA_state_t MBAstate)
{
	if (sendCMDtoServer(stationID, CMD_CTRL_MBA, phone_numb, timedelay, StationIDlist, MBAstate,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
			return 1;
		return 0;
}

uint8_t CMD_SMS_Ctrl_StepM (uint8_t stationID, uint8_t *phone_numb, uint16_t timedelay, uint8_t *StationIDlist, Stepmotor_dir_t stepDIR, Stepmotor_change_mode_t stepMode, uint8_t changeValue)
{
	if (sendCMDtoServer(stationID, CMD_CTRL_STEP_MOTOR, phone_numb, timedelay, StationIDlist, MBA_NULL,stepDIR , stepMode, changeValue) )
			return 1;
		return 0;
}

uint8_t CMD_SMS_getStationLatestData (uint8_t stationID, uint8_t *phone_numb, uint8_t *StationIDlist)
{
	if (sendCMDtoServer(stationID, CMD_SMS_GETSTATUS, phone_numb, 0, StationIDlist, MBA_NULL,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
		return 1;
	return 0;
}

uint8_t CMD_SMS_getSensorLatestData (uint8_t stationID, uint8_t *phone_numb, uint8_t *SensorIDlist)
{
	if (sendCMDtoServer(stationID, CMD_SMS_GETSTATUS, phone_numb, 0, SensorIDlist, MBA_NULL,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
		return 1;
	return 0;
}

uint8_t CMD_sleep(uint8_t stationID)
{
	if (sendCMDtoServer(stationID, CMD_SLEEP, NULL, 0, NULL, MBA_NULL,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
		return 1;
	return 0;
}

uint8_t CMD_wakeup(uint8_t stationID)
{
	if (sendCMDtoServer(stationID, CMD_WAKEUP, NULL, 0, NULL, MBA_NULL,STEPM_DIR_DEFAULT , STEPM_MODE_DEFAULT, 0) )
		return 1;
	return 0;
}
