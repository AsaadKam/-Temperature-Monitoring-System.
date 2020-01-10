/**********************************************
 * This driver implements timer managment unit 
 * that supplys delay from 1m to above 
 *
 **********************************************/
/*- INCLUDES ----------------------------------------------*/
#include"Timer.h"
#include"DIO.h"
#include"SOS.h"
#include"SOS_Config.h"
#include"Atmega32Registers.h"


/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL Dataypes ----------------------------------------*/

/********************************************
 * This stucture contain the three variables 
 * pointer to function and the peridicity in
 * milisecond
 ********************************************/
 
typedef struct 
{
	 uint8_t      Periodic_or_not;
	 uint8_t      Priority;
	 uint64_t     Periodicity_MS;
	 uint64_t     Period_Count;
	 PntrToFunc_t PntrToFunc;
	 
}SOS_Node_t;

/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/

static void SOS_Function_ISR(void);

/*- GLOBAL STATIC VARIABLES -------------------------------*/

static  SOS_Node_t sga_SOS_Events[SOS_Events]={0};/**Array of OS tasks**/
static  uint64_t sgu16_SOS_index=0;/****Index for above array****/
static  volatile uint64_t sgu64_SOS_Timer_ISR_Count=0;/***Counter of SOS milisecond events **/

/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/

/***Function that will be passed to Timer ISR from SOS***/

void  SOS_Function_ISR(void)
{
    sgu64_SOS_Timer_ISR_Count++;
}

/*- APIs IMPLEMENTATION -----------------------------------*/
/**SOS initalization happens by this API below**/
/** set configuration of timer OS      **/
SOS_Error_t SOS_Init()
{
	StrSOS_Configuration_t strSOS_configuration;
	
	strSOS_configuration.SOS_TIMER_IDS=SOS_TIMER_ID0;
	/*******************************************
	 *  Clock prescaler is adjusted internally *
	 *           no need to enter it           *  
	 *******************************************/
	strSOS_configuration.SOS_TIMER_PSC=SOS_NO_NEED_PSC;
	strSOS_configuration.SOS_TIMER_MODE=SOS_TIMER_MODE_MILIE;
	strSOS_configuration.SOS_TIMER_INT_Mode=SOS_TIMER_INT;
	
	Timer_Init(&strSOS_configuration);
	
	return SOS_Error_OK;
}

/***Creat SOS tasks by By initalize array of task of SOS ****/ 
SOS_Error_t SOS_Create(PntrToFunc_t PntrToFunc_Copy_SOS_Start,uint16_t u16_Copy_Periodicity_MS_SOS_Start,uint16_t u16_Copy_Periodic_or_not_SOS_Start,uint16_t u16_Copy_SOS_TASK_Priority)
{
    SOS_Error_t SOS_Start_Error= SOS_Error_OK;
	
	if(sgu16_SOS_index>SOS_Events) SOS_Start_Error= SOS_Error_EXTRA_EVENTS;
	else
	{  
        /**Check whether function that passed is nullpointer or not**/
		if(NullPointer!=PntrToFunc_Copy_SOS_Start)
		{	
			sga_SOS_Events[sgu16_SOS_index].PntrToFunc=PntrToFunc_Copy_SOS_Start;	
			/**Check whether the function categorty of function is SOS_Function_ONESHOT or SOS_Function_PERIODIC**/
			if(u16_Copy_Periodic_or_not_SOS_Start==SOS_Function_ONESHOT || u16_Copy_Periodic_or_not_SOS_Start==SOS_Function_PERIODIC)
			{		
		
				sga_SOS_Events[sgu16_SOS_index].Periodic_or_not=u16_Copy_Periodic_or_not_SOS_Start;
				sga_SOS_Events[sgu16_SOS_index].Priority=u16_Copy_SOS_TASK_Priority;
				/**Check whether the function periodicty more than 0 or not **/
				if(u16_Copy_Periodicity_MS_SOS_Start>0U)
				{
                    
					sga_SOS_Events[sgu16_SOS_index].Periodicity_MS=u16_Copy_Periodicity_MS_SOS_Start;
					sgu16_SOS_index++;	
				}
				/**If not adjust error according to criteria **/
				else
				{
					SOS_Start_Error=SOS_Error_Function_Periodicity_Less_Than_OR_EQUAL_zero;
				}
			
			}  
			/**If not adjust error according to criteria **/
			else 
			{			
				SOS_Start_Error=SOS_Error_Function_type_undefined;
			}
		
		}
		/**If not adjust error according to criteria **/		
		else
		{
			SOS_Start_Error=SOS_Error_Start_Null_func;
		}
	}
	return SOS_Start_Error;
}
/***SOS start manage the system for consumers***/
void SOS_Dispatch(void)
{
    uint8_t u8_1st_Loop_Dispatch_Flag=1;
	uint64_t u64_Dispatch_Func_Count=0;
    uint64_t u64_SOS_COUNT=0U;
	while(1)
	{ 
		/*
		 *  Check if the disptach is the the first loop to 
		 *  it to start if so start the timer
		 */
		if((u8_1st_Loop_Dispatch_Flag==1)&&(sgu16_SOS_index!=0))   
		{
			/**Use bubble sorting for arrangeing prioirty of dispatcher**/
		 	for(int8_t s8_iteration=0; s8_iteration<sgu16_SOS_index;s8_iteration++)
			{ 
				for(int8_t s8_i=sgu16_SOS_index-1;s8_i>s8_iteration;s8_i--)
				{
					SOS_Node_t SOS_Node_t_Buffer;
					if(sga_SOS_Events[s8_i].Priority<sga_SOS_Events[s8_i-1].Priority)  
					{
						SOS_Node_t_Buffer=sga_SOS_Events[s8_i];
						sga_SOS_Events[s8_i]=sga_SOS_Events[s8_i-1];
						sga_SOS_Events[s8_i-1]=SOS_Node_t_Buffer;
					}
				  
				}
			} 
			/*Timer start working in milies*/
			Timer_Start(TIMER0,0,SOS_Function_ISR);
			u8_1st_Loop_Dispatch_Flag=0;
		}
		/**If it is not first loop for dispatcher of SOS**/
		else if((u8_1st_Loop_Dispatch_Flag==0)&&(sgu16_SOS_index!=0))
		{
			u64_SOS_COUNT=sgu64_SOS_Timer_ISR_Count;
			/*Looping to execute the function which it's time comes(Brain of SOS)*/	
			for(uint64_t u64_i=0;u64_i<sgu16_SOS_index;u64_i++)
			{ 
				u64_Dispatch_Func_Count=((sga_SOS_Events[u64_i].Period_Count)+1)*sga_SOS_Events[u64_i].Periodicity_MS;
				
				/*** I will increment each function count until i reach to it's periodicity,***
    		     *** then i will execute it's routine                                       ***/
				if((u64_Dispatch_Func_Count==u64_SOS_COUNT))
				{      
					/**Execute the customer's function**/
					sga_SOS_Events[u64_i].PntrToFunc();/*Executing...................*/
					/**Increment the number of occurance of customer function**/
					sga_SOS_Events[u64_i].Period_Count=sga_SOS_Events[u64_i].Period_Count+1;
                    /***If the function is one shot,let it work then stop it directly**/
  					if(sga_SOS_Events[u64_i].Periodic_or_not==SOS_Function_ONESHOT)
					{
                        /**Stop function (Remove it from array)**/	
						SOS_Stop(sga_SOS_Events[u64_i].PntrToFunc);
					}
					else
					{
						/**Do Nothing**/
					} 

				}
			}
		}
		else
		{
		}

    }
}

/***Stop Certain task from being servied by Dispatcher****/ 
SOS_Error_t SOS_Stop(PntrToFunc_t PntrToFunc_Copy_SOS_Start)
{

    SOS_Error_t SOS_Dispatch_Error= SOS_Error_OK;
    /***Check whether is empty or not**/
	if(sgu16_SOS_index==0) SOS_Dispatch_Error=SOS_Error_Nothing_To_Stop;
	/***Else stop the task from being mangaed by dispatcher by removing it from array */
	else
	{
        /**Loop TO find the function that needed to be stopped**/
 		for(uint8_t u8_SOS_Stop_Index=0;u8_SOS_Stop_Index<sgu16_SOS_index;u8_SOS_Stop_Index++)
		{
			/**If we find the function that needed to be removed**/
			if(sga_SOS_Events[u8_SOS_Stop_Index].PntrToFunc==PntrToFunc_Copy_SOS_Start) 
			{
	             /**If the index of function not in the end of array ,
				    we should arrange the array**/
				 if(u8_SOS_Stop_Index != sgu16_SOS_index-1 )
				 { 
					/*Decrement the sgu16_SOS_index*/
         			sgu16_SOS_index--;
 					while(u8_SOS_Stop_Index<sgu16_SOS_index )
 					{
					sga_SOS_Events[u8_SOS_Stop_Index]=sga_SOS_Events[u8_SOS_Stop_Index+1];
					u8_SOS_Stop_Index++;
					}
				 }
				 /**Else we don't need to arrange**/
				 else if(u8_SOS_Stop_Index == sgu16_SOS_index-1)
				 {  

                     /*Decrement the sgu16_SOS_index*/					 
					 sgu16_SOS_index--;
     				 break;
				 } 
				
			}
			else
			{
				/**Do nothing**/
			} 
		}
	    
	}

	return SOS_Dispatch_Error;
}

