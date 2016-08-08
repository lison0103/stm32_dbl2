/*******************************************************************************
* File Name          : bsp_iocfg.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Contains input and output pin Initializes.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "bsp_iocfg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : Input_Output_PinInit
* Description    : Initializes the Input and Output GPIO
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Input_Output_PinInit(void)
{
  
      GPIO_InitTypeDef GPIO_InitStruct;
      
#ifdef GEC_DBL2_MASTER
      
      GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;      

      /** SAFETY input gpio ------------------------------------------------ **/     
      /* INA1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INA2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INA3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INA4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INA5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA6 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA7 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA8 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA9 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA10 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA11 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
      GPIO_Init(GPIOD , &GPIO_InitStruct);         

      /* INA12 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INA13 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
      GPIO_Init(GPIOC , &GPIO_InitStruct); 

      /* INA14 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
      GPIO_Init(GPIOC , &GPIO_InitStruct);   

      
      /** SAFETY switch input gpio ----------------------------------------- **/
      /* SFSW_INA1 1*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOA , &GPIO_InitStruct);   

      /* SFSW_INA2 6*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOC , &GPIO_InitStruct);   

      /* SFSW_INA3 11*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INA4 15*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INA5 2*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOA , &GPIO_InitStruct);   

      /* SFSW_INA6 7*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INA7 12*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INA8 16*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INA9 3*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOA , &GPIO_InitStruct);   

      /* SFSW_INA10 8*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INA11 13*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
      GPIO_Init(GPIOE , &GPIO_InitStruct);         

      /* SFSW_INA12 17*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INA13 4*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOA , &GPIO_InitStruct); 

      /* SFSW_INA14 9*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INA15 14*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /* SFSW_INA16 18*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INA17 5*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOC , &GPIO_InitStruct); 

      /* SFSW_INA18 10*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOE , &GPIO_InitStruct);       

      
      /** Safety switch feedback gpio -------------------------------------- **/ 
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;       
        
      /* SFSW_C_F_A1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);      
      
      /* SFSW_C_F_A2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);        

      /* SFSW_C_F_A3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);       

      /* SFSW_C_F_A4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);        

      /* SFSW_C_F_A5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct); 
  
      /** SINA_BUSY_BA -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; 
      GPIO_Init(GPIOD , &GPIO_InitStruct);  

      /** SYNC_SYS_BA -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8; 
      GPIO_Init(GPIOD , &GPIO_InitStruct);           

      /** Safety switch output gpio ---------------------------------------- **/
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

      /* SFSW_C_A1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);      
      
      /* SFSW_C_A2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);        

      /* SFSW_C_A3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);       

      /* SFSW_C_A4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);        

      /* SFSW_C_A5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct); 
      
      /** SINA_BUSY_AB -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; 
      GPIO_Init(GPIOD , &GPIO_InitStruct);  

      /** SYNC_SYS_AB -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 
      GPIO_Init(GPIOD , &GPIO_InitStruct);  

      
#else
      

      
                
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;   


      /** SAFETY input gpio ------------------------------------------------ **/     
      /* INB1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INB2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INB3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INB4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* INB5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB6 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB7 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB8 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB9 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB10 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB11 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
      GPIO_Init(GPIOD , &GPIO_InitStruct);         

      /* INB12 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOD , &GPIO_InitStruct);   

      /* INB13 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
      GPIO_Init(GPIOC , &GPIO_InitStruct); 

      /* INB14 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
      GPIO_Init(GPIOC , &GPIO_InitStruct);   

      
      /** SAFETY switch input gpio ----------------------------------------- **/
      /* SFSW_INB1 1*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_Init(GPIOA , &GPIO_InitStruct);   

      /* SFSW_INB2 6*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INB3 11*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INB4 15*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INB5 2*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOA , &GPIO_InitStruct);   

      /* SFSW_INB6 7*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INB7 12*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INB8 16*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INB9 3*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
      GPIO_Init(GPIOC , &GPIO_InitStruct);   

      /* SFSW_INB10 8*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INB11 13*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
      GPIO_Init(GPIOE , &GPIO_InitStruct);         

      /* SFSW_INB12 17*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INB13 4*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOC , &GPIO_InitStruct); 

      /* SFSW_INB14 9*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
      GPIO_Init(GPIOE , &GPIO_InitStruct);   

      /* SFSW_INB15 14*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /* SFSW_INB16 18*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
      GPIO_Init(GPIOB , &GPIO_InitStruct);   

      /* SFSW_INB17 5*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOB , &GPIO_InitStruct); 

      /* SFSW_INB18 10*/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
      GPIO_Init(GPIOE , &GPIO_InitStruct);       

      
      
      /** Safety switch feedback gpio -------------------------------------- **/ 
      GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;       
        
      /* SFSW_C_F_B1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);      
      
      /* SFSW_C_F_B2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);        

      /* SFSW_C_F_B3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);       

      /* SFSW_C_F_B4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);        

      /* SFSW_C_F_B5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct); 

      /** SINA_BUSY_AB -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 
      GPIO_Init(GPIOC , &GPIO_InitStruct);  

      /** SYNC_SYS_AB -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; 
      GPIO_Init(GPIOC , &GPIO_InitStruct);  
      
      
      /** Safety switch output gpio ---------------------------------------- **/
      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

      /* SFSW_C_B1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);      
      
      /* SFSW_C_B2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);        

      /* SFSW_C_B3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);       

      /* SFSW_C_B4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;                  
      GPIO_Init(GPIOB , &GPIO_InitStruct);        

      /* SFSW_C_B5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;                  
      GPIO_Init(GPIOE , &GPIO_InitStruct);       
      
      /** Relay output gpio ------------------------------------------------ **/ 
      /* RELAY_OC1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);      
      
      /* RELAY_OC2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);        

      /* RELAY_OC3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);       

      /* RELAY_OC4 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);        

      /* RELAY_OC5 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);

      /** Transistor output gpio ------------------------------------------- **/ 
      /* TRAN_OC1 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);      
      
      /* TRAN_OC2 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);        

      /* TRAN_OC3 */
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;                  
      GPIO_Init(GPIOD , &GPIO_InitStruct);     
      
      /** SINA_BUSY_BA -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1; 
      GPIO_Init(GPIOC , &GPIO_InitStruct);  

      /** SYNC_SYS_BA -------------------------------------------------- **/
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; 
      GPIO_Init(GPIOA , &GPIO_InitStruct);      
      
#endif      

}

/*******************************************************************************
* Function Name  : SW_DP_Init
* Description    : Initializes the SW DP switch gpio.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void SW_DP_Init(void)
{
      GPIO_InitTypeDef GPIO_InitStructure;
      
#ifdef GEC_DBL2_MASTER 
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11;				 
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
      GPIO_Init(GPIOA, &GPIO_InitStructure);              
      
#else	 
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
      GPIO_Init(GPIOC, &GPIO_InitStructure);       
			
#endif   
   
}


/******************************  END OF FILE  *********************************/


