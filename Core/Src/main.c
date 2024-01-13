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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "App_MQTT.h"
#include <stdio.h>
//#include "SIM.h"
#include "Serial_CFG.h"
//#include "MQTT.h"
//#include <stdlib.h>
//#include <time.h>
//#include "ds3231.h"
//#include "ServerMessage.h"
//#include "stationCMD.h"
//#include "Serial_log.h"
//#include "crc32.h"
#include "App_Serial.h"

#include "flash_storage.h"
#include "Lora.h"
#include "Task.h"
#include "App_MCU.h"
#include "App_SMS.h"
//#include "user_lcd1604.h"
#include "button.h"
//#include "Contactor.h"

#include "App_Display.h"
#include "App_MBA_stepmor.h"


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
SMS_t mySMS;


uint8_t isDataAvailable_CFG =0;
uint32_t rxFlashdata;

// Create list of sensor node
s_list* SSnode_list;

Station_t myStation = STATION_T_INIT;

_RTC myRTC ;

uint8_t RTC_buffer[20];

GPS_t myGPS;

DISPLAY_MODE_t myDisplayMode = HOME;

extern uint8_t alarmflag;

uint32_t volatile  tmpadc= 0;
uint16_t volatile  adccount = 0;
uint8_t debug = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void currentConvert(uint32_t volatile *adcval);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1 )
	{
		SIM_callback(Size);
	}
	if (huart->Instance== __SCFG_UART->Instance)
	{
		Serial_CFG_Callback(Size);
	}

	if (huart->Instance == USART3) {
		Lora_callback(Size);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == RTC_ALARM_TRIGGER_Pin)	{
//		alarmflag = 0;
		// turn OFF MBA
		myStation.MBAstate = switchContactor(MBA_OFF);
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		DS3231_ClearAlarm1();
		// Change to Calib mode
		setStationMode(STATION_MODE_CALIB);
		triggerTaskflag(TASK_START_CALIB, FLAG_EN);
	}
	if (GPIO_Pin == BUTTON_MENU_Pin) {
		// Button Menu handler
		buttonMENU_handler();
	}

	if (GPIO_Pin == BUTTON_OK_Pin) {
		// Button OK handler
		buttonOK_handler();
	}

	if (GPIO_Pin == BUTTON_UP_Pin) {
		// Button UP handler
		buttonUP_handler();
	}

	if (GPIO_Pin == BUTTON_DOWN_Pin) {
		// Button DOWN handler
		buttonDOWN_handler();
	}

	if (GPIO_Pin == LIMIT_SWITCH_MAX_Pin) {
		// Button Limit MAX handler
		SW_LIMIT_MAX_handler();
	}

	if (GPIO_Pin == LIMIT_SWITCH_MIN_Pin) {
		// Button Limit MIN handler
		SW_LIMIT_MIN_handler();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == hadc1.Instance)	{
		tmpadc += HAL_ADC_GetValue(hadc);
		adccount++;
		currentConvert(&tmpadc);
	}
}
void setStationMode(Station_Mode_t mode)
{
	myStation.StMODE = mode;
}

Station_Mode_t checkStationMode()
{
	return myStation.StMODE;
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
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  SSnode_list =list_create();
  /* For Emulator only */
  uint8_t Sensorcalibvalue1 [101];
  for (uint8_t i =0 ; i < 100 ; i++)	{
	  Sensorcalibvalue1[i] = rand() % 255 ;
  }
  SensorNode_t firstSensornode = {0x51, 0, V_p, 10, SENSOR_ACTIVE , 1 };
  memcpy(firstSensornode.dataCalibBuffer, Sensorcalibvalue1, 100);
  list_append(SSnode_list, firstSensornode);

  uint8_t Sensorcalibvalue2 [101];
  for (uint8_t i =0 ; i < 100 ; i++)	{
	  Sensorcalibvalue2[i] = rand() % 255 ;
  }
  SensorNode_t secondSensornode = {0x52, 0, V_p, 10, SENSOR_ACTIVE , 1};
  memcpy(secondSensornode.dataCalibBuffer, Sensorcalibvalue2, 100);
  list_append(SSnode_list, secondSensornode);
  /**********************************************************************/

  myStation.ssNode_list = SSnode_list;

  // Get station ID from flash
  myStation.stID = (uint8_t)Flash_Read_NUM(FLASH_PAGE_127);

	mySIM.mqttServer.host = "tcp://broker.hivemq.com";
	mySIM.mqttServer.port = 1883;
	mySIM.mqttServer.willtopic = "unnormal_disconnect";
	mySIM.mqttServer.willmsg = (uint8_t*)malloc(sizeof(myStation.stID));
	sprintf((char*)mySIM.mqttServer.willmsg,"%d",myStation.stID);
	mySIM.mqttClient.keepAliveInterval = 10;
	mySIM.mqttClient.clientID = (char*)malloc(sizeof(myStation.stID));
	sprintf((char*)mySIM.mqttClient.clientID,"%d",myStation.stID);
	mySIM.mqttReceive.qos =1;
	mySIM.mqttServer.connect=0;

	mySIM.sms = mySMS;

	// ADC
	HAL_ADC_Start_IT(&hadc1);


	initTask(&myStation);

	// MBA and Step motor
	initApp_MBA_stepmor();
	// Lora
//	initLora(&huart2, &hdma_usart2_rx);
	initmyLora();
	// GPS
	myGPS.getFlag = 0;
	initGPS(&myStation, &myRTC);

	initSIM();

	// Init Serial log
//	init_Serial_log(&huart2);

	// Init MQTT app
	initApp_MQTT();

//	initApp_MCU(&mySIM);

	initApp_SMS(&mySIM.sms);

	init_App_Serial();

	// Init RTC module (DS3231)
	DS3231_Init(&hi2c1);



	initButton(&myDisplayMode);


	initApp_Display(&myDisplayMode, &myRTC);

	//	__MY_DISPLAY_MODE = displaymode;
	//	__MY_SS_LIST = mylist;
	//	__MY_STATION = myStation;

//DS3231_GetTime(&myRTC);
//	myRTC.Year = 24;
//	myRTC.Month = 1;
//	myRTC.Date = 11;
//	myRTC.DaysOfWeek = 5;
//	myRTC.Hour = 15;
//	myRTC.Min = 17;
//	myRTC.Sec = 0;
//	DS3231_SetTime(&myRTC);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  testLora_receive();
//	  testProcessingMsg();
//	  Serial_log_testOperation();
//	  SIM_checkOperation();
//	  MQTT_testReceive();
//	  LCD_GotoXY(1, 1);
//	  LCD_Print("hello");
	  processApp_MCU();
	  processApp_MQTT();
	  processingApp_display();
	  processingApp_MBA_stepmor();

	  processApp_SMS();
//	  LCD_PrintNumber(5);
//	  processing_CMD(&myStation.stID);
//	  testSynchronize();
//	  DS3231_GetTime(&myRTC);

//	  testSMS();
//	  myStation.getGPStimeflag = 1;
//	  HAL_GPIO_TogglePin(MBA_CONTACTOR_GPIO_Port, MBA_CONTACTOR_Pin);
	  HAL_Delay(200);
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

static void currentConvert(uint32_t volatile *adcval)
{
	if (adccount == 1000)	{
		myStation.stCurrent = (uint16_t)( ( ( (*adcval)*3/4095) + 50)/6);
		*adcval = 0;
		adccount = 0;
	}
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
