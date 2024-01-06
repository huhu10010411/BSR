/*
 * App_MQTT.h
 *
 *  Created on: Dec 15, 2023
 *      Author: Admin
 */

#ifndef INC_APPLICATION_APP_MQTT_H_
#define INC_APPLICATION_APP_MQTT_H_


#include "SIM.h"
#include "main.h"


void initApp_MQTT(Station_t *station, SIM_t *sim);

void processApp_MQTT(void);

#endif /* INC_APPLICATION_APP_MQTT_H_ */
