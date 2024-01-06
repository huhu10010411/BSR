/**
  * @file    user_a4988.c
  * @author  Sagit
  * @date    Dec 10, 2023
  * @brief   
  */

/* INCLUDES ------------------------------------------------------------------*/
#include "user_a4988.h"

/* EXTERN VARIABLES ----------------------------------------------------------*/
StructManageMotorA4988 myMotor	=
{
		.DIR		= MOTOR_DIR_Pin,
		.PDIR		= MOTOR_DIR_GPIO_Port,
		.STEP		= MOTOR_STEP_Pin,
		.PSTEP		= MOTOR_STEP_GPIO_Port,
		.ENABLE		= MOTOR_EN_Pin,
		.PENABLE	= MOTOR_EN_GPIO_Port,
		.SLEEP		= MOTOR_SLEEP_Pin,
		.PSLEEP		= MOTOR_SLEEP_GPIO_Port,
		.RESET		= MOTOR_RST_Pin,
		.PRESET		= MOTOR_RST_GPIO_Port,
};
/* PRIVATE MACROS AND DEFINES ------------------------------------------------*/

/* PRIVATE TYPEDEFS ----------------------------------------------------------*/

/* STATIC VARIABLES ----------------------------------------------------------*/

/* GLOBAL VARIABLES ----------------------------------------------------------*/

/* STATIC FUNCTION PROTOTYPES --- --------------------------------------------*/

/* STATIC FUNCTIONS ----------------------------------------------------------*/

/* GLOBAL FUNCTIONS ----------------------------------------------------------*/
void A4988_Init()
{

}

void A4988_EnableDriver()
{
	HAL_GPIO_WritePin(myMotor.PENABLE, myMotor.ENABLE, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(myMotor.PSLEEP, myMotor.SLEEP, GPIO_PIN_SET);
	HAL_GPIO_WritePin(myMotor.PRESET, myMotor.RESET, GPIO_PIN_SET);
}

void A4988_DisableDriver()
{
	HAL_GPIO_WritePin(myMotor.PENABLE, myMotor.ENABLE, GPIO_PIN_SET);
	HAL_GPIO_WritePin(myMotor.PSLEEP, myMotor.SLEEP, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(myMotor.PRESET, myMotor.RESET, GPIO_PIN_RESET);
}

void A4988_SetDirection(int direction)
{
	if (direction > 0)
	{
		HAL_GPIO_WritePin(myMotor.PDIR, myMotor.DIR, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(myMotor.PDIR, myMotor.DIR, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(myMotor.PSTEP, myMotor.STEP, GPIO_PIN_SET);
	HAL_GPIO_WritePin(myMotor.PSTEP, myMotor.STEP, GPIO_PIN_RESET);
}

void A4988_StepByStep(int steps, int direction)
{
	if (direction > 0) {
		HAL_GPIO_WritePin(myMotor.PDIR, myMotor.DIR, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(myMotor.PDIR, myMotor.DIR, GPIO_PIN_RESET);
	}

	// Step the motor
	for (int i = 0; i < steps; i++) {
		HAL_GPIO_WritePin(myMotor.PSTEP, myMotor.STEP, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(myMotor.PSTEP, myMotor.STEP, GPIO_PIN_RESET);
		HAL_Delay(1);
	}
}

void A4988_StepByDegrees(float degrees, int direction)
{
//	int steps = (int)(degrees * DEGREES_PER_STEP / 360.0);
//	A4988_StepByStep(config, steps, direction);
}

void A4988_ResetDriver()
{
	HAL_GPIO_WritePin(myMotor.PRESET, myMotor.RESET, GPIO_PIN_RESET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(myMotor.PRESET, myMotor.RESET, GPIO_PIN_SET);
}

/* End of file ----------------------------------------------------------------*/
