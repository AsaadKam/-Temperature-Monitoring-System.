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

void Task_System_Init_3_milisecond(void )
{
	static uint8_t su8_LCD_INIT_FLAG=1;
	static uint8_t su8_LCD_SEND_STRING_FLAG=1;
	static uint8_t su8_LCD_INIT_STATE=0;

	if(gu8_Task_Init_Flag==1)
	{

		switch(su8_LCD_INIT_STATE)
		{
			case 0:
			{
				ADC_INIT(&gstr_Temp_sensorConfig);
				if(su8_LCD_INIT_FLAG==1)
				{
					LCD_init(&su8_LCD_INIT_FLAG);

				}
				else
				{
					su8_LCD_INIT_FLAG=0;
					su8_LCD_INIT_STATE=1;
				}
			}
			break;
			
			case 1:
			{
				if(su8_LCD_SEND_STRING_FLAG==1) LCD_displayString(" TEMP=",&su8_LCD_SEND_STRING_FLAG);
				else
				{
					su8_LCD_SEND_STRING_FLAG=0;
					su8_LCD_INIT_STATE=2;
					gu8_Task_Init_Flag=0;
					gu8_Task_ADC_Flag=1;
				}
			}
			break;
		}
	}
	
}

void Task_ADC_1_Second(void )
{
	static uint8_t su8_Task_ADC_1S_Flag=1;

	if(gu8_Task_ADC_Flag==1)
	{

		if(su8_Task_ADC_1S_Flag==1)
		{
		
			ADC_READ(&gu32_ADC_DATA,NullPointer,&su8_Task_ADC_1S_Flag);

		}
		else
		{

            DIO_Init_Pin(24,1);
            DIO_toggle_Pin(24);
			
			su8_Task_ADC_1S_Flag=1;
			gu8_Task_LCD_Flag=1;
			gu8_Task_ADC_Flag=0;
		}
	}
	else
	{

	}
}

void Task_LCD_3_milisecond(void )
{
	static uint8_t su8_LCD_WRITE_ADC_FLAG=1;
    static uint8_t su8_LCD_WRITE_ADC_STATE=0;
	static su8_ADC_DATA_BUFFER=0;

	if(gu8_Task_LCD_Flag==1)
	{
		{
			switch(su8_LCD_WRITE_ADC_STATE)
			{
				case 0:
			    {

					  if(su8_LCD_WRITE_ADC_FLAG==1)	LCD_integerToString((gu32_ADC_DATA/2.049),1,6,&su8_LCD_WRITE_ADC_FLAG);
					  else
					  {
						  su8_LCD_WRITE_ADC_FLAG=1;
						  su8_LCD_WRITE_ADC_STATE=1;

					  }
				}
				break;
				
				case 1:
				{
				
					 if(su8_LCD_WRITE_ADC_FLAG==1)	LCD_displayString("C.",&su8_LCD_WRITE_ADC_FLAG);
					 else
					 {
						DIO_Init_Pin(25,1);
						DIO_toggle_Pin(25);
						su8_LCD_WRITE_ADC_FLAG=1;
						gu8_Task_ADC_Flag=1;						
						gu8_Task_LCD_Flag=0;
						su8_LCD_WRITE_ADC_STATE=0;
					 }
				}
				break;
			}

         
		}

	}
	else
	{

	}
}

 int main()
 {
 	/**Small Os Init**/
  	SOS_Init();
 	/**Create tasks**/

 	SOS_Create(&Task_System_Init_3_milisecond,3U,SOS_Function_PERIODIC,0);
    SOS_Create(&Task_ADC_1_Second,1000U,SOS_Function_PERIODIC,1);
 	SOS_Create(&Task_LCD_3_milisecond,3U,SOS_Function_PERIODIC,2);

 	/*Start Small OS dispatching**/
  	SOS_Dispatch();
 }
 