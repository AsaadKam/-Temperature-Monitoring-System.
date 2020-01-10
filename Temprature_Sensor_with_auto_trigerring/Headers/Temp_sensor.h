/*
 * Temp_sensor.h
 *
 * Created: 06/01/2020 13:46:58
 *  Author: Native_programmer
 */ 


#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include "ADC.h"

void TEMP_SENSOR_INIT(void);

void TEMP_SENSOR_READ(uint32_t*Copy_pu32_Temp_Buffer);


#endif /* TEMP_SENSOR_H_ */