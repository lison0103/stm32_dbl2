/*******************************************************************************
* File Name          : initial_devices.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Contains the devices initinal.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "initial_devices.h"
#include "stm32f10x_STLlib.h"
#include "stm32f10x_STLclassBvar.h"
#include "check_instruction.h"
#include "check_instruction_2.h"
#include "pc_test.h"
#include "flag_test.h"
#include "config_test.h"
#include "esc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void PVD_Configuration(void);
static void SysTickTimerInit(void);
static void PluseOutputInit(void);
static void Data_init(void);
static void RCC_Configuration(void);
static void NVIC_Configuration(void);


/*******************************************************************************
* Function Name  : Initial_Device
* Description    : Initialize the CPU1,CPU2 and peripherals
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Initial_Device(void)
{

        /* self test ---------------------------------------------------------*/
#if SELF_TEST
        Safety_StartupCheck2();
#endif   
        /** set system interrupt priority group 2 **/
	NVIC_Configuration();
        
        /** RCC  and  PVD Configuration **/
        RCC_Configuration();   
        PVD_Configuration();
        
        /** delay init **/
	Delay_Init();  

        /** LED init **/
	LED_Init();
        
        /* EWDT init */
        EWDT_Drv_pin_config();
/*        ExtWdtCheck();*/
        
        /** input and relay output init **/
        Input_Output_PinInit();                
        
        /* SW DP SWITCH */
        SW_DP_Init();
        
        /** Data array is initialized to 0 **/
        Data_init();
        
        
#ifdef GEC_DBL2_SLAVE           
            
        /** CAN1 init,baud rate 500Kbps **/
	CAN_Int_Init(CAN1);     
        
        /* ADC init, measure the sf_in voltage */
        Adc_Init();
        
        /* MAX31865 init */
        SPI_MAX31865_Init();
        
#else
        /* wait for CPU2 */
        delay_ms(500u);
      
#endif  

        /** spi communication init **/
        SPIx_Init();
        
        /*----------------------------------------------------------------------*/
        /*------------------------- Cross Comm CPU test ------------------------*/
        /*------------------------- Data Integrity Test ------------------------*/
        /*----------------------------------------------------------------------*/   
        
        CrossCommCPUCheck();
           
        /* HardwareTest */
        /*HardwareTEST();*/


        /** pluse output init **/
        PluseOutputInit();        
        
#if SELF_TEST
        /* Self test routines initialization ---------------------------------------*/
        Safety_InitRunTimeChecks();
#endif
        /* systick timer , 5ms interrupt */
        SysTickTimerInit();
}



/*******************************************************************************
* Function Name  : PluseOutputInit
* Description    : Initialization the pluse output.
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
static void PluseOutputInit(void)
{
    
    SFSW_C_1_SET();
    SFSW_C_2_SET();
    SFSW_C_3_SET();
    SFSW_C_4_SET();
    SFSW_C_5_SET();

}

/*******************************************************************************
* Function Name  : SysTickTimerInit
* Description    : Initialization the systick timer.
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
static void SysTickTimerInit(void)
{
  
    /** interrupt time 5ms **/
    if(SysTick_Config(SystemCoreClock / ( 1000u / SYSTEMTICK )))
    {
        /* Capture error */
        while (1u)
        {}
    }

}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void RCC_Configuration(void)
{

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2); 

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
#ifdef GEC_DBL2_SLAVE
    FLASH_PrefetchBufferCmd(ENABLE);
#else
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
#endif

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08u)
    {
    }

    
    /* Enable GPIO and Peripherals clocks */ 
#ifdef GEC_DBL2_SLAVE
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1 , ENABLE);    

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 , ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2
                           |RCC_APB1Periph_TIM4 ,
                            ENABLE); 
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG | RCC_APB2Periph_SPI1, ENABLE);

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA
                           |RCC_AHBPeriph_GPIOB
                           |RCC_AHBPeriph_GPIOC
                           |RCC_AHBPeriph_GPIOD
                           |RCC_AHBPeriph_GPIOE ,
                           ENABLE);
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_CRC, ENABLE);
#else     
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE );    
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1 , ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2
                           |RCC_APB1Periph_TIM4 ,
                            ENABLE);  
                                        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 , ENABLE);
  
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA
                           |RCC_APB2Periph_GPIOB
                           |RCC_APB2Periph_GPIOC
                           |RCC_APB2Periph_GPIOD
                           |RCC_APB2Periph_GPIOE ,
                           ENABLE); 
  

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_CRC, ENABLE); 
#endif
          
      
      
    /* Enable CSS */
    RCC_ClockSecuritySystemCmd(ENABLE);
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configuring NVIC interrupt group.
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
  
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0u);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
    
}

/*******************************************************************************
* Function Name  : PVD_Configuration
* Description    : Configuring system Voltage Detector.
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
static void PVD_Configuration(void)
{
  
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    EXTI_InitStructure.EXTI_Line = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the PVD Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0u;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0u;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
#ifdef GEC_DBL2_SLAVE
    PWR_PVDLevelConfig(PWR_PVDLevel_4);  
#else
    PWR_PVDLevelConfig(PWR_PVDLevel_2V5);
#endif
    PWR_PVDCmd(ENABLE);

}


/*******************************************************************************
* Function Name  : Data_init
* Description    : Data initialzation.
*                  
* Input          : None
*                 
* Output         : None
* Return         : None
*******************************************************************************/
static void Data_init(void)
{

    

    

}

/******************************  END OF FILE  *********************************/


