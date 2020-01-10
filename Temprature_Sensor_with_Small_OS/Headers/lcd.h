/*
 * lcd.h
 *
 *  Created on: Apr 29, 2019
 *      Author: LENOVO
 */

#ifndef LCD_H_
#define LCD_H_


#include "Atmega32Registers.h"
#include "BitManipulation.h"
#include "Data_Types.h"
#include "Timer.h"
#include "Timer_PostBConfig.h"


#define LCD_MODE 4
#define  True 1
#define  False 0
/*
 * 4 bit mode.
 */
#if LCD_MODE == 4
	/*
	 * Set upper pins to True  if you want to use for upper pins of specific port (0,3).
	 * Set upper pins to False if you want to use the lower pins in specific port (4,7).
	 */
    #define UPPER_PINS False
#endif
/*
 * LCD connected ports.
 */
#define LCD_DDR  DDRA
#define LCD_PORT PORTA
#define LCD_PIN  PINA

/*
 * Configuration pins.
 */
#define LCD_CONFIG_DDR  DDRA
#define LCD_CONFIG_PORT PORTA
#define RS 1
#define RW 2
#define E  3



/*
 * Init LCD.
 */
void LCD_init(uint8_t* pu8_flag);


/*
 * send cmd to LCD.
 */
void LCD_sendCommand(const uint8_t cmd,uint8_t*Copy_Pntr_u8_LCD_sendCommand_Flag);

/*
 * sendingData.
 */
void LCD_displayCharacter(const uint8_t data,uint8_t*Copy_Pntr_u8_LCD_displayCharacter_Flag);

void LCD_displayString(uint8_t* Copy_Pntr_u8_LCD_displayString,uint8_t*Copy_Pntr_u8_LCD_displayString_Flag);

void LCD_integerToString(uint32_t value,uint8_t row,uint8_t col,uint8_t* Copy_Pntr_u8_LCD_integerToString_Flag);

void LCD_goToRowColumn(uint8_t row, uint8_t col,uint8_t* Copy_Pntr_u8_LCD_goToRowColumn_Flag);
void LCD_stringRowCol(uint8_t* Copy_Pntr_u8_LCD_displayString, uint8_t row, uint8_t col,uint8_t* Copy_Pntr_u8_LCD_stringRowCol_Flag);
void LCD_clearScreen(uint8_t*Copy_Pntr_u8_LCD_clearScreen_Flag);
#endif /* LCD_H_ */
