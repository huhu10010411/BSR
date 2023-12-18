/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "linkedlist.h"
#include "SIM.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
	 uint8_t volatile ID;
	 uint8_t volatile sID;
}ID_t;

typedef struct {
	uint8_t stID;
	uint16_t stCurrent;
	uint16_t stVoltage;
	s_list  * ssNode_list;
	uint8_t subcribeStatus;
}Station_t;
extern Station_t myStation;
extern SIM_t mySIM;
extern ID_t myID;
extern s_list* SSnode_list;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define ACTIVE		0x01
#define DEACTIVE	0x02

#define STATION_T_INIT	{8, 300, 0, NULL, 0}
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define MOTOR_SENSE1_Pin GPIO_PIN_0
#define MOTOR_SENSE1_GPIO_Port GPIOA
#define MOTOR_SENSE2_Pin GPIO_PIN_1
#define MOTOR_SENSE2_GPIO_Port GPIOA
#define SIM_TX_Pin GPIO_PIN_6
#define SIM_TX_GPIO_Port GPIOB
#define SIM_RX_Pin GPIO_PIN_7
#define SIM_RX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
