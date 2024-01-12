/**
  * @file    user_screen.c
  * @author  Sagit
  * @date    Jan 7, 2024
  * @brief   
  */

/* INCLUDES ------------------------------------------------------------------*/
#include "user_screen.h"
#include <stdio.h>
#include <string.h>

/* EXTERN VARIABLES ----------------------------------------------------------*/

/* PRIVATE MACROS AND DEFINES ------------------------------------------------*/

/* PRIVATE TYPEDEFS ----------------------------------------------------------*/

/* STATIC VARIABLES ----------------------------------------------------------*/
static _RTC *__MY_RTC;
/* GLOBAL VARIABLES ----------------------------------------------------------*/

/* STATIC FUNCTION PROTOTYPES --- --------------------------------------------*/

/* STATIC FUNCTIONS ----------------------------------------------------------*/

/* GLOBAL FUNCTIONS ----------------------------------------------------------*/
void Screen_Init(_RTC *myRTC)
{
	__MY_RTC = myRTC;
//	DS3231_Init(&hi2c1);
//	__MY_RTC->Year = 24;
//	__MY_RTC->Month = 1;
//	__MY_RTC->Date = 1;
//	__MY_RTC->Hour = 15;
//	__MY_RTC->Min = 17;
//	__MY_RTC->Sec = 0;
//	DS3231_SetTime(__MY_RTC);
	LCD_Init();
	LCD_Clear();
//	HAL_Delay(1000);
//	LCD_GotoXY(0, 0);
//	LCD_Print("Hello!");
}

void Screen_Home_Origin(uint8_t ID)
{
    char buffer[20];
    DS3231_GetTime(__MY_RTC);
//	LCD_Clear();
	LCD_GotoXY(0, 0);
//	LCD_Print("Station Node");
//    LCD_GotoXY(0, 1);
    sprintf(buffer,"Station ID: 0x%02X", ID);
    LCD_Print(buffer);
    LCD_GotoXY(4, 1);
    sprintf(buffer,"%02d/%02d/%d", __MY_RTC->Date, __MY_RTC->Month, __MY_RTC->Year);
    LCD_Print(buffer);
    LCD_GotoXY(4, 2);
    sprintf(buffer,"%02d:%02d:%02d", __MY_RTC->Hour, __MY_RTC->Min, __MY_RTC->Sec);
    LCD_Print(buffer);
    LCD_GotoXY(5, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(6, 3);
    LCD_Print("HOME");
    LCD_GotoXY(10,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Home_Sync(uint8_t time)
{
    char buffer[20];
//	LCD_Clear();
	LCD_GotoXY(1, 0);
	LCD_Print("Received Order");

    LCD_GotoXY(1, 1);
    LCD_Print("OFF after:");
    LCD_GotoXY(6, 2);
    sprintf(buffer, "%03ds", time);
    LCD_Print(buffer);
    LCD_GotoXY(5, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(6, 3);
    LCD_Print("HOME");
    LCD_GotoXY(10,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Home_Sync_Set_Countdown(uint8_t time)
{
	if (time >= 0)
	{
	    char buffer[20];
		LCD_GotoXY(12, 2);
		sprintf(buffer, "%03ds", time);
		LCD_Print(buffer);
	}
}

void Screen_Home_Calib(void)
{
    char buffer[20];
//    DS3231_GetTime(&__MY_RTC);
//	LCD_Clear();
	LCD_GotoXY(1, 0);
	LCD_Print("Received Order");
    LCD_GotoXY(0, 1);
    sprintf(buffer,"SWoff: %02dh%02dm%02ds", __MY_RTC->Hour, __MY_RTC->Min, __MY_RTC->Sec);
    LCD_Print(buffer);
    LCD_GotoXY(0, 2);
    sprintf(buffer,"Date:  %02d/%02d/%d", __MY_RTC->Date, __MY_RTC->Month, __MY_RTC->Year);
    LCD_Print(buffer);
    LCD_GotoXY(5, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(6, 3);
    LCD_Print("HOME");
    LCD_GotoXY(10,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Monitor_Station(uint8_t mode, uint16_t current)
{
    char buffer[20];
	LCD_GotoXY(0, 0);
    LCD_SendData(_LCD_CHAR_UP);
	LCD_GotoXY(1, 0);
	LCD_Print("Station Status");
    LCD_GotoXY(1, 1);
    if (mode == 2)
    {
        LCD_Print("Contactor: OFF");
    } else if (mode == 1){
        LCD_Print("Contactor: ON");
    }
    LCD_GotoXY(1, 2);
    sprintf(buffer,"Current: %04dmA", current);
    LCD_Print(buffer);
    LCD_GotoXY(0, 2);
	LCD_SendData(_LCD_CHAR_DOWN);
    LCD_GotoXY(4, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(5, 3);
    LCD_Print("MONITOR");
    LCD_GotoXY(12,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Monitor_Node(uint8_t ID, uint8_t mode, uint16_t battery, uint8_t type, uint16_t voltage)
{
    char buffer[20];
//	LCD_Clear();
	LCD_GotoXY(0, 0);
    LCD_SendData(_LCD_CHAR_UP);
    LCD_GotoXY(1, 0);
    sprintf(buffer,"Sensor ID: 0x%02X", ID);
    LCD_Print(buffer);
    LCD_GotoXY(1, 1);
    if (mode == 2)
    {
        LCD_Print("Status: OFF");
    } else if (mode == 1){
        LCD_Print("Status: ON");
    }
    LCD_GotoXY(1, 2);
    sprintf(buffer,"Battery: %01d.%02dV", battery/100, battery%100);
    LCD_Print(buffer);
    LCD_GotoXY(0, 3);
	LCD_SendData(_LCD_CHAR_DOWN);
    LCD_GotoXY(1, 3);
    if (type == 1)
    {
        sprintf(buffer,"VP: %01d.%02dV", voltage/100, voltage%100);
    } else {
        sprintf(buffer,"VNA: %01d.%02dV", voltage/100, voltage%100);
    }
    LCD_Print(buffer);
}

void Screen_Function(uint8_t position)
{
//	LCD_Clear();
	LCD_GotoXY(0, position);
	LCD_SendData(_LCD_CHAR_RIGHT_INVERT);
    LCD_GotoXY(1, 0);
    LCD_Print("Output Voltage");
    LCD_GotoXY(1, 1);
    LCD_Print("Output ON/OFF");
    LCD_GotoXY(3, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(4, 3);
    LCD_Print("FUNCTION");
    LCD_GotoXY(12,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Voltage_Control(uint8_t limit)
{
//	LCD_Clear();
	LCD_GotoXY(0, 0);
	LCD_Print("Voltage Control");
    LCD_GotoXY(0, 1);
    LCD_Print("Step: ");
    LCD_GotoXY(0, 2);
    if (limit == 0){
        LCD_Print("Limit: MIN");
    } else if (limit == 1) {
        LCD_Print("Limit: MAX");
    } else {
        LCD_Print("Limit: NONE");
    }
    LCD_GotoXY(3, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(4, 3);
    LCD_Print("FUNCTION");
    LCD_GotoXY(12,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Voltage_Control_Control_Motor(uint16_t step)
{
//	uint16_t adc_value = 330;
    char buffer[20];
//    if (dir == 0){
//        // Motor go to left
//    } else {
//        // Motor go to left
//    }
//    // Read ADC
	LCD_GotoXY(7, 1);
	sprintf(buffer,"%d", step);
//	sprintf(buffer,"%01d.%02dV", adc_value/100, adc_value%100);
	LCD_Print(buffer);
}

void Screen_Control_Relay(uint8_t position)
{
//	LCD_Clear();
	LCD_GotoXY(0, position+1);
	LCD_SendData(_LCD_CHAR_RIGHT_INVERT);
    LCD_GotoXY(1, 0);
    LCD_Print("Output Control");
    LCD_GotoXY(1, 1);
    LCD_Print("Trans. is: ");
    LCD_GotoXY(1, 2);
    LCD_Print("Confirm: ");
    LCD_GotoXY(3, 3);
    LCD_SendData(_LCD_CHAR_LEFT);
    LCD_GotoXY(4, 3);
    LCD_Print("FUNCTION");
    LCD_GotoXY(12,3);
    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Control_Relay_Change_Mode(uint8_t status)
{
//	LCD_GotoXY(9, 1);
//	if (status == 0){
//		LCD_Print("OFF");
//	} else {
//		LCD_Print("ON");
//	}
	LCD_GotoXY(10, 2);
	if (status == 0){
		LCD_Print("OFF");
	} else {
		LCD_Print("ON");
	}
}

void Screen_Accept_Relay(_RTC switchtime, uint8_t status)
{
    char buffer[20];
//    DS3231_GetTime(__MY_RTC);
//	LCD_Clear();
	LCD_GotoXY(1, 0);
	LCD_Print("Output Control");
	LCD_GotoXY(0, 1);
	LCD_Print("SWITCH");
	LCD_GotoXY(7, 1);
	if (status == 0){
		LCD_Print("OFF");
	} else {
		LCD_Print("ON");
	}
	LCD_GotoXY(11, 1);
	LCD_Print("AT:");
    LCD_GotoXY(4, 2);
    sprintf(buffer,"%02d:%02d:%02d", switchtime.Hour, switchtime.Min, switchtime.Sec);
    LCD_Print(buffer);
    LCD_GotoXY(4, 3);
    sprintf(buffer,"%02d/%02d/%d", switchtime.Date, switchtime.Month, switchtime.Year);
    LCD_Print(buffer);
//    LCD_GotoXY(3, 3);
//    LCD_SendData(_LCD_CHAR_LEFT);
//    LCD_GotoXY(4, 3);
//    LCD_Print("FUNCTION");
//    LCD_GotoXY(12,3);
//    LCD_SendData(_LCD_CHAR_RIGHT);
}

void Screen_Test(uint8_t button)
{
	LCD_Clear();
	LCD_GotoXY(0, button);
	LCD_SendData(_LCD_CHAR_RIGHT_INVERT);
    LCD_GotoXY(1, 0);
    LCD_Print("Button Down");
    LCD_GotoXY(1, 1);
    LCD_Print("Button Up");
    LCD_GotoXY(1, 2);
    LCD_Print("Button Enter");
    LCD_GotoXY(1, 3);
    LCD_Print("Button Next");
}
/* End of file ----------------------------------------------------------------*/
