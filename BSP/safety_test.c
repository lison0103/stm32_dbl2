/*******************************************************************************
* File Name          : safety_test.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Last modify date   : 09/07/2016
* Description        : Contains the Self-test functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "safety_test.h"
#include "sys.h"
#include "stm32f10x_STLlib.h"
#include "initial_devices.h"

#include "check_instruction.h"
#include "check_instruction_2.h"
#include "pc_test.h"
#include "flag_test.h"
#include "config_test.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void ConfigurationRegister_StartupCheck(void);
static void ConfigurationRegister_RunCheck(void);
static void FlagRegisterCheck(void);
static void ProgramCounterCheck(void);
static void ProgramExecutionInstructionCheck(void);

/* variable is located in the stack */
static volatile type_testResult_t result = IEC61508_testFailed;   
static u32 SafetyTestFlowCnt = 0u;
static u32 SafetyTestFlowCntInv = 0xFFFFFFFFuL;


/*******************************************************************************
* Function Name  : FailSafeTest
* Description    : Contains the Fail Safe routine executed in case of
*                  failure detected during one of the POR self-test routines
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void FailSafeTest(void)
{
/*  
  while(1)
  {
    NVIC_SystemReset();
  }
*/     
}
    
/*******************************************************************************
* Function Name  : ConfigurationRegister_StartupCheck
* Description    : This function check the configuration register after startup.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
static void ConfigurationRegister_StartupCheck(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    SafetyTestFlowCnt += CONFIGURATION_REG_TEST_CALLER;
    
    RCC_Configuration_8M();
    RCC_GetClocksFreq(&RCC_Clocks); 
    if (RCC_Clocks.SYSCLK_Frequency != 8000000u)
    {
        g_u32InitTestError = 1u;
        FailSafeTest();
    }
    
    RCC_Configuration_72M(); 
    RCC_GetClocksFreq(&RCC_Clocks); 
    if (RCC_Clocks.SYSCLK_Frequency != 72000000u)
    {
        g_u32InitTestError = 1u;
        FailSafeTest();
    }       
    
    
    SafetyTestFlowCntInv -= CONFIGURATION_REG_TEST_CALLER;
}

/*******************************************************************************
* Function Name  : ConfigurationRegister_RunCheck
* Description    : This function check the configuration register during running.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ConfigurationRegister_RunCheck(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    SafetyTestFlowCnt += CONFIGURATION_REG_TEST_CALLER;
        
    RCC_GetClocksFreq(&RCC_Clocks); 
    if (RCC_Clocks.SYSCLK_Frequency != 72000000u)
    {
        g_u16RunTestError = 1u;
        FailSafeTest();
    }
    /* Wait till PLL is used as system clock source */ 
    if (RCC_GetSYSCLKSource() != 0x08u ) 
    {
        g_u16RunTestError = 1u;
        FailSafeTest();
    } 
    result = ConfigurationRegister_Check();
#if 0    
    if (result != IEC61508_testPassed)
    {
        FailSafeTest();                           
    }
    else
    {
        SafetyTestFlowCntInv -= CONFIGURATION_REG_TEST_CALLER;
    }  
#else    
    SafetyTestFlowCntInv -= CONFIGURATION_REG_TEST_CALLER;
#endif
}

/*******************************************************************************
* Function Name  : FlagRegisterCheck
* Description    : Check the flag register.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void FlagRegisterCheck(void)
{
    SafetyTestFlowCnt += FLAG_TEST_CALLER;
    result = flag_test();
    if (result != IEC61508_testPassed)
    {
        FailSafeTest();                           
    }
    else
    {
        SafetyTestFlowCntInv -= FLAG_TEST_CALLER;
    }
}

/*******************************************************************************
* Function Name  : ProgramCounterCheck
* Description    : Check the program counter.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ProgramCounterCheck(void)
{
    SafetyTestFlowCnt += PC_TEST_CALLER;
    result = IEC61508_PCTest_POST();
    if (result != IEC61508_testPassed)
    {
        FailSafeTest();                                    
    }   
    else
    {
        SafetyTestFlowCntInv -= PC_TEST_CALLER;
    }
}

/*******************************************************************************
* Function Name  : ProgramExecutionInstructionCheck
* Description    : Check the program execution instruction and accumulator.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ProgramExecutionInstructionCheck(void)
{
    SafetyTestFlowCnt += PEI_TEST_CALLER;
    /* Do the IEC61508 instruction tests */
    if (iec61508_InstCheck_POST() == IEC61508_testFailed)
    {            
        FailSafeTest();
    }
    else
    {
        SafetyTestFlowCntInv -= PEI_TEST_CALLER;
    }
}



/*******************************************************************************
* Function Name  : Safety_StartupCheck2
* Description    : This function contains self test after startup.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Safety_StartupCheck2(void)
{    
    /* Get the Safety_StartupCheck1 startup init test result */
#ifdef GEC_DBL2_SLAVE
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    g_u32InitTestError = RTC_ReadBackupRegister(RTC_BKP_DR3);
#else
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE );
    g_u32InitTestError = BKP_ReadBackupRegister(BKP_DR3);
#endif
             
      /*----------------------------------------------------------------------*/
      /*---------------------- Configuration registers -----------------------*/
      /*----------------------------------------------------------------------*/  
      ConfigurationRegister_StartupCheck();
        
      /*----------------------------------------------------------------------*/
      /*--------------------------- FLAG registers ---------------------------*/
      /*----------------------------------------------------------------------*/
      FlagRegisterCheck();

      /*----------------------------------------------------------------------*/
      /*------------------- program execution instruction --------------------*/
      /*------------------------- Accumulator Test ---------------------------*/
      /*----------------------------------------------------------------------*/  
      ProgramExecutionInstructionCheck();

      /*----------------------------------------------------------------------*/
      /*---------------- Check Safety routines Control flow  -----------------*/
      /*----------------------------------------------------------------------*/        
       if (((SafetyTestFlowCnt ^ SafetyTestFlowCntInv) != 0xFFFFFFFFuL)
          ||(SafetyTestFlowCnt != CHECKCNT ))  
       {
           g_u32InitTestError = 1u;
       }
       else
       {
           SafetyTestFlowCnt = 0u;
           SafetyTestFlowCntInv = 0xFFFFFFFFuL;          
       }
        

}

/*******************************************************************************
* Function Name  : Safety_RunCheck2
* Description    : This function contains self test during running.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Safety_RunCheck2(void)
{ 
    
    static u32 stat_u32CheckTimePeriod = 0u;
    
    stat_u32CheckTimePeriod++;
    if( ( stat_u32CheckTimePeriod * 5u ) >= RUNCHECK_TIME_PERIOD )
    {
        stat_u32CheckTimePeriod = 0u;
        
        /*----------------------------------------------------------------------*/
        /*---------------------- Configuration registers -----------------------*/
        /*----------------------------------------------------------------------*/  
        ConfigurationRegister_RunCheck();            
        
        /*----------------------------------------------------------------------*/
        /*--------------------------- FLAG registers ---------------------------*/
        /*----------------------------------------------------------------------*/
        FlagRegisterCheck();
        
        /*----------------------------------------------------------------------*/
        /*------------------------------- PC Test ------------------------------*/
        /*----------------------------------------------------------------------*/ 
        ProgramCounterCheck();
        
        /*----------------------------------------------------------------------*/
        /*------------------- program execution instruction --------------------*/
        /*------------------------- Accumulator Test ---------------------------*/
        /*----------------------------------------------------------------------*/  
        ProgramExecutionInstructionCheck();
        
        /*----------------------------------------------------------------------*/
        /*---------------- Check Safety routines Control flow  -----------------*/
        /*----------------------------------------------------------------------*/        
        if (((SafetyTestFlowCnt ^ SafetyTestFlowCntInv) != 0xFFFFFFFFuL)
            ||(SafetyTestFlowCnt != CHECKCNTRUN ))  
        {
            g_u16RunTestError = 1u;
        }
        else
        {
            SafetyTestFlowCnt = 0u;
            SafetyTestFlowCntInv = 0xFFFFFFFFuL;          
        }
        
    }
}


/******************************  END OF FILE  *********************************/
