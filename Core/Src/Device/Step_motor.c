/*
 * Step_motor.c
 *
 *  Created on: Jan 12, 2024
 *      Author: Admin
 */
#include "Step_motor.h"
#include "user_a4988.h"

#define MAX_STEP	100

static LIMIT_t limit = LIMIT_NONE;
static Stepmotor_dir_t step_dir = STEPM_DIR_INC;
static uint16_t volatile stepChange = 0;
static uint8_t steppercentChange = 0;
static Stepmotor_change_mode_t stepMode = STEPM_MODE_STEP;


void setLimit(LIMIT_t Limit)
{
	limit = Limit;
}

LIMIT_t getLimit ()
{
	return limit;
}

Stepmotor_dir_t Step_getDir()
{
	return step_dir;
}

void Step_setDir(Stepmotor_dir_t dir)
{
	step_dir = dir;
}

void increaseStepchange()
{
	if (stepChange >= MAX_STEP )	{
		stepChange = 0;
	}
	else {
		stepChange ++;
	}
}

void decreaseStepchange()
{
	if (stepChange == 0  )	{
		stepChange = MAX_STEP;
	}
	else {
		stepChange--;
	}
}

uint16_t getStepchange()
{
	return stepChange;
}

uint8_t Step_getPercentChange()
{
	return steppercentChange;
}

void Step_setPercentChange(uint8_t percent)
{
	steppercentChange = percent;
}

void Step_setChangeMode(Stepmotor_change_mode_t mode)
{
	stepMode = mode;
}

Stepmotor_change_mode_t Step_getChangemode()
{
	return stepMode;
}

void Step_step()
{
	A4988_StepByStep(getStepchange(), (uint8_t)Step_getDir() );
}

void Step_percent()
{
	uint16_t stepChange = (uint16_t)(Step_getPercentChange() * MAX_STEP / 100);
	A4988_StepByStep(stepChange, Step_getDir());
}
