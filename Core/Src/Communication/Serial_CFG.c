/*
 * Serial_CFG.c
 *
 *  Created on: Oct 2, 2023
 *      Author: Admin
 */

#include "Serial_CFG.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define Rx_SIZE_CFG     20
#define Main_SIZE_CFG    25

UART_HandleTypeDef *__SCFG_UART;
DMA_HandleTypeDef  *__SCFG_DMA_UART;

uint8_t Rxbuff_CFG[Rx_SIZE_CFG];
uint8_t Mainbuff_CFG[Main_SIZE_CFG];
uint16_t oldPos_CFG=0, newPos_CFG =0;
uint8_t head_CFG = 0, tail_CFG = 0;

uint8_t tx_buff[30];


void initSerial_CFG(UART_HandleTypeDef *huart,DMA_HandleTypeDef  *hdma)
{
	__SCFG_UART = huart;
	__SCFG_DMA_UART = hdma;
	enableReceiveDMAtoIdle_CFG();
}
void enableReceiveDMAtoIdle_CFG(void)
{
	  HAL_UARTEx_ReceiveToIdle_DMA(__SCFG_UART, Rxbuff_CFG, Rx_SIZE_CFG);
	  __HAL_DMA_DISABLE_IT(__SCFG_DMA_UART,DMA_IT_HT);
}
void Serial_CFG_Callback(uint16_t Size)
{

	oldPos_CFG = newPos_CFG;  // Update the last position before copying new data

	/* If the data in large and it is about to exceed the buffer size, we have to route it to the start of the buffer
	 * This is to maintain the circular buffer
	 * The old data in the main buffer will be overlapped
	 */
	if (oldPos_CFG+Size > Main_SIZE_CFG)  // If the current position + new data size is greater than the main buffer
	{
		uint8_t datatocopy = Main_SIZE_CFG-oldPos_CFG;  // find out how much space is left in the main buffer
		memcpy ((uint8_t *)Mainbuff_CFG+oldPos_CFG, (uint8_t *)Rxbuff_CFG, datatocopy);  // copy data in that remaining space

		oldPos_CFG = 0;  // point to the start of the buffer
		memcpy ((uint8_t *)Mainbuff_CFG, (uint8_t *)Rxbuff_CFG+datatocopy, (Size-datatocopy));  // copy the remaining data
		newPos_CFG = (Size-datatocopy);  // update the position
	}

	/* if the current position + new data size is less than the main buffer
	 * we will simply copy the data into the buffer and update the position
	 */
	else
	{
		memcpy ((uint8_t *)Mainbuff_CFG+oldPos_CFG, (uint8_t *)Rxbuff_CFG, Size);
		newPos_CFG = Size+oldPos_CFG;
	}

	/* Update the position of the Head
	 * If the current position + new size is less then the buffer size, Head will update normally
	 * Or else the head will be at the new position from the beginning
	 */
	if (head_CFG+Size < Main_SIZE_CFG) head_CFG = head_CFG+Size;
	else head_CFG = head_CFG+Size - Main_SIZE_CFG;


	enableReceiveDMAtoIdle_CFG();
	/*
	 * Processing data
	 */

}
uint8_t nID_validation (uint8_t *str, uint8_t ID,uint8_t NumofID)
{
	if (ID >=0x51 && ID <=0xFE)
	{
		uint8_t ID_buff[NumofID];
		sprintf((char*)ID_buff,"%d",ID);
		if (!memcmp(str,ID_buff,NumofID))
		{
			return 1;
		}
	}
	return 0;
}

uint8_t sID_validation(uint8_t *str, uint8_t ID,uint8_t NumofID)
{
	if (ID >=0x01 && ID <=0x50)
	{
		uint8_t ID_buff[NumofID];
		sprintf((char*)ID_buff,"%d",ID);
		if (!memcmp(str,ID_buff,NumofID))
		{
			return 1;
		}
	}

	return 0;
}
void processing_CMD(uint8_t *myID)
{

	uint8_t len;
	uint8_t size;
	uint8_t tmpID;
	uint8_t *Processingbuff;
	if(head_CFG > tail_CFG)
	{
		size = head_CFG -tail_CFG+1;
		Processingbuff = (uint8_t*)malloc(size*sizeof(uint8_t));
		memcpy((uint8_t*)Processingbuff,(uint8_t*)Mainbuff_CFG+ tail_CFG, size);
	}
	else if (head_CFG < tail_CFG)
	{
		size = Main_SIZE_CFG - tail_CFG+ head_CFG;
		Processingbuff = (uint8_t*)malloc(size*sizeof(uint8_t));
		memcpy((uint8_t*)Processingbuff,(uint8_t*)Mainbuff_CFG + tail_CFG, Main_SIZE_CFG - tail_CFG);
		memcpy((uint8_t*)Processingbuff+Main_SIZE_CFG - tail_CFG,(uint8_t*)Mainbuff_CFG, head_CFG);
	}
	else{
		return;
	}

	uint8_t index =0;
	while (index < size)
	{

			if (Processingbuff[index]=='M' && Processingbuff[index+1]=='C'
					&& Processingbuff[index+2]=='F'&& Processingbuff[index+3]=='G'
							&& Processingbuff[index+4]=='+' )
			{
				index+= 5;
				uint8_t tmpbuff[10] ;
				memcpy((uint8_t*)tmpbuff,(uint8_t*)&Processingbuff[index],10);
				if (strstr((char*)tmpbuff,"SETID:"))
				{
					uint8_t NumofID =0;
					if (tmpbuff[7] ==';')
					{
						tmpID = (tmpbuff[6]-48);
						index+=7;
						NumofID =1;
					}
					else if(tmpbuff[8] ==';')
					{
						tmpID = (tmpbuff[6]-48)*10 + tmpbuff[7]-48;
						index+=8;
						NumofID =2;
					}
					else if (tmpbuff[9]==';')
					{
						tmpID= (tmpbuff[6]-48)*100 + (tmpbuff[7]-48)*10 + tmpbuff[8]-48;
						index+=9;
						NumofID =3;
					}
					if (sID_validation(tmpbuff+6, tmpID, NumofID))
					{
						*myID =tmpID;
						len =sprintf((char*)tx_buff,"MCFG+SETID:%d OK",*myID);
						HAL_UART_Transmit_DMA(__SCFG_UART, tx_buff, len);
					}
					else
					{
						HAL_UART_Transmit_DMA(__SCFG_UART, (uint8_t*)"MCFG+SETID ERROR", 16);
					}

				}
				else if(strstr((char*)tmpbuff,"GETID?"))
				{
					len = sprintf((char*)tx_buff,"MCFG+GETID:%d OK",*myID);
					HAL_UART_Transmit_DMA(__SCFG_UART,(uint8_t*) tx_buff, len);
					index+= 6;
				}

			}
			else index++;
	}

	free(Processingbuff);
}

void MarkAsReadData_CFG(void)
{
	tail_CFG = head_CFG;
}
