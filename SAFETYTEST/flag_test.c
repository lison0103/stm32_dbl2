/**
 * Company	: ThyssenKrupp PDC
 * @file	: flag_test.c
 * @purpose     :
 * @brief	: c based Register flag bit test POST configure test 
 *                  IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 13-May-2014
 * @author	: Paul
*/

#include "flag_test.h"

IEC61508_FlagCheck_struct  FlagCheck;
unsigned int RestFlag;

type_testResult_t flag_test(void)
{

  FlagCheck.Flag_Pass_Cnt = 0;
  FlagCheck.Flag_Err_Cnt = 0;
  
  //Checks whether the FLASH Read Out Protection Status is set or not
#ifdef GEC_DBL2_SLAVE  
  if ( FLASH_OB_GetRDP() == RESET )
#else
  if ( FLASH_GetReadOutProtectionStatus() == RESET)
#endif
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  
  //Checks whether the FLASH Prefetch Buffer status is set or not
#ifndef GEC_DBL2_SLAVE
  if( FLASH_GetPrefetchBufferStatus() != RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
#endif  
  //Checks whether the specified RCC interrupt has occurred or not
  //RCC_IT_LSIRDY: LSI ready interrupt
  if (RCC_GetITStatus(RCC_IT_LSIRDY) == RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_IT_LSERDY: LSE ready interrupt
  if (RCC_GetITStatus(RCC_IT_LSERDY) == RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_IT_HSIRDY: HSI ready interrupt
  if (RCC_GetITStatus(RCC_IT_HSIRDY) == RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_IT_HSERDY: HSE ready interrupt
  if (RCC_GetITStatus(RCC_IT_HSERDY) == RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_IT_PLLRDY: PLL ready interrupt
  if (RCC_GetITStatus(RCC_IT_PLLRDY) == RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }

  //RCC_FLAG_HSIRDY: HSI oscillator clock ready
  if (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_FLAG_HSERDY: HSE oscillator clock ready
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_FLAG_PLLRDY: PLL clock ready
  if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }
  //RCC_FLAG_LSIRDY: LSI oscillator clock ready
  if (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  } 
  //RCC_FLAG_PINRST: Pin reset
  if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
  {  
    FlagCheck.Flag_Pass_Cnt++;
  }
  else
  {
    FlagCheck.Flag_Err_Cnt++;
  }    
    
  //RCC_FLAG_IWDGRST:  IWDG RESET
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    RestFlag = 1;
  }      
  //RCC_FLAG_SFTRST: Software reset
  else if(RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
  {
    RestFlag =  2;  
  }
  //RCC_FLAG_PORRST: Power on reset 
  else if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
  {
    RestFlag = 3;         
  }
  
  if (FlagCheck.Flag_Err_Cnt >= IEC61508_testPassed) 
  {
      return (IEC61508_testFailed);
  }
  else
  {
      return IEC61508_testPassed;
  }
  
}