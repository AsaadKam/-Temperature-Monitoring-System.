/*
 * ADC.c
 *
 * Created: 06/01/2020 13:43:09	
 *  Author: Native_programmer
 */ 

#define ADSC 6
/*- INCLUDES ----------------------------------------------*/
#include "ADC.h"
#include "Timer.h"
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL Dataypes ----------------------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
static ADC_Error_t sgADC_Error_ADC_INIT_API=ADC_INIT_API_NO_ERROR;
static ADC_Error_t sgADC_Error_ADC_READ_API=ADC_READ_API_NO_ERROR;
static uint8_t sgu8_ADC_Execution_Way=0;
static uint8_t sgu8_ADC_Left_Adjust_Enabled_Flag=0;
static volatile PntrToFunc_t sgPntrToFun_ADC_ISR=NullPointer;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/
ADC_Error_t ADC_INIT(gstrADC_Config_t*CopyPntrStr_ADC_Init_Config)
 { 
      if(NullPointer!=CopyPntrStr_ADC_Init_Config)
	  {
		    /***Enable ADC unit***/
			ADC_Unit_Enable();

			/**** Configure the input channel for ADC****/
			if( ( (CopyPntrStr_ADC_Init_Config->ADC_Channel_Choose)<ADC_Channel_0                ) || ( (CopyPntrStr_ADC_Init_Config->ADC_Channel_Choose)>ADC_Channel_7         ) ) 
			{
			
				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_CHANNEL_CHOOSE ;
			}
			else
			{

				ADC_Channel_Set(CopyPntrStr_ADC_Init_Config->ADC_Channel_Choose);	 
			}
			/**** Configure ADC Refrence voltage****/
			if( ( (CopyPntrStr_ADC_Init_Config->ADC_Reference_Voltage)<ADC_Voltage_Refrence_AREF) || ( (CopyPntrStr_ADC_Init_Config->ADC_Reference_Voltage)>ADC_Voltage_Internal ) )
			{

				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_VOLTAGE_REFRENCE_CHOOSE ;
			}
			else
			{
				ADC_Voltage_Refrence_Set(CopyPntrStr_ADC_Init_Config->ADC_Reference_Voltage); 	 
			}
			/**** Configure ADC sampling frequency ****/
			if( ( (CopyPntrStr_ADC_Init_Config->ADC_Frequency)<ADC_Freq_4MHZ                     ) || ( (CopyPntrStr_ADC_Init_Config->ADC_Frequency)        >ADC_Freq_125KHZ       ) ) 
			{
			
				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_FREQUENCY_CHOOSE ;
			}
			else
			{
				ADC_Choose_Frequency(CopyPntrStr_ADC_Init_Config->ADC_Frequency);
			}
			/**** Configure Auto Trigger if it is requested by the user ****/
			if( (CopyPntrStr_ADC_Init_Config->ADC_Auto_Trig_Enable)==ADC_Auto_Trigger_Enable ) 
			{
		
				ADC_Auto_Trigger_Set();
			}
			else if ( (CopyPntrStr_ADC_Init_Config->ADC_Auto_Trig_Enable)==ADC_Auto_Trigger_Disable ) 
			{
				/*Do Nothing*/
			}
			else
			{
				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_NEITHER_TRIGGER_ENABLE_NOR_DISABLE;
		 
			}
			/**** Configure Source of auto triggering  ****/
			if( ( (CopyPntrStr_ADC_Init_Config->ADC_Auto_Trig_Source)<ADC_Auto_Trig_Source_Free_Running_mode_Bits      ) ||  (CopyPntrStr_ADC_Init_Config->ADC_Auto_Trig_Source)>ADC_No_Need_To_Trigger  )
			{
				
				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_NO_DEFINED_TRIGGER ;		 
			} 
			else if((CopyPntrStr_ADC_Init_Config->ADC_Auto_Trig_Source)!=ADC_No_Need_To_Trigger  )
			{
		 	    ADC_Auto_Trigger_Source_Set(CopyPntrStr_ADC_Init_Config->ADC_Auto_Trig_Source);
		 	 
			}
			else
			{
				/*Do Nothing*/
			}
			/**** Configure Left or Right Data Register ****/
			if(  (CopyPntrStr_ADC_Init_Config->ADC_Left_Right_Adjustment)==ADC_Left_Adjustment    )  
			{
			
				ADC_Data_Left_Adjustment_Set(); 
				sgu8_ADC_Left_Adjust_Enabled_Flag=ADC_Left_Adjustment;
			}
			else if( (CopyPntrStr_ADC_Init_Config->ADC_Left_Right_Adjustment)==ADC_Right_Adjustment )
			{
		 
				sgu8_ADC_Left_Adjust_Enabled_Flag=ADC_Right_Adjustment;
			}	
			else
			{
				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_NEITHER_LEFT_NOR_RIGHT;			 
		
			}
			/**Configure Execution Way**/
			if( ( (CopyPntrStr_ADC_Init_Config->ADC_Execution_Method)<ADC_Executed_By_Interrupt      ) || ( (CopyPntrStr_ADC_Init_Config->ADC_Execution_Method)>ADC_Executed_By_Event_Driven  ) )
			{
				
				sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_NO_DEFINED_EXECUTION_METHOD ;
			}
			else if(  CopyPntrStr_ADC_Init_Config->ADC_Execution_Method ==ADC_Executed_By_Interrupt || CopyPntrStr_ADC_Init_Config->ADC_Execution_Method ==ADC_Executed_By_Event_Driven)
			{
				sgu8_ADC_Execution_Way=CopyPntrStr_ADC_Init_Config->ADC_Execution_Method;
				EnableGeneralInterrupt();

			}
			else 
			{
				
				sgu8_ADC_Execution_Way=CopyPntrStr_ADC_Init_Config->ADC_Execution_Method;
			}
	}
	else
	{
		sgADC_Error_ADC_INIT_API=ADC_INIT_ERROR_Config_Pointer_Point_To_NULL;
	}
		 return  sgADC_Error_ADC_INIT_API;     
 }
 
ADC_Error_t ADC_READ(uint32_t* Copyu32_ADC_Value,PntrToFunc_t Copy_PntrToFunc_Requsted_From_User_Called_In_ADC_ISR,uint8_t* Copy_Pntr_u8_ADC_READ_TIME_EVENT_FLAG)
 {

     int Ain,AinLow;
     static uint8_t Event_Driven_1st_time=1;
	 static uint8_t sgu8_ADC_READ_STATE=0;

     /*Check that there is no error occur while initialization*/	 
	 if(sgADC_Error_ADC_INIT_API==ADC_INIT_API_NO_ERROR)
	 { 		
/*
		DIO_Init_Pin(30,1);
		DIO_toggle_Pin(30);		 */ 	 
         /**Check If the execution way is done by polling **/ 	
		 if(sgu8_ADC_Execution_Way==ADC_Executed_By_Polling)
		 {

			 ADC_Start_Conversion();

			 while(ADC_Read_Interrupt_Flag_State()==0);

             /**Check If the data is left adjusted or right**/ 			 
			 if(sgu8_ADC_Left_Adjust_Enabled_Flag==ADC_Left_Adjustment)
			 {				
				  *Copyu32_ADC_Value=((uint32_t)(ADC_DATA_HIGH_REG<<2))|(uint8_t)(ADC_DATA_LOW_REG>>6);
			 }
			 else
			 {
					AinLow = (uint32_t)ADC_DATA_LOW_REG;		/* Read lower byte*/
					Ain = (uint32_t)(ADC_DATA_HIGH_REG<<8);		/* Read higher 2 bits and*/
					*Copyu32_ADC_Value=Ain|AinLow ;
			 }
		 }
         /**Check If the execution way is done by interrupt or event or time driven **/ 		 
		 else
		 {
			 
			 if(sgu8_ADC_Execution_Way!=ADC_Executed_By_Time_Driven)
			 {
				
			    /*while(ADC_Read_Interrupt_Flag_State()==0);*/
				/**Check If PntrToFunction does'nt point to NullPointer**/
				if(NullPointer!=Copy_PntrToFunc_Requsted_From_User_Called_In_ADC_ISR)
				{
				 
					sgPntrToFun_ADC_ISR=Copy_PntrToFunc_Requsted_From_User_Called_In_ADC_ISR;
				}
				else
				{
					sgADC_Error_ADC_READ_API=ADC_READ_CALL_BACK_POITNER_IS_NULL;
				}
				/**Check If the data is left adjusted or right**/
				if(sgu8_ADC_Left_Adjust_Enabled_Flag==ADC_Left_Adjustment)
				{
						             
					*Copyu32_ADC_Value=((uint32_t)(ADC_DATA_HIGH_REG<<2))|(uint8_t)(ADC_DATA_LOW_REG>>6);
				}
				else
				{
					AinLow = (uint32_t)ADC_DATA_LOW_REG;		/* Read lower byte*/
					Ain = (uint32_t)(ADC_DATA_HIGH_REG<<8);		/* Read higher 2 bits and*/
					*Copyu32_ADC_Value=Ain|AinLow ;
				}
			 }
			 else
			 {
				switch(sgu8_ADC_READ_STATE)
				{
					case 0:
					{
				        ADC_Start_Conversion();
						sgu8_ADC_READ_STATE=1;
					}

					break;
					
					case 1:
					{

/*							SET_BIT(ADC_CNTRL_STATUS_REG,ADC_Interrupt_flag_Bit);*/
                      		/**Check If the data is left adjusted or right**/
                      		if(sgu8_ADC_Left_Adjust_Enabled_Flag==ADC_Left_Adjustment)
                      		{
	                      					
	                      		*Copyu32_ADC_Value=((uint32_t)(ADC_DATA_HIGH_REG<<2))|(uint8_t)(ADC_DATA_LOW_REG>>6);
                      		}
                      		else
                      		{
								AinLow = (uint32_t)ADC_DATA_LOW_REG;		/* Read lower byte*/
								Ain = (uint32_t)(ADC_DATA_HIGH_REG<<8);		/* Read higher 2 bits and*/
							   *Copyu32_ADC_Value=Ain|AinLow ;	
                      		}			
							sgu8_ADC_READ_STATE=0;
							*Copy_Pntr_u8_ADC_READ_TIME_EVENT_FLAG=0;
						}									

					break;
					
				}

				
			 }
	 		 
		 }
	 }
	 else
	 {
		 sgADC_Error_ADC_READ_API=ADC_READ_ERROR_Due_To_ADC_INIT_ERROR;
	 }
	 return sgADC_Error_ADC_READ_API; 
 }
 
ADC_ISR_CODE()
{
	DIO_Init_Pin(26,1);
	DIO_toggle_Pin(26);
	
	TIMER_CLR_INT_FLAG(TIMER1_B_COMP_FLAG);
	
	if(NullPointer!=sgPntrToFun_ADC_ISR)
	{
			sgPntrToFun_ADC_ISR();

  	}
	else
	{
		/*Do Nothing*/
	}

	
}