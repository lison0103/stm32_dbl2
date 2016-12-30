/*******************************************************************************
* File Name          : ewdt.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : This file contains the external watchdog functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"
#include "ewdt.h"
#include "esc_error_process.h"
#include "config_test.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : EWDT_Drv_pin_config
* Description    : Configure the pin.
*                  the external watchdog reset  min:1.12 typ:1.6 max:2.4
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void EWDT_Drv_pin_config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
#ifdef GEC_DBL2_MASTER
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE );
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA , &GPIO_InitStruct);
#else           
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC , &GPIO_InitStruct);
#endif
    
}

/*******************************************************************************
* Function Name  : write_bkp
* Description    : This function write the cpu backup register to store data.
*                  
* Input          : adr: the address of backup register
*                  dat: data to be written
* Output         : None
* Return         : None
*******************************************************************************/
void write_bkp(u32 adr,u32 dat)
{
  PWR_BackupAccessCmd(ENABLE);
#ifdef GEC_DBL2_SLAVE
  RTC_WriteBackupRegister(adr, dat);    
#else
  BKP_WriteBackupRegister((u16)adr, (u16)dat);
#endif   
  /*PWR_BackupAccessCmd(DISABLE);*/
}

/*******************************************************************************
* Function Name  : ExternalWatchdogCheck
* Description    : Check the external watchdog.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void ExternalWatchdogCheck(void)
{
    u32 iwdg_check_flag = 0u;
    u8 u8EwdtError = 0u;
    
#ifdef GEC_DBL2_SLAVE    
    u32 bkr_rst_flag = 0u;
#else
    u16 bkr_rst_flag = 0u;  
#endif
  
#ifdef GEC_DBL2_SLAVE
      iwdg_check_flag = RTC_ReadBackupRegister(RTC_BKP_DR2);
#else
      iwdg_check_flag = BKP_ReadBackupRegister(BKP_DR2);
#endif
  if( iwdg_check_flag == 0u )
  {
      
     /* Clear the init test error flag */
#ifdef GEC_DBL2_SLAVE
          write_bkp(RTC_BKP_DR3, 0u);  
#else
          write_bkp(BKP_DR3, 0u);
#endif 
          
      RCC_Configuration_72M();
      Delay_Init();
      
      delay_ms(10u); 
      EWDT_TOOGLE();
      delay_ms(10u);    
      EWDT_TOOGLE();
      delay_ms(10u);    
      EWDT_TOOGLE();
        
#ifdef GEC_DBL2_SLAVE
      bkr_rst_flag = RTC_ReadBackupRegister(RTC_BKP_DR1);
#else
      bkr_rst_flag = BKP_ReadBackupRegister(BKP_DR1);
#endif
      if(bkr_rst_flag == 0xfa01u)
      {
    /** soft reset **/
/*          
    if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET)
    {
        
    }
*/
        /** pin reset **/
        if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != SET)
        {
#ifdef GEC_DBL2_SLAVE
            write_bkp(RTC_BKP_DR1, 0u);  
#else
            write_bkp(BKP_DR1, 0u);
#endif  
            RCC_ClearFlag();   
            u8EwdtError = 0x01u;
        }   
      }  
      else
      {

#ifdef GEC_DBL2_SLAVE
          write_bkp(RTC_BKP_DR1, 0xfa01u); 
#else  
          write_bkp(BKP_DR1, 0xfa01u);  
#endif  
          RCC_ClearFlag();
          
          delay_ms(2000u);    
          
          u8EwdtError = 0x01u;
      }  
      
#ifdef GEC_DBL2_SLAVE
      write_bkp(RTC_BKP_DR1, 0u); 
#else  
      write_bkp(BKP_DR1, 0u);  
#endif  
      
      if( u8EwdtError == 1u ) 
      {
/*          
          ESC_EWDT_Error_Process();
*/
     /* Reocrd the init test error */
#ifdef GEC_DBL2_SLAVE
          write_bkp(RTC_BKP_DR3, 1u);  
#else
          write_bkp(BKP_DR3, 1u);
#endif           
      }
      
  }
  
} 



/******************************  END OF FILE  *********************************/


