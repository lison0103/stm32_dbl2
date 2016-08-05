/*******************************************************************************
* File Name          : bsp_iocfg.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Contains prototype of input and output pin Initializes.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_IOCFG_H
#define __BSP_IOCFG_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//#define EXTERNAL_INPUT_TEST

/* Exported functions ------------------------------------------------------- */
void Input_Output_PinInit(void);


#ifdef GEC_DBL2_MASTER

#define SFSW_C_1_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_SET)
#define SFSW_C_1_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_RESET)
#define SFSW_C_2_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET)
#define SFSW_C_2_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET)
#define SFSW_C_3_SET()                 GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET)
#define SFSW_C_3_CLR()       GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET)
#define SFSW_C_4_SET()                 GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET)
#define SFSW_C_4_CLR()       GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET)
#define SFSW_C_5_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_SET)
#define SFSW_C_5_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_RESET)

#else


/* OUTPUT */
#define RELAY_OC1_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_SET)
#define RELAY_OC1_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_RESET)       
#define RELAY_OC2_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET)
#define RELAY_OC2_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET)
#define RELAY_OC3_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_SET)
#define RELAY_OC3_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_RESET)
#define RELAY_OC4_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_SET)
#define RELAY_OC4_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_RESET)
#define RELAY_OC5_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET)
#define RELAY_OC5_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET)

#define TRAN_OC1_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET)
#define TRAN_OC1_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_RESET)
#define TRAN_OC2_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET)
#define TRAN_OC2_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET)
#define TRAN_OC3_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET)
#define TRAN_OC3_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET)

#define SFSW_C_1_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_SET)
#define SFSW_C_1_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_3, Bit_RESET)
#define SFSW_C_2_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET)
#define SFSW_C_2_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET)
#define SFSW_C_3_SET()                 GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET)
#define SFSW_C_3_CLR()       GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET)
#define SFSW_C_4_SET()                 GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET)
#define SFSW_C_4_CLR()       GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET)
#define SFSW_C_5_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_SET)
#define SFSW_C_5_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_5, Bit_RESET)


#endif  /* GEC_DBL2_MASTER */


#endif /* __BSP_IOCFG_H */


/******************************  END OF FILE  *********************************/



