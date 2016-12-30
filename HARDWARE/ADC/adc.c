/*******************************************************************************
* File Name          : adc.c
* Author             : lison
* Version            : V1.0
* Date               : 08/07/2016
* Description        : This file contains adc conversion functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC_CDR_ADDRESS    ((uint32_t)0x5000030C)

/* Private macro -------------------------------------------------------------*/
#define Sample_Num       5u

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static __IO uint16_t ADCDualConvertedValue[Sample_Num][2];
static __IO uint32_t calibration_value = 0u;
	   
/*******************************************************************************
* Function Name  : Adc_Init
* Description    : Initialization ADC.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/		   																   
void  Adc_Init(void)
{ 	
    GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef       ADC_InitStructure;
    DMA_InitTypeDef        DMA_InitStructure;
    
    /* Configure the ADC clock */
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
    
    /* Enable ADC1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /* DMA configuration */
    /* DMA1 Channel1 Init Test */
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_CDR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCDualConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = Sample_Num*2u;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
  
    ADC_StructInit(&ADC_InitStructure);
    
    /* Calibration procedure */ 
    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
    
    /* Insert delay equal to 10  */
    delay_us(10u);
    
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
    
    while(ADC_GetCalibrationStatus(ADC1) != RESET )
    {}
    calibration_value = ADC_GetCalibrationValue(ADC1);
    
    
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                                                                    
    ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;                    
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;             
    ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_Circular;                  
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0xfu;          
    
    ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
    
    ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
    ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
    ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
    ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
    ADC_InitStructure.ADC_NbrOfRegChannel = 2u;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /* ADC1 regular channel1 and channel2 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1u, ADC_SampleTime_601Cycles5); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2u, ADC_SampleTime_601Cycles5); 

    /* Configures the ADC DMA */
    ADC_DMAConfig(ADC1, ADC_DMAMode_Circular);
    /* Enable the ADC DMA */
    ADC_DMACmd(ADC1, ENABLE);
  
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /* wait for ADCRDY */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY))
    {}

    /* Enable the DMA channel */
    DMA_Cmd(DMA1_Channel1, ENABLE);
  
    /* Start ADC1 Software Conversion */ 
    ADC_StartConversion(ADC1);  


}	

/*******************************************************************************
* Function Name  : Get_Adc
* Description    : Get the adc value.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : Returns the results of a recent conversion ADC1 rule groups.
*******************************************************************************/
u16 Get_Adc(void)   
{  	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
        {}

	return ADC_GetConversionValue(ADC1);	
}


/*******************************************************************************
* Function Name  : Get_Adc_Average
* Description    : Get the average adc conversion. 
*                  
* Input          : channel:  ADC1 channel number
*                  None
* Output         : None
* Return         : The average adc value of ADC1 channel. 
*******************************************************************************/
u32 Get_Adc_Average(u8 channel)
{
        uint8_t i;
        uint32_t sum = 0u;

        for( i = 0u; i < Sample_Num; i++ )
        {
            sum += ADCDualConvertedValue[i][channel - 1u];
        }

        return (sum*60u/4096u/Sample_Num);
        
} 	 


/******************************  END OF FILE  *********************************/
























