/*
 * Temp_sensor.c
 *
 * Created: 06/01/2020 13:33:34
 *  Author: Native_programmer
 */ 
#include "Temp_sensor.h"
#include "ADC.h"
#include "ADC_PostBConfig.h"
#include "Timer.h"
#include "Timer_PostBConfig.h"

void ISR_DAs()
{
	
	
}
void TEMP_SENSOR_INIT(void)
{
	uint8_t u8_ADC_value_buffer=0;
	ADC_INIT(&gstr_Temp_sensorConfig);
	ADC_Interrupt_Enable();
	ADC_Start_Conversion();
	Timer_Init(&TEMP_TIMER_Init_Config);
	Timer_Start(TIMER1,7812U,NullPointer);
}

void TEMP_SENSOR_READ(uint32_t*Copy_pu32_Temp_Buffer)
{
	ADC_READ(Copy_pu32_Temp_Buffer,NullPointer,NullPointer);
    *Copy_pu32_Temp_Buffer=*Copy_pu32_Temp_Buffer*0.488;
}