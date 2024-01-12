/*
 * Step_motor.h
 *
 *  Created on: Jan 12, 2024
 *      Author: Admin
 */

#ifndef INC_DEVICE_STEP_MOTOR_H_
#define INC_DEVICE_STEP_MOTOR_H_

#include "stm32f1xx.h"

typedef enum {
	LIMIT_MIN,
	LIMIT_MAX,
	LIMIT_NONE
}LIMIT_t;

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

LIMIT_t getLimit();

void setLimit(LIMIT_t Limit);

Stepmotor_dir_t Step_getDir();

void Step_setDir(Stepmotor_dir_t dir);

uint16_t getStepchange();

void decreaseStepchange();

void increaseStepchange();

void Step_setChangeMode(Stepmotor_change_mode_t mode);

Stepmotor_change_mode_t Step_getChangemode();

void Step_percent();

void Step_step();



#endif /* INC_DEVICE_STEP_MOTOR_H_ */
