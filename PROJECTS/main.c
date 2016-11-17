/*******************************************************************************
* File Name          : main.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Main program body.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "initial_devices.h"
#include "safety_test.h"
#include "stm32f10x_STLlib.h"
#include "stm32f10x_STLclassBvar.h"
#include "bsp_input.h"
#include "esc_comm_dualcpu.h"
#include "esc_comm_safety.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u16 Tms10Counter = 0,Tms20Counter = 0,Tms50Counter = 0,Tms100Counter = 0,Tms500Counter = 0,Tms1000Counter = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u32 TimingDelay = 0;
u32 SysRunTime = 0; 
u8 testmode = 0;

/* ESC */
DBL2EscData EscData;
DBL2EscData OmcEscData;

/* safety board data */
u8 EscDataToSafety[4][8];
u8 EscDataFromSafety[2][8];


/*******************************************************************************
* Function Name  : LED_indicator
* Description    : LED indicator flashes Timing                 
* Input          : None               
* Output         : None
* Return         : None
*******************************************************************************/
void LED_indicator(void)
{
	static u32 led_idr_cnt = 0;	 
	
	led_idr_cnt++;
	 
	if(led_idr_cnt >= 100)   
	{
            led_idr_cnt = 0;
            LED_FLASH();  
	}   
}


/*******************************************************************************
* Function Name  : Task_Loop
* Description    : This function contains the task to be executed in a loop.               
* Input          : None                
* Output         : None
* Return         : None
*******************************************************************************/
void Task_Loop(void)
{          

      if( ++Tms10Counter >= 2 ) Tms10Counter = 0;
      if( ++Tms20Counter >= 4 ) Tms20Counter = 0;
      if( ++Tms50Counter >= 9 ) Tms50Counter = 0;
      if( ++Tms100Counter >= 19 ) Tms100Counter = 0;
      if( ++Tms500Counter >= 99 ) Tms500Counter = 0;
      if( ++Tms1000Counter >= 200 ) Tms1000Counter = 0;      

#if SELF_TEST      
      /* self check */
      Safety_RunCheck1();     
      Safety_RunCheck2();
#endif  
      
      Safety_Receive_Data_Process();
      
#ifdef GEC_DBL2_MASTER  
      if( testmode == 1 )
      {
          EscData.SwdpAdr = ReadSwDp();
      }
#endif
      
      Get_GpioInput(&EscData.DBL2InputData[0]);
#ifdef GEC_DBL2_SLAVE      
      output_driver(&EscData.DBL2OutputData);
      Communication_To_Safety();
      Communication_CPU();
#endif

      
      /*  ESC  */
      if( testmode == 0 )
      {
          DBL2GetAdr();
      }

      
#ifdef GEC_DBL2_MASTER 
      if( Tms10Counter == 0 )
      {
          Communication_CPU();
          Safety_Send_Data_Process(0u); 
      }      
      if( Tms20Counter == 0 )
      {                

      }  
#else
      if( Tms10Counter == 0 )
      {
                   
      }
      if( Tms20Counter == 0 )
      {
           
      }       
#endif     
      
      if( Tms50Counter == 0 )
      {                                 
          /* Reload EWDT counter */          
          EWDT_TOOGLE();
          
      } 
      
      if( Tms100Counter == 0 )
      {   
#ifdef GEC_DBL2_SLAVE         
          EscData.DBL2AnalogData[0] = Get_Adc_Average(1);
          EscData.DBL2AnalogData[1] = Get_Adc_Average(2);
          EscData.DBL2AnalogData[2] = Get_RTD_Value();
#endif
      }
           
      if( Tms500Counter == 0 )
      {    
#ifdef GEC_DBL2_SLAVE
          Input_Check();         
#endif
      }
      
      if( Tms1000Counter == 0 )
      { 

      }
     

}


/*******************************************************************************
* Function Name  : main
* Description    : Main program.             
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{        

    u16 i; 
    
    /* Power up delay */
    for( i = 0; i < 10000; i++ );
    
    /** hardware init **/
    Initial_Device();    
    
    while(1)
    {
      
        /* 5ms */
        while ( !TimingDelay );
        TimingDelay = 0;

        Task_Loop();
        LED_indicator();
   
    }          
          
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/******************************  END OF FILE  *********************************/



