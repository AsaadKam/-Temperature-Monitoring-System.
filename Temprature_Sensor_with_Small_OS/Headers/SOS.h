#ifndef SOS_H_
#define SOS_H_



/*- INCLUDES -----------------------------------------------*/
#include"Timer.h"
#include"DATA_types.h"
/*-  MACROS -------------------------------------------------*/
/******************************************************************/
/*              SOS_TIMER_Structure_Config                        */
/******************************************************************/
#define     StrSOS_Configuration_t         StrTimerConfiguration_t
#define     SOS_TIMER_IDS                  Timer_Channel
#define     SOS_TIMER_ID0                  TIMER0
#define     SOS_TIMER_ID1                  TIMER1
#define     SOS_TIMER_ID2                  TIMER2
#define     SOS_TIMER_PSC                  Timer_Psc  
#define     SOS_TIMER_MODE                 Timer_Mode 
#define     SOS_TIMER_MODE_MICRO           TIMER_MODE_MICRO  
#define     SOS_TIMER_MODE_MILIE           TIMER_MODE_MILIE    
#define     SOS_TIMER_COUNTER_MODE         COUNTER_MODE  
#define     SOS_TIMER_INT_Mode             Interrupt_Mode
#define     SOS_TIMER_INT                  INTERRUPT
#define     SOS_No_Need_For_TickCounts      0
#define     SOS_Function_PERIODIC           0
#define     SOS_Function_ONESHOT            1
/*________________________________________________________________*/
/******************************************************************/
/*              SOS_TIMER_Prescaler                               */
/******************************************************************/

/*_______SOS_TIMER0__________________________________*/
#define NO_CLOCK_SOS_TIMER_ID0     					0U
#define F_CPU_CLOCK_SOS_TIMER_ID0   					1U
#define F_CPU_CLOCK_8_SOS_TIMER_ID0 					2U
#define F_CPU_CLOCK_64_SOS_TIMER_ID0					3U
#define F_CPU_CLOCK_256_SOS_TIMER_ID0					4U
#define F_CPU_CLOCK_1024_SOS_TIMER_ID0				    5U
#define F_EXTERNAL_CLOCK_FALLING_SOS_TIMER_ID0		    6U
#define F_EXTERNAL_CLOCK_RISING_SOS_TIMER_ID0		    7U

/*_______SOS_TIMER1__________________________________*/
#define NO_CLOCK_SOS_TIMER_ID1     					    0U
#define F_CPU_CLOCK_SOS_TIMER_ID1   					1U
#define F_CPU_CLOCK_8_SOS_TIMER_ID1 					2U
#define F_CPU_CLOCK_64_SOS_TIMER_ID1					3U
#define F_CPU_CLOCK_256_SOS_TIMER_ID1					4U
#define F_CPU_CLOCK_1024_SOS_TIMER_ID1				    5U
#define F_EXTERNAL_CLOCK_FALLING_SOS_TIMER_ID1		    6U
#define F_EXTERNAL_CLOCK_RISING_SOS_TIMER_ID1			7U

/*_______SOS_TIMER2__________________________________*/
#define NO_CLOCK_SOS_TIMER_ID2     					    0U
#define F_CPU_CLOCK_SOS_TIMER_ID2   					1U
#define F_CPU_CLOCK_8_SOS_TIMER_ID2 					2U
#define F_CPU_CLOCK_32_SOS_TIMER_ID2					3U
#define F_CPU_CLOCK_64_SOS_TIMER_ID2					4U
#define F_CPU_CLOCK_128_SOS_TIMER_ID2					5U
#define F_CPU_CLOCK_256_SOS_TIMER_ID2		 			6U
#define F_CPU_CLOCK_1024_SOS_TIMER_ID2				    7U

#define SOS_NO_NEED_PSC                                 0U


/*__________________________________________________________*/
#define SOS_Counts_One_mili_TIMER_ID_ZERO 125
/*__________________________________________________________*/
/******************************************************************/
/**                       ERROR                                  **/
/******************************************************************/
#define SOS_Error_OK                                            0
#define SOS_Error_EXTRA_EVENTS                                  1
#define SOS_Error_Start_Null_func                               2
#define SOS_Error_Nothing_To_Stop                               3
#define SOS_Error_Function_type_undefined                       4
#define SOS_Error_Nothing_To_Dispatch                           5
#define SOS_Error_Function_Periodicity_Less_Than_OR_EQUAL_zero  6

/*__________________________________________________________*/
/*- STRUCTS AND UNIONS -------------------------------------*/
/*- CONSTANTS ----------------------------------------------*/
/*- PRIMITIVE TYPES ----------------------------------------*/
typedef uint8_t SOS_Error_t ;
/*- ENUMS --------------------------------------------------*/
/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/
/*_______________________________________________________________________________________________________________________________*/
/*Description: SOS Initialization
 * Input     : StrSOS_Configuration_t* pSOS_Init_Config->Configuration structure
               that configure the SOS and the timer of it also
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
extern SOS_Error_t SOS_Init();
/*_______________________________________________________________________________________________________________________________*/
/*Description: SOS Start
 * Input     : void(*PntrToFunc_Copy_SOS_Start)(void)->Pointer to function,Periodicity_MS->Periodicity milisecond
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
extern SOS_Error_t SOS_Create_Task(PntrToFunc_t PntrToFunc_Copy_SOS_Start,uint16_t u16_Copy_Periodicity_MS_SOS_Start,uint16_t u16_Copy_Periodic_or_not_SOS_Start,uint16_t u16_Copy_SOS_TASK_Priority);
/*_______________________________________________________________________________________________________________________________*/
/*Description: SOS Initialization
 * Input     : Without input
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
extern void SOS_Dispatch(void);
/*_______________________________________________________________________________________________________________________________*/
/*Description: SOS Stop
 * Input     : void(*PntrToFunc_Copy_SOS_Start)(void)->Pointer to function
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
extern SOS_Error_t SOS_Stop(PntrToFunc_t PntrToFunc_Copy_SOS_Start);


#endif /* SOS_H_ */