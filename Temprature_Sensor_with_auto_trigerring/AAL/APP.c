/*
 * APP.c
 *
 * Created: 08/01/2020 13:30:26
 *  Author: ASAAD
 */ 

#include "lcd.h"
#include "Temp_sensor.h"

 int main()
 {
	    /*Initialize Variables for temp. sensor*/
		uint32_t  u32_TEMP_VALUE=0;
		uint32_t  u32_TEMP_VALUE_Buffer=0;/*This for making decision to print the current read value or not*/
		/**Inialize Temprature sensor**/
		TEMP_SENSOR_INIT();
		/**Inialize LCD**/
		LCD_init();
		/**Display string "Temp=" LCD**/		
		LCD_displayString("TEMP=");

		while(1)
		{
			/**Read the value and put it in u32_TEMP_VALUE register**/
		    TEMP_SENSOR_READ(&u32_TEMP_VALUE);
			/**Make decison if i am going to display the string of integer on screen or not**/
		    if(u32_TEMP_VALUE!=u32_TEMP_VALUE_Buffer)
			{
			   /**Display the string of integer on the screen in postion row->>1 and column->>5 **/
	           LCD_integerToString(u32_TEMP_VALUE,1,5);
			   /**Put the value of the approved sample in buffer of the sample**/
			   u32_TEMP_VALUE_Buffer=u32_TEMP_VALUE;
			}
			else
			{
				/**Do Nothing**/
			}
		}
 }
 
