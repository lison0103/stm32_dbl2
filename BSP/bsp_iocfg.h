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
/* Exported functions ------------------------------------------------------- */
void Input_Output_PinInit(void);


#ifdef GEC_DBL2_MASTER

/** Safety switch output gpio ---------------------------------------- **/
#define SFSW_C_1_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_SET)
#define SFSW_C_1_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_4, Bit_RESET)
#define SFSW_C_2_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_SET)
#define SFSW_C_2_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_RESET)
#define SFSW_C_3_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_SET)
#define SFSW_C_3_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_0, Bit_RESET)
#define SFSW_C_4_SET()                 GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET)
#define SFSW_C_4_CLR()       GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET)
#define SFSW_C_5_SET()                 GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_SET)
#define SFSW_C_5_CLR()       GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_RESET)

/** SYNC/BUSY output gpio -------------------------------------------------- **/
#define SINA_BUSY_OUT_SET()       GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_SET)
#define SINA_BUSY_OUT_CLR()       GPIO_WriteBit(GPIOD, GPIO_Pin_11, Bit_RESET)
#define SYNC_SYS_OUT_SET()       GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET)
#define SYNC_SYS_OUT_CLR()       GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET)

/** SAFETY switch input gpio ----------------------------------------- **/
#define SFSW_IN1        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)
#define SFSW_IN2        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)
#define SFSW_IN3        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)
#define SFSW_IN4        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)
#define SFSW_IN5        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_17)
#define SFSW_IN6        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SFSW_IN7        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define SFSW_IN8        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define SFSW_IN9        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)
#define SFSW_IN10        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)
#define SFSW_IN11        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8)
#define SFSW_IN12        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)
#define SFSW_IN13        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10)
#define SFSW_IN14        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)
#define SFSW_IN15        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12)
#define SFSW_IN16        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13)
#define SFSW_IN17        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14)
#define SFSW_IN18        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15)

/** Safety switch feedback gpio -------------------------------------- **/
#define SFSW_C_F1       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define SFSW_C_F2       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)
#define SFSW_C_F3       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define SFSW_C_F4       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define SFSW_C_F5       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)

/** SYNC/BUSY input gpio -------------------------------------------------- **/
#define SINA_BUSY_IN       GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)
#define SYNC_SYS_IN       GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)

#else


/** Relay output gpio ----------------------------------------------**/
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

/** Transistor output gpio ----------------------------------------------**/
#define TRAN_OC1_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET)
#define TRAN_OC1_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_RESET)
#define TRAN_OC2_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET)
#define TRAN_OC2_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET)
#define TRAN_OC3_ON()    		GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET)
#define TRAN_OC3_OFF()    	GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET)

/** Safety switch output gpio ---------------------------------------- **/
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

/** SYNC/BUSY output gpio -------------------------------------------------- **/
#define SINA_BUSY_OUT_SET()       GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET)
#define SINA_BUSY_OUT_CLR()       GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET)
#define SYNC_SYS_OUT_SET()       GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET)
#define SYNC_SYS_OUT_CLR()       GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET)


/** SAFETY switch input gpio ----------------------------------------- **/
#define SFSW_IN1        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)
#define SFSW_IN2        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)
#define SFSW_IN3        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define SFSW_IN4        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SFSW_IN5        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define SFSW_IN6        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)
#define SFSW_IN7        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)
#define SFSW_IN8        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)
#define SFSW_IN9        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8)
#define SFSW_IN10        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)
#define SFSW_IN11        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10)
#define SFSW_IN12        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)
#define SFSW_IN13        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12)
#define SFSW_IN14        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13)
#define SFSW_IN15        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14)
#define SFSW_IN16        GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15)
#define SFSW_IN17        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)
#define SFSW_IN18        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)

/** Safety switch feedback gpio -------------------------------------- **/
#define SFSW_C_F1       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define SFSW_C_F2       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)
#define SFSW_C_F3       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define SFSW_C_F4       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define SFSW_C_F5       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)

/** SYNC/BUSY input gpio -------------------------------------------------- **/
#define SINA_BUSY_IN       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
#define SYNC_SYS_IN       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)

#endif  /* GEC_DBL2_MASTER */


#endif /* __BSP_IOCFG_H */


/******************************  END OF FILE  *********************************/



