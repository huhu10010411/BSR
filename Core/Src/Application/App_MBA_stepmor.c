/*
 * App_MBA_stepmor.c
 *
 *  Created on: Jan 12, 2024
 *      Author: Admin
 */
//#include "button.h"
#include "Task.h"
#include "Step_motor.h"
#include "user_a4988.h"

static Stepmotor_change_mode_t changeMode;
void initApp_MBA_stepmor()
{

}

void processingApp_MBA_stepmor()
{
	if(checkTaskflag(TASK_CTRL_STEPMOR))	{
		changeMode = Step_getChangemode();
		switch (changeMode)	{
		case STEPM_MODE_STEP:
			Step_step();
			break;
		case STEPM_MODE_PERCENTAGE:
			Step_percent();
			break;
		default:
			break;
		}
		triggerTaskflag(TASK_CTRL_STEPMOR, FLAG_DIS);
	}
}
