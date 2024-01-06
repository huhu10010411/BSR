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

typedef enum {
	STATION_MODE_NORMAL,
	STATION_MODE_CALIB,
	STATION_MODE_CONFIG
}Station_Mode_t;



typedef struct {
	uint8_t register2server : 1;
	uint8_t subscribe : 1;
	uint8_t PrepareCalib: 1;
	uint8_t StartCalib : 1;
	uint8_t CtrlMBA : 1;
	uint8_t CtrlStepmotor : 1;
	uint8_t getGPStimeflag : 1;
	uint8_t sendDataPeriod :1;
	uint8_t sendNWready :1;
	uint8_t sendDATACALIB :1;
	uint8_t sendDATAafterCALIB : 1;
	uint8_t sendcmdCtrlMBA : 1;
	uint8_t sendcmdCtrlStepmotor : 1;
	uint8_t sendcmdCalib : 1;
	uint8_t sendcmdGetstatus : 1;
	uint8_t sendcmdGetstation : 1;
	uint8_t sendcmdGetsensor : 1;
}Task_handle_t;

#define TASK_FLAG_INIT	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

typedef struct {
	Station_Mode_t StMODE ;
	uint8_t stID;
	uint16_t stCurrent;
	uint16_t stVoltage;
	s_list  * ssNode_list;
	Task_handle_t task;
}Station_t;

//extern Station_t myStation;
//extern SIM_t mySIM;
//extern s_list* SSnode_list;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define STATION_T_INIT	{STATION_MODE_NORMAL, 8, 300, 20, NULL, TASK_FLAG_INIT}

#define ACTIVE		0x01
#define DEACTIVE	0x02

#define MAX_ADC_VALUE	4095

//#define CMD_TRIGGER	 1
//#define CMD_IDLE	 0


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void setStationMode(Station_Mode_t mode);
Station_Mode_t checkStationMode();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_DB7_Pin GPIO_PIN_13
#define LCD_DB7_GPIO_Port GPIOC
#define LCD_DB6_Pin GPIO_PIN_14
#define LCD_DB6_GPIO_Port GPIOC
#define LCD_DB5_Pin GPIO_PIN_15
#define LCD_DB5_GPIO_Port GPIOC
#define RTC_ALARM_TRIGGER_Pin GPIO_PIN_0
#define RTC_ALARM_TRIGGER_GPIO_Port GPIOA
#define RTC_ALARM_TRIGGER_EXTI_IRQn EXTI0_IRQn
#define MOTOR_SENSE2_Pin GPIO_PIN_1
#define MOTOR_SENSE2_GPIO_Port GPIOA
#define MBA_CONTACTOR_Pin GPIO_PIN_4
#define MBA_CONTACTOR_GPIO_Port GPIOA
#define MOTOR_DIR_Pin GPIO_PIN_7
#define MOTOR_DIR_GPIO_Port GPIOA
#define MOTOR_STEP_Pin GPIO_PIN_0
#define MOTOR_STEP_GPIO_Port GPIOB
#define MOTOR_SLEEP_Pin GPIO_PIN_1
#define MOTOR_SLEEP_GPIO_Port GPIOB
#define MOTOR_RST_Pin GPIO_PIN_12
#define MOTOR_RST_GPIO_Port GPIOB
#define MOTOR_EN_Pin GPIO_PIN_13
#define MOTOR_EN_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_10
#define LED_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_11
#define LCD_RS_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_12
#define LCD_EN_GPIO_Port GPIOA
#define LCD_DB4_Pin GPIO_PIN_15
#define LCD_DB4_GPIO_Port GPIOA
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
