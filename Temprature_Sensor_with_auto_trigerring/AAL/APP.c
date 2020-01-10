/*
 * APP.c
 *
 * Created: 06/01/2020 13:30:26
 *  Author: Native_programmer
 */ 
#include "SOS.h"
#include "lcd.h"
#include "ADC.h"
#include "ADC_PostBConfig.h"
#include "DIO.h"
#include "Timer.h"
#include "Timer_PostBConfig.h"
uint32_t gu32_ADC_DATA=0;
uint8_t  gu8_Task_Init_Flag=1;
uint8_t  gu8_Task_LCD_Flag=0;
uint8_t  gu8_Task_ADC_Flag=0;
uint8_t  gu8_ADC_value=12;

void ADC_ISR_Function(void )
{
	
}


 int main()
 {
		uint8_t u8_ADC_value_buffer=0;
		ADC_INIT(&gstr_Temp_sensorConfig);
		ADC_READ(&gu8_ADC_value,ADC_ISR_Function,NullPointer);

		Timer_Init(&TEMP_TIMER_Init_Config);
		Timer_Start(TIMER1,7812U,NullPointer);

		LCD_init();
		LCD_displayString("TEMP=");

		while(1)
		{
			ADC_READ(&gu8_ADC_value,ADC_ISR_Function,NullPointer);
            LCD_integerToString(gu8_ADC_value/2.049,1,5);
		}
 }
 
