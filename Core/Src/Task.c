/*
 * Task.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#include "Task.h"


static Station_t *__MY_STATION;

void initTask(Station_t *station)
{
	__MY_STATION = station;
}

bool checkTaskflag (Task_t task)
{
	bool res = false;
	switch (task) {
	case TASK_REGISTER:
		res = (bool)__MY_STATION->task.register2server ;
		break;
	case TASK_SUBSCRIBE:
		res = (bool)__MY_STATION->task.subscribe;
		break;
	case TASK_SEND_MBA_STATUS:
		res = (bool)__MY_STATION->task.sendMBAstatus;
		break;
	case TASK_CTRL_STEPMOR:
		res = (bool)__MY_STATION->task.CtrlStepmotor;
		break;
	case TASK_PREPARE_CALIB:
		res = (bool)__MY_STATION->task.PrepareCalib;
		break;
	case TASK_START_CALIB:
		res = (bool)__MY_STATION->task.StartCalib;
		break;
	case TASK_GET_GPS_TIME:
		res = (bool)__MY_STATION->task.getGPStimeflag;
		break;
	case TASK_SEND_NWREADY:
		res = (bool)__MY_STATION->task.sendNWready;
		break;
	case TASK_SEND_DATACALIB:
		res = (bool)__MY_STATION->task.sendDATACALIB;
		break;
	case TASK_SEND_DATAPERIOD:
		res = (bool)__MY_STATION->task.sendDataPeriod;
		break;
	case TASK_SEND_DATAAFTERCALIB:
		res = (bool)__MY_STATION->task.sendDATAafterCALIB;
		break;
	case TASK_SEND_CTRLMBA:
		res = (bool)__MY_STATION->task.sendcmdCtrlMBA;
		break;
	case TASK_SEND_CTRLSTEPMOR:
		res = (bool)__MY_STATION->task.sendcmdCtrlStepmotor;
		break;
	case TASK_SEND_CALIB:
		res = (bool)__MY_STATION->task.sendcmdCalib;
		break;
	case TASK_SEND_GETSTATUS:
		res = (bool)__MY_STATION->task.sendcmdGetstatus;
		break;
	case TASK_SEND_GETSTATION:
		res = (bool)__MY_STATION->task.sendcmdGetstation;
		break;
	case TASK_SEND_GETSENSOR:
		res = (bool)__MY_STATION->task.sendcmdGetsensor;
		break;
	case TASK_SEND_STEP_LIMIT:
		res = (bool)__MY_STATION->task.sendSTEPLIMIT;
		break;
	default:
		break;
	}
	return res;
}



void triggerTaskflag (Task_t task, Flag_t ENorDIS)
{
	switch (task) {
	case TASK_REGISTER:
		__MY_STATION->task.register2server = ENorDIS;
		break;
	case TASK_SUBSCRIBE:
		__MY_STATION->task.subscribe = ENorDIS;
		break;
	case TASK_SEND_MBA_STATUS:
		__MY_STATION->task.sendMBAstatus = ENorDIS;
		break;
	case TASK_CTRL_STEPMOR:
		__MY_STATION->task.CtrlStepmotor = ENorDIS;
		break;
	case TASK_PREPARE_CALIB:
		__MY_STATION->task.PrepareCalib = ENorDIS;
		break;
	case TASK_START_CALIB:
		__MY_STATION->task.StartCalib = ENorDIS;
		break;
	case TASK_GET_GPS_TIME:
		__MY_STATION->task.getGPStimeflag = ENorDIS;
		break;
	case TASK_SEND_NWREADY:
		__MY_STATION->task.sendNWready = ENorDIS;
		break;
	case TASK_SEND_DATACALIB:
		__MY_STATION->task.sendDATACALIB = ENorDIS;
		break;
	case TASK_SEND_DATAPERIOD:
		__MY_STATION->task.sendDataPeriod = ENorDIS;
		break;
	case TASK_SEND_DATAAFTERCALIB:
		__MY_STATION->task.sendDATAafterCALIB = ENorDIS;
		break;
	case TASK_SEND_CTRLMBA:
		__MY_STATION->task.sendcmdCtrlMBA = ENorDIS;
		break;
	case TASK_SEND_CTRLSTEPMOR:
		__MY_STATION->task.sendcmdCtrlStepmotor = ENorDIS;
		break;
	case TASK_SEND_CALIB:
		__MY_STATION->task.sendcmdCalib = ENorDIS;
		break;
	case TASK_SEND_GETSTATUS:
		__MY_STATION->task.sendcmdGetstatus = ENorDIS;
		break;
	case TASK_SEND_GETSTATION:
		__MY_STATION->task.sendcmdGetstation = ENorDIS;
		break;
	case TASK_SEND_GETSENSOR:
		__MY_STATION->task.sendcmdGetsensor = ENorDIS;
		break;
	case TASK_SEND_STEP_LIMIT:
		__MY_STATION->task.sendSTEPLIMIT = ENorDIS;
	default:
		break;
	}
	return ;
}





