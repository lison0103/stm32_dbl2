/*******************************************************************************
* File Name          : bsp_input.h
* Author             : lison
* Version            : V1.0
* Date               : 06/16/2016
* Description        : Contains prototype of get the input pin value function.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_INPUT_H
#define __BSP_INPUT_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Get_GpioInput(u8 inBuff[]);
u8 ReadSwDp(void);
#ifdef GEC_DBL2_SLAVE
void output_driver(u8 out_buff[]);
#endif

#endif /* __BSP_INPUT_H */


/******************************  END OF FILE  *********************************/



