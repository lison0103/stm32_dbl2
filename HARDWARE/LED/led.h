/*******************************************************************************
* File Name          : led.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : The file contains prototype of led functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LED_H
#define _LED_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** LED Port **/
#ifdef GEC_DBL2_MASTER
#define LED_ON()   GPIO_ResetBits(GPIOA,GPIO_Pin_15)	
#define LED_OFF()   GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define LED_FLASH()   GPIOA->ODR ^= 0x8000u
#else
#define LED_ON()   GPIO_ResetBits(GPIOB,GPIO_Pin_12)	
#define LED_OFF()   GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define LED_FLASH()   GPIOB->ODR ^= 0x1000u
#endif
/* Exported functions ------------------------------------------------------- */
void LED_Init(void); 
 

#endif  /* _LED_H */


/******************************  END OF FILE  *********************************/


