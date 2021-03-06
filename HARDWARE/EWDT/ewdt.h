/*******************************************************************************
* File Name          : can.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : This file contains prototype of the external watchdog functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EWDT_H
#define __EWDT_H 

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef GEC_DBL2_MASTER
#define EWDT_TOOGLE() 	GPIOA->ODR ^= 0X0001u
#else
#define EWDT_TOOGLE() 	GPIOC->ODR ^= 0X0004u
#endif

/* Exported functions ------------------------------------------------------- */
void ExternalWatchdogCheck(void);
void EWDT_Drv_pin_config(void);
void write_bkp(u32 adr,u32 dat);


#endif  /* __EWDT_H */


/******************************  END OF FILE  *********************************/




