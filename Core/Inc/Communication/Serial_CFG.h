/*
 * Serial_CFG.h
 *
 *  Created on: Oct 2, 2023
 *      Author: Admin
 */

#ifndef INC_SERIAL_CFG_H_
#define INC_SERIAL_CFG_H_

//#include "main.h"
#include <stm32f1xx.h>

#include <string.h>


//#define SCFG_DMA_UART



extern UART_HandleTypeDef *__SCFG_UART;
//extern DMA_HandleTypeDef  *__SCFG_DMA_UART;


/****************************************************/
void initSerial_CFG(UART_HandleTypeDef *huart, DMA_HandleTypeDef  *hdma);

void enableReceiveDMAtoIdle_CFG(void);

void Serial_CFG_Callback(uint16_t Size);

void processing_CMD(uint8_t *myID);

void MarkAsReadData_CFG(void);

#endif /* INC_SERIAL_CFG_H_ */
