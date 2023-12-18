/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "App_MQTT.h"
#include <stdio.h>
#include "SIM.h"
#include "Serial_CFG.h"
#include "MQTT.h"
#include <stdlib.h>
#include <time.h>
#include "ds3231.h"
#include "ServerMessage.h"
#include "stationCMD.h"
#include "Serial_log.h"
#include "crc32.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define FLASH_ID_ADDR  0x0801FC00

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


SIM_t mySIM;
SMS_t mySMS= {0};

uint8_t isDataAvailable_CFG =0;
uint32_t rxFlashdata;

// Create list of sensor node
s_list* SSnode_list;

Station_t myStation = STATION_T_INIT;

_RTC myRTC ;

uint8_t RTC_buffer[20];



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ReadIDfromFlash(uint8_t *myID);
void WriteIDtoFlash(uint8_t *myID);







/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == __SIM_UART->Instance )
	{
		SIM_callback(Size);
	}
	if (huart->Instance== __SCFG_UART->Instance)
	{
		isDataAvailable_CFG =1;
		Serial_CFG_Callback(Size);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  SSnode_list =list_create();
  SensorNode_t firstSensornode = {0x51, 0, V_p, 0, SENSOR_ACTIVE};
  list_append(SSnode_list, firstSensornode);
  myStation.ssNode_list = SSnode_list;
//   ReadIDfromFlash(&myStation.stID);

   mySIM.mqttServer.host = "tcp://broker.hivemq.com";
   mySIM.mqttServer.port = 1883;
   mySIM.mqttServer.willtopic = "unnormal_disconnect";
   mySIM.mqttServer.willmsg = (uint8_t*)malloc(sizeof(myStation.stID));
   sprintf((char*)mySIM.mqttServer.willmsg,"%d",myStation.stID);
   mySIM.mqttClient.keepAliveInterval = 120;
   mySIM.mqttClient.clientID = (char*)malloc(sizeof(myStation.stID));
   sprintf((char*)mySIM.mqttClient.clientID,"%d",myStation.stID);
   mySIM.mqttReceive.qos =1;
   mySIM.mqttServer.connect=0;

//  initSerial_CFG(&huart2,&hdma_usart2_rx);
  initSIM(&huart1, &hdma_usart1_rx, &mySIM);

  // Init Serial log
  init_Serial_log(&huart2);

  // Init MQTT app
  initApp_MQTT();

  // Init RTC module (DS3231)
  DS3231_Init(&hi2c1);

//  myRTC.Year = 23;
//  myRTC.Month = 12;
//  myRTC.Date = 17;
//  myRTC.Hour = 15;
//  myRTC.Min = 21;
//  myRTC.Sec = 0;
//  DS3231_SetTime(&myRTC);

  uint8_t crcbuffer [10];
  crcbuffer[0] = 0xF1;
  crcbuffer[1] = 0x12;
  crcbuffer[2] = 0x02;
  crcbuffer[3] = 0x8A;
  crcbuffer[4] = 0x32;
  crcbuffer[5] = 0x47;
  crcbuffer[6] = 0x2F;
  crcbuffer[7] = 0x54;
  crcbuffer[8] = 0x8F;
  crcbuffer[9] = 0x05;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  if( isDataAvailable_CFG)
//	  {
//		  isDataAvailable_CFG =0;
//		  processing_CMD(&myStation.stID);
//		  MarkAsReadData_CFG();
//		  WriteIDtoFlash(&myStation.stID);
//	  }
//	   if (mySIM.mqttServer.connect)
//	   {
//		   HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//		  if (! MQTT_publish((uint8_t*)"testbsr", (uint8_t*)"hellobsr", 8))
//		  {
//			  mySIM.mqttServer.connect=0;
//			  MQTT_connect(&mySIM);
//		  }
//		  Register2Server(myStation);
//	   }
//	   else
//	   {
//		   MQTT_connect(&mySIM);
//	   }

//	   if (newSMS)
//	   {
//		   newSMS = 0;
//		   if (processingSMS(&mySMS))
//		   {
//			   if (mySMS.cmd != SMS_NO_CMD)
//			   {
//				   if (!mySIM.mqttServer.connect)
//				   {
//					   MQTT_connect(&mySIM);
//				   }
//
////				   sendCMDtoServer(mySMS.cmd);
//			   }
//
//		   }
//	   }
//	  Serial_log_testOperation();
//	  SIM_checkOperation();
//	  MQTT_testReceive();

//		0x3F67D17C
//	 if ( DS3231_GetTime(&myRTC) ) {
//		 Serial_log_string("get RTC success\r\n");
//	 }
//	  uint8_t len = sprintf( (char*)RTC_buffer, "%d/%d/%d %d:%d:%d\r\n",
//			  myRTC.Date, myRTC.Month, myRTC.Year, myRTC.Hour, myRTC.Min, myRTC.Sec);
//	  Serial_log_buffer(RTC_buffer, len);
//	  processApp_MQTT();
	  HAL_Delay(5000);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void WriteIDtoFlash(uint8_t *myID)
{
	uint32_t tmp =0;
	tmp |= *myID;
	Flash_Write_Data(FLASH_ID_ADDR,&tmp,1);
}
void ReadIDfromFlash(uint8_t *myID)
{
	uint32_t tmp;
	uint8_t tmp2;
	Flash_Read_Data(FLASH_ID_ADDR, &tmp, 1);
	tmp2 =(uint8_t)(tmp & 0xFF);
	*myID = tmp2;
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
