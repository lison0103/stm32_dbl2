/*******************************************************************************
* File Name          : can.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : This file contains prototype of can functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H	 

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define canbuffsize 120

#define CAN_FRAME_LEN   8u
#define CAN_SEND_LEN    3u*CAN_FRAME_LEN
  
#define CAN1_TEST_ID  0xfaf1u

/* SF <---> DBL2 */
#define CAN1TX_DBL2_UPPER_ID1  0x40u
#define CAN1TX_DBL2_UPPER_ID2  0x41u
#define CAN1TX_DBL2_UPPER_ID3  0x42u
#define CAN1TX_DBL2_LOWER_ID1  0x64u
#define CAN1TX_DBL2_LOWER_ID2  0x65u
#define CAN1TX_DBL2_LOWER_ID3  0x66u
#define CAN1TX_DBL2_INTERM1_ID1  0x88u
#define CAN1TX_DBL2_INTERM1_ID2  0x89u
#define CAN1TX_DBL2_INTERM1_ID3  0x8Au
#define CAN1TX_DBL2_INTERM2_ID1  0xACu
#define CAN1TX_DBL2_INTERM2_ID2  0xADu
#define CAN1TX_DBL2_INTERM2_ID3  0xAEu

#define CAN1TX_DBL2_UPPER_NONSAFETY_ID  0xC1u
#define CAN1TX_DBL2_LOWER_NONSAFETY_ID  0xC2u
#define CAN1TX_DBL2_INTERM1_NONSAFETY_ID  0xC3u
#define CAN1TX_DBL2_INTERM2_NONSAFETY_ID  0xC4u

#define CAN1RX_SAFETY_DBL2_UPPER_ID1  0x20u
#define CAN1RX_SAFETY_DBL2_UPPER_ID2  0x21u
#define CAN1RX_SAFETY_DBL2_LOWER_ID1  0x24u
#define CAN1RX_SAFETY_DBL2_LOWER_ID2  0x25u
#define CAN1RX_SAFETY_DBL2_INTERM1_ID1  0x28u
#define CAN1RX_SAFETY_DBL2_INTERM1_ID2  0x29u
#define CAN1RX_SAFETY_DBL2_INTERM2_ID1  0x2Cu
#define CAN1RX_SAFETY_DBL2_INTERM2_ID2  0x2Du


/* Exported functions ------------------------------------------------------- */
u8 CAN_Int_Init(CAN_TypeDef* CANx); 
u8 Can_Send_Msg(CAN_TypeDef* CANx,u32 exid,u8 msg[],u8 len);			
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 buf[]);					
void Can_Receive_Data(void);
void Can_Clean_Buffer(void);
void Can_SetFilter(u16 CAN_Filter_ID1, u16 CAN_Filter_ID2);

extern u8 can1_receive;

#endif  /* __CAN_H */




/******************************  END OF FILE  *********************************/















