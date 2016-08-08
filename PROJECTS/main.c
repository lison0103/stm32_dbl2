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
u8 Modbuff[3000];
u8 EscRTBuff[200];
u8 McRxBuff[1000];
u8 *const pcOMC_EscRTBuff = &McRxBuff[0]; 
u8 *const pcOmcErrorBuff = &McRxBuff[70];
u8 *const pcErrorBuff = &EscRTBuff[70];

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
      
#ifdef GEC_DBL2_MASTER  
      if( testmode == 1 )
      {
          EscRTBuff[0] = ReadSwDp();
      }
#endif
      
      Get_GpioInput(&EscRTBuff[4]);
#ifdef GEC_DBL2_SLAVE      
      output_driver(&EscRTBuff[30]);
#endif

      
      /*  ESC  */
      if( testmode == 0 )
      {

      }

      
#ifdef GEC_DBL2_MASTER 
      if( Tms10Counter == 0 )
      {

      }      
      if( Tms20Counter == 0 )
      {                
          Communication_CPU();
           
      }  
#else
      if( Tms10Counter == 0 )
      {
          Communication_CPU();         
      }
      if( Tms20Counter == 0 )
      {
          Communication_To_Safety(); 
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
          CAN1_TX_Data[2] = Get_Adc_Average();
          CAN1_TX_Data[3] = SPI_MAX31856_Send(0xaa);
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



