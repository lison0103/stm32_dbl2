/*******************************************************************************
* File Name          : led.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : The file contains led functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LED_Init
* Description    : Initialization led.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_Init(void)
{

#ifdef GEC_DBL2_MASTER
      GPIO_InitTypeDef  GPIO_InitStructure;	
  
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
      GPIO_Init(GPIOA, &GPIO_InitStructure);					 
      GPIO_SetBits(GPIOA,GPIO_Pin_15);


#else 
      GPIO_InitTypeDef GPIO_InitStructure;
   
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);  
      GPIO_SetBits(GPIOB,GPIO_Pin_12);
#endif 
}

/******************************  END OF FILE  *********************************/



