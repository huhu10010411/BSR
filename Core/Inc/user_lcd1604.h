/**
  * @file    user_lcd1604.h
  * @author  Sagit
  * @date    Dec 10, 2023
  * @brief   
  */

#ifndef USER_LCD1604_H_
#define USER_LCD1604_H_

/* INCLUDES ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "user_delay.h"

#define MAX_COLUMN		15
#define MAX_ROW			3

/* MACROS AND DEFINES --------------------------------------------------------*/
typedef struct
{
	uint16_t		D4, D5, D6, D7, EN, RS;
	GPIO_TypeDef* 	D4P;
	GPIO_TypeDef*  	D5P;
	GPIO_TypeDef*	D6P;
	GPIO_TypeDef*	D7P;
	GPIO_TypeDef*	ENP;
	GPIO_TypeDef*	RSP;
} StructManageLCD1604;

/* TYPEDEFS ------------------------------------------------------------------*/

/* EXTERNAL VARIABLES --------------------------------------------------------*/

/* GLOBAL FUNCTION PROTOTYPES ------------------------------------------------*/
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_GotoXY(uint8_t x, uint8_t y);
void LCD_Print(char *str);
void LCD_PrintNumber(uint16_t number);
void LCD_ClearXY(uint8_t x, uint8_t y);
void LCD_ClearRow(uint8_t row);
void LCD_Clear();

#endif /*USER_LCD1604_H_*/

/* End of file ----------------------------------------------------------------*/
