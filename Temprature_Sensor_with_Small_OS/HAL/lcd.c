
/*
 * lcd.c
 *
 *  Created on: Apr 29, 2019
 *      Author: LENOVO
 */


#include "lcd.h"



static uint8_t LCD_pinEPulse();
/*
 * LCD_pinEPulse.
 */
static uint8_t LCD_pinEPulse()
{
	static uint8_t su8_LCD_pinEPulse_state=0;
	
	if(su8_LCD_pinEPulse_state==2) su8_LCD_pinEPulse_state=0;
	else
	{}
	switch(su8_LCD_pinEPulse_state)
	{
		//High Pulse.
		case 0 :
		TOGGLE_BIT(LCD_CONFIG_PORT,E);
		su8_LCD_pinEPulse_state=1;
		break;
		//Low Pulse.
		case 1:
		TOGGLE_BIT(LCD_CONFIG_PORT,E);
		su8_LCD_pinEPulse_state=2;
		break;
	}
	
	return su8_LCD_pinEPulse_state ;

}
/*
 * Init LCD.
 */
void LCD_init(uint8_t* pu8_flag)
{
	
	static uint8_t u8_LCD_command_state=1;
    static uint8_t u8_LCD_init_state=0;

	/*
	 * Init CMD.
	 */
    switch(u8_LCD_init_state)
	{
		case 0:
		{
			LCD_DDR |= 0xF0;
						
			SET_BIT(LCD_CONFIG_DDR, RS);//output.
			SET_BIT(LCD_CONFIG_DDR, RW);//output.
			SET_BIT(LCD_CONFIG_DDR, E);//output.
			if(u8_LCD_command_state) LCD_sendCommand(0x02,&u8_LCD_command_state);
			else
			{
				u8_LCD_command_state=1;
				u8_LCD_init_state=1;
			}
		}
		break;
		case 1:
		{
			if(u8_LCD_command_state) LCD_sendCommand(0x28,&u8_LCD_command_state);	
            else
			{
				u8_LCD_command_state=1;				
				u8_LCD_init_state=2;
			}
		}
		break;
		case 2:
		{
			if(u8_LCD_command_state) LCD_sendCommand(0x0C,&u8_LCD_command_state);
			else
			{
				u8_LCD_command_state=1;				
				u8_LCD_init_state=3;

			}
		}
		break;
		case 3:
		{
			if(u8_LCD_command_state) LCD_sendCommand(0x01,&u8_LCD_command_state);
			else
			{
				u8_LCD_command_state=1;
				u8_LCD_init_state=0;
				*pu8_flag=0;
			}
		}
		break;		
	}
	
/*
	
	LCD_sendCommand(0x01);*/
	
}
/*
 * send command to LCD.
 */
void LCD_sendCommand(const uint8_t cmd,uint8_t*Copy_Pntr_u8_LCD_sendCommand_Flag)
{
	static uint8_t u8_Command_State=0;

	/*
	 * After each CMD I must wait about (100 Ms).
	 */
	//Changed in 4-bit.
	switch(u8_Command_State)
	{
			case 0:
				CLR_BIT(LCD_CONFIG_PORT, RS);
				CLR_BIT(LCD_CONFIG_PORT, RW);
				SET_BIT(LCD_CONFIG_PORT, E);
				u8_Command_State=1;
				//( LCD_PORT & 0x0F ) ﬂ‰  ‰«”ÌÂ« Œ«·’ „Õÿ Â«‘ ›Ì «·Õ”»«‰
			break;
			case 1:				
				LCD_PORT = ( LCD_PORT & 0x0F )|( ( cmd & 0xF0 ));//Passed.
				CLR_BIT(LCD_CONFIG_PORT, E);
				u8_Command_State=2;
			break;
			case 2:
			    SET_BIT(LCD_CONFIG_PORT, E);
				u8_Command_State=3;
			break;
			case 3:
				LCD_PORT = ( LCD_PORT & 0x0F ) | (( cmd << 4 ));
				CLR_BIT(LCD_CONFIG_PORT, E);
				u8_Command_State=0;
				*Copy_Pntr_u8_LCD_sendCommand_Flag=0;
			break;

		/*
		 * High to low pulse to on PIN "E".
		 */		
	}

}
/*
 * sendingData.
 */
void LCD_displayCharacter(const uint8_t data,uint8_t*Copy_Pntr_u8_LCD_displayCharacter_Flag)
{
	static uint8_t u8_displayCharacter=0;
	
	switch(u8_displayCharacter)
	{               

			case 0:	
				SET_BIT(LCD_CONFIG_PORT, RS);
				CLR_BIT(LCD_CONFIG_PORT, RW);				
				SET_BIT(LCD_CONFIG_PORT, E);
				u8_displayCharacter=1;
            break;
			case 1:		
				LCD_PORT = ( LCD_PORT & 0x0F ) | ( ( data & 0xF0 ));//Passed.
				CLR_BIT(LCD_CONFIG_PORT, E);
				u8_displayCharacter=2;
			break;
			case 2:	
				SET_BIT(LCD_CONFIG_PORT, E);
				u8_displayCharacter=3;
			break;
			case 3:
				LCD_PORT = ( LCD_PORT & 0x0F ) | (( data << 4 ));
				CLR_BIT(LCD_CONFIG_PORT, E);
				*Copy_Pntr_u8_LCD_displayCharacter_Flag=0;
				u8_displayCharacter=0;
			break;
	
	}			
				
}


void LCD_displayString(uint8_t* Copy_Pntr_u8_LCD_displayString,uint8_t*Copy_Pntr_u8_LCD_displayString_Flag)
{
	static uint8_t u8_index_string = 0;
	static uint8_t u8_LCD_displayString_FLag=1;
	
	if(Copy_Pntr_u8_LCD_displayString[u8_index_string]!='\0')
	{
        
		if(u8_LCD_displayString_FLag==1) LCD_displayCharacter(Copy_Pntr_u8_LCD_displayString[u8_index_string],&u8_LCD_displayString_FLag);
		else                             
		{
			u8_LCD_displayString_FLag=1;
             u8_index_string++;
		}
	}
	else if(Copy_Pntr_u8_LCD_displayString[u8_index_string]=='\0')
	{
		u8_index_string=0;
		*Copy_Pntr_u8_LCD_displayString_Flag=0;
	}

	else
	{
		/**Do Nothing**/
	}
	
	
}


void LCD_goToRowColumn(uint8_t row, uint8_t col,uint8_t* Copy_Pntr_u8_LCD_goToRowColumn_Flag)
{
	uint8_t address;
	static uint8_t u8_LCD_goToRowColumn_Flag=1;
	switch(row)
	{
		case 1:
			address = 0x80 + col;
			//OR LCD_sendCommand(0x80).
			break;
		case 2:
			address = 0xC0 + col;
			//OR LCD_sendCommand(0xC0);
			break;
		default:
			break;
			//Nothing.
	}
	/*Another solution.
	 * uint8 i;
	for(i = 0;i<col;i++)
	{
		LCD_sendCommand(0x14);//right shift cursor to the wanted col.
	}*/

	/*
	 * Another solution
	 * cmd = address | 0b100000000
	 */
	if(u8_LCD_goToRowColumn_Flag==1) LCD_sendCommand(address | 0b100000000,& u8_LCD_goToRowColumn_Flag);
	else
	{
		u8_LCD_goToRowColumn_Flag=1;
		*Copy_Pntr_u8_LCD_goToRowColumn_Flag=0;
	}

}

/*
 * Put Copy_Pntr_u8_LCD_displayString in ROW-COL.
 */

void LCD_stringRowCol(uint8_t* Copy_Pntr_u8_LCD_displayString, uint8_t row, uint8_t col,uint8_t* Copy_Pntr_u8_LCD_stringRowCol_Flag)
{
	static uint8_t u8_LCD_displayString_FLAG=1;
	static uint8_t u8_LCD_goToRowColumn_FLAG=1;
	static uint8_t u8_LCD_stringRowCol_State=0;
	switch (u8_LCD_stringRowCol_State)
	{
		case 0:
		 	if(u8_LCD_goToRowColumn_FLAG) LCD_goToRowColumn(row, col,&u8_LCD_goToRowColumn_FLAG) ;
			else
			{
				u8_LCD_goToRowColumn_FLAG=1;
				u8_LCD_stringRowCol_State=1;
			}
		break;
		case 1:
		    if(u8_LCD_displayString_FLAG) LCD_displayString(Copy_Pntr_u8_LCD_displayString,&u8_LCD_displayString_FLAG);
		    else
		    {
			    u8_LCD_displayString_FLAG=1;
				u8_LCD_stringRowCol_State=0;
			    *Copy_Pntr_u8_LCD_stringRowCol_Flag=0;
		    }
		break;
		case 2:
           /*Do Nothing*/
		break;
	}


}


/*
 * Clear Screen.
 */

void LCD_clearScreen(uint8_t*Copy_Pntr_u8_LCD_clearScreen_Flag)
{
	static uint8_t u8_LCD_clearScreen_state=1;
	if(u8_LCD_clearScreen_state) LCD_sendCommand(0x01,&u8_LCD_clearScreen_state);
	else
	{
		u8_LCD_clearScreen_state=1;
		*Copy_Pntr_u8_LCD_clearScreen_Flag=0;
	}

}

/*
 * integerToString.
 */

void LCD_integerToString(uint32_t value,uint8_t row,uint8_t col,uint8_t* Copy_Pntr_u8_LCD_integerToString_Flag)
{
	static uint8_t u8_LCD_integerToString_FLAG=1;
	uint32_t buffer[20];
	itoa(value, buffer , 10);
	if(u8_LCD_integerToString_FLAG==1) LCD_stringRowCol(buffer,row,col,&u8_LCD_integerToString_FLAG);
	else
	{
		u8_LCD_integerToString_FLAG=1;
		*Copy_Pntr_u8_LCD_integerToString_Flag=0;
	}
}