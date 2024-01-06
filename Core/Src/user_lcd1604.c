/**
  * @file    user_lcd1604.c
  * @author  Sagit
  * @date    Dec 10, 2023
  * @brief   
  */

/* INCLUDES ------------------------------------------------------------------*/
#include "user_lcd1604.h"
#include <stdio.h>
#include <string.h>

/* EXTERN VARIABLES ----------------------------------------------------------*/
StructManageLCD1604		myLCD	=
{
		.D4		= LCD_DB4_Pin,
		.D4P	= LCD_DB4_GPIO_Port,
		.D5		= LCD_DB5_Pin,
		.D5P    = LCD_DB5_GPIO_Port,
		.D6		= LCD_DB6_Pin,
		.D6P	= LCD_DB6_GPIO_Port,
		.D7		= LCD_DB7_Pin,
		.D7P	= LCD_DB7_GPIO_Port,
		.EN		= LCD_EN_Pin,
		.ENP	= LCD_EN_GPIO_Port,
		.RS		= LCD_RS_Pin,
		.RSP	= LCD_RS_GPIO_Port,
};

/* PRIVATE MACROS AND DEFINES ------------------------------------------------*/

/* PRIVATE TYPEDEFS ----------------------------------------------------------*/

/* STATIC VARIABLES ----------------------------------------------------------*/

/* GLOBAL VARIABLES ----------------------------------------------------------*/

/* STATIC FUNCTION PROTOTYPES --- --------------------------------------------*/

/* STATIC FUNCTIONS ----------------------------------------------------------*/

/* GLOBAL FUNCTIONS ----------------------------------------------------------*/
void LCD_Init(void){
	  HAL_Delay(50);
	  LCD_SendCommand(0x03);
	  HAL_Delay(5);
	  LCD_SendCommand(0x03);
	  HAL_Delay(1);
	  LCD_SendCommand(0x03);
	  HAL_Delay(1);
	  LCD_SendCommand(0x02);
	  HAL_Delay(1);

	  LCD_SendCommand(0x28);
	  HAL_Delay(1);

	  LCD_SendCommand(0x0C);
	  HAL_Delay(1);

	  LCD_SendCommand(0x01);
	  HAL_Delay(2);

	  LCD_SendCommand(0x06);
	  HAL_Delay(1);
}

void LCD_SendCommand(uint8_t cmd){
	  // Send high nibble
	  HAL_GPIO_WritePin(myLCD.RSP, myLCD.RS, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D4P, myLCD.D4, (cmd & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D5P, myLCD.D5, (cmd & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D6P, myLCD.D6, (cmd & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D7P, myLCD.D7, (cmd & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_SET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_RESET);

	  // Send low nibble
	  HAL_GPIO_WritePin(myLCD.D4P, myLCD.D4, (cmd & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D5P, myLCD.D5, (cmd & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D6P, myLCD.D6, (cmd & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D7P, myLCD.D7, (cmd & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_SET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_RESET);
}

void LCD_SendData(uint8_t data){
	  // Send high nibble
	  HAL_GPIO_WritePin(myLCD.RSP, myLCD.RS, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(myLCD.D4P, myLCD.D4, (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D5P, myLCD.D5, (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D6P, myLCD.D6, (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D7P, myLCD.D7, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_SET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_RESET);

	  // Send low nibble
	  HAL_GPIO_WritePin(myLCD.D4P, myLCD.D4, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D5P, myLCD.D5, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D6P, myLCD.D6, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.D7P, myLCD.D7, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_SET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(myLCD.ENP, myLCD.EN, GPIO_PIN_RESET);
}

void LCD_GotoXY(uint8_t x, uint8_t y){
    uint8_t address;
    if (y == 0) {
        address = 0x80 + x;
    } else {
        address = 0xC0 + x;
    }
    LCD_SendCommand(address);
}

void LCD_Print(char *str){
    while (*str) {
        LCD_SendData(*str++);
    }
}

void LCD_PrintNumber(uint16_t number)
{
	uint8_t tmpbuffer[10];
	memset(tmpbuffer, 0, 10);
	sprintf((char*)tmpbuffer, "%d", number);
	LCD_Print((char*)tmpbuffer);
}
void LCD_Clear(){
	LCD_SendCommand(0x01);
}

void LCD_ClearXY(uint8_t x, uint8_t y)
{
	LCD_GotoXY(x, y);
	LCD_Print(" ");
}
void LCD_ClearRow(uint8_t row)
{
	for (uint8_t i = 0; i < MAX_COLUMN; i++ )	{
		LCD_ClearXY(i, row);
	}

}
/* End of file ----------------------------------------------------------------*/
