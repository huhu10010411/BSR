/*
 * App_MCU.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Admin
 */

#ifndef INC_APPLICATION_APP_MCU_H_
#define INC_APPLICATION_APP_MCU_H_

#include "stm32f1xx.h"
#include "main.h"

#define UPDATE_DATA_PERIOD		10              // second


void initApp_MCU(Station_t *station, SIM_t *mySIM);

void processApp_MCU(void);

#endif /* INC_APPLICATION_APP_MCU_H_ */
