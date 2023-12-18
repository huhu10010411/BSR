/*
 * Serial_log.c
 *
 *  Created on: Dec 12, 2023
 *      Author: Admin
 */

#include "Serial_log.h"
#include <stdio.h>
#include <string.h>

#define LOG_TIMEOUT		1000

UART_HandleTypeDef *__SERIAL_LOG_UART;

void init_Serial_log (UART_HandleTypeDef *huart)
{
	__SERIAL_LOG_UART = huart;
}

void Serial_log_string(char *string)
{
	HAL_UART_Transmit(__SERIAL_LOG_UART, (uint8_t*)string, strlen(string), LOG_TIMEOUT);

}

void Serial_log_buffer(uint8_t *buffer, uint16_t buffersize)
{
	HAL_UART_Transmit(__SERIAL_LOG_UART, buffer, buffersize, LOG_TIMEOUT);
}

void Serial_log_number(uint16_t number)
{
	uint8_t tmpbuffer[10];
	uint8_t len = sprintf( (char*)tmpbuffer, "%d", number);
//	HAL_UART_Transmit_DMA(__SERIAL_LOG_UART, tmpbuffer, len);
	HAL_UART_Transmit(__SERIAL_LOG_UART, tmpbuffer, len, LOG_TIMEOUT);
}

void Serial_log_testOperation (void)
{
	Serial_log_string("This is test Serial log string function\r\n");

	Serial_log_number(2001);
}
