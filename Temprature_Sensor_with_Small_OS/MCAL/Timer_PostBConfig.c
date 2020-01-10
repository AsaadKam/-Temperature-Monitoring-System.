/*
 * TimerConfig.c
 *
 * Created: 23/11/2019 04:07:28
 *  Author: Native_programmer
 */ 


#include "Timer_PostBConfig.h"

/*
 *
 * Let's suppose i have two case the 1st one i want to delay for 4ms and we will initiate for 1st case structure
 *              from type called TimerConfig called u_DelayConfig and let us define it 
 *
 *
 */
  StrTimerConfiguration_t TEMP_TIMER_Init_Config={TIMER1
 
                                                  ,F_CPU_CLOCK_1024_TIMER_1
																  
											      ,COUNTER_MODE
																  
												  ,INTERRUPT                              };
												  
  StrTimerConfiguration_t LCD_TIMER_Init_Config={ TIMER0
	  
                                                  ,TIMER_NOTHING_PASSED_FOR_PRESCALER
                                                  
                                                  ,TIMER_MODE_MILIE
                                                  
                                                  ,POLLING                              };