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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

__IO uint32_t calibration_value = 0;
	   
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
    
    /* Configure the ADC clock */
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
    
    /* Enable ADC1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    ADC_StructInit(&ADC_InitStructure);
    
    /* Calibration procedure */ 
    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
    
    /* Insert delay equal to 10 µs */
    delay_us(10);
    
    ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
    
    while(ADC_GetCalibrationStatus(ADC1) != RESET );
    calibration_value = ADC_GetCalibrationValue(ADC1);
    
    
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                                                                    
    ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;                    
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;             
    ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;                  
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
    
    ADC_CommonInit(ADC1, &ADC_CommonInitStructure);
    
    ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
    ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
    ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Enable;   
    ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
    ADC_InitStructure.ADC_NbrOfRegChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /* ADC1 regular channel7 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5);    
    
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    
    /* wait for ADRDY */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
    
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
  	
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5 );	  			    
//  		
//        ADC_StartConversion(ADC1); 
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);	
}


/*******************************************************************************
* Function Name  : Get_Adc_Average
* Description    : Get the average adc conversion. 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : -1 : conversion calculation is not complete, other: the average adc conversion. 
*******************************************************************************/
s32 Get_Adc_Average(void)
{
	static u32 temp_val = 0;
	static u8 sf_get_adc_times = 0;
        static s32 val = 0;
        
        temp_val += Get_Adc();
        sf_get_adc_times++;
        if( sf_get_adc_times == 5 )
        {
            val = temp_val*60/4096/5;
            temp_val = 0;
            sf_get_adc_times = 0;           
        }
        return val;
} 	 


/******************************  END OF FILE  *********************************/
























