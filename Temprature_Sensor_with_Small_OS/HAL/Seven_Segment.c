/*
 * Seven_Segment.c
 *
 * Created: 04/01/2020 16:04:18
 *  Author: Native_programmer
 */ 

#include "Seven_Segment.h"

uint8_t Seven_Segment_Init(void )
{
	SET_LOWER_NIBBLE(SEG_DIRECTION_REG);
	SET_BIT(SEG_DIRECTION_REG_ENABLE,SEG_1_ENABLE);
    SET_BIT(SEG_DATA_REG_ENABLE,SEG_1_ENABLE);
	
	return 0;
}	
uint8_t Seven_Segment_Write(char_t Data)
{
    SEG_DATA_REG=Data;	
	return 0;
}