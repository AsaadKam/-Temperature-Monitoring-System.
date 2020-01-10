/*
 * Seven_Segment.h
 *
 * Created: 04/01/2020 16:20:23
 *  Author: Native_programmer
 */ 


#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

/**Includes**/
#include"Atmega32Registers.h"
#include"Data_Types.h"
#include"BitManipulation.h"

/**Defines**/
#define SEGA                        0
#define SEGB                        1
#define SEGC                        2
#define SEGD                        3
#define SEG_DATA_REG                PORTB
#define SEG_DIRECTION_REG           DDRB
#define SEG_DATA_REG_ENABLE         PORTD 
#define SEG_DIRECTION_REG_ENABLE    DDRD  
#define SEG_1_ENABLE                3
#define SEG_2_ENABLE                4
#endif /* SEVEN_SEGMENT_H_ */