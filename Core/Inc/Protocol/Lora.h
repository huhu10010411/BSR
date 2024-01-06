/*
 * Lora.h
 *
 *  Created on: Dec 24, 2023
 *      Author: Admin
 */

#ifndef INC_PROTOCOL_LORA_H_
#define INC_PROTOCOL_LORA_H_

//#include "stm32f1xx.h"
#include "main.h"

#define START_POS		0
#define ID_LENGTH_POS	1
#define ID_STARTPOS		3


#define START_VALUE		1

extern UART_HandleTypeDef *__LORA_UART;
void initLora(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma);

void initmyLora(Station_t *myStation);

void enableReceiveDMAtoIdle_LORA(void);

void Lora_callback(uint16_t Size);

uint8_t Lora_Setmode(sensor_mode_t mode);

void MarkAsReadData_LORA(void);

void testLora_receive (void);

#endif /* INC_PROTOCOL_LORA_H_ */
