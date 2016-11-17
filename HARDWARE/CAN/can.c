/*******************************************************************************
* File Name          : can.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : This file contains can functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "led.h"
#include "delay.h"
#include "crc16.h"
#include "esc.h"
#include "bsp_input.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* #define CAN_BAUDRATE  1000 */     /* 1MBps   */
/* #define CAN_BAUDRATE  500 */ /* 500kBps */
#define CAN_BAUDRATE  250  /* 250kBps */
/* #define CAN_BAUDRATE  125 */ /* 125kBps */
/* #define CAN_BAUDRATE  100 */ /* 100kBps */ 
/* #define CAN_BAUDRATE  50 */  /* 50kBps  */ 
/* #define CAN_BAUDRATE  20 */ /* 20kBps  */ 
/* #define CAN_BAUDRATE  10 */  /* 10kBps  */

/* Private macro -------------------------------------------------------------*/
#define CAN1_RX0_INT_ENABLE	1		
#ifdef GEC_DBL2_MASTER
#define CAN2_RX0_INT_ENABLE	1		
#endif

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
static void Can_Receive_Buffer(u8 rxmsg[], u16 canid);


u8 can1_receive = 0u;

u16 g_u16CANSendFail = 0u;
u16 g_u16CANSendSuccess = 0u;

/* Esc receive data buffer */
u8 EscDataFromSafetyBuffer[2][8];

/*******************************************************************************
* Function Name  : CAN_Int_Init
* Description    : Initialization can.
* CAN_SJW: CAN_SJW_1tq~ CAN_SJW_4tq
* CAN_BS2: CAN_BS2_1tq~CAN_BS2_8tq;
* CAN_BS1: CAN_BS1_1tq ~CAN_BS1_16tq
* CAN_Prescaler: 1~1024;  tq=(brp)*tpclk1
* baud rate = Fpclk1/((tbs1+1+tbs2+1+1)*brp)
* if Fpclk is 36M, baud rate:36M/((1+3+2)*24)=250Kbps               
* Input          : CANx: CAN1 or CAN2
*                  None
* Output         : None
* Return         : None
*******************************************************************************/

u8 CAN_Int_Init(CAN_TypeDef* CANx)
{ 
    GPIO_InitTypeDef 		GPIO_InitStructure; 
    CAN_InitTypeDef        	CAN_InitStructure;
    CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE || CAN2_RX0_INT_ENABLE
    NVIC_InitTypeDef  		NVIC_InitStructure;
#endif       
    
    if(CANx == CAN1)
    {

        /* Connect CAN pins to AF9 */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_9);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_9); 
        
        /* Configure CAN RX and TX pins */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
                   
        
        CAN_DeInit(CANx);
        CAN_StructInit(&CAN_InitStructure);
        
        /*  non-time-triggered communication mode */
        CAN_InitStructure.CAN_TTCM=DISABLE;			
        /* automatic offline management software */
        CAN_InitStructure.CAN_ABOM=DISABLE;				 
        /* wake-sleep mode via software (Clear CAN-> MCR's SLEEP bit) */
        CAN_InitStructure.CAN_AWUM=DISABLE;			
        /* message is automatically transferred, in accordance with the CAN standard, */
        /* CAN hardware failure when sending packets would have been automatic retransmission until sent successfully */
        CAN_InitStructure.CAN_NART=DISABLE;	
        /* message is not locked, the new over the old one */
        CAN_InitStructure.CAN_RFLM=DISABLE;		 	
        /* priority is determined by the packet identifier */
        CAN_InitStructure.CAN_TXFP=DISABLE;			
        CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;	 
        
        /* set baud rate */
        CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
        CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
        CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;   
        
#if CAN_BAUDRATE == 1000 /* 1MBps */
            CAN_InitStructure.CAN_Prescaler = 6u;
#elif CAN_BAUDRATE == 500 /* 500KBps */
            CAN_InitStructure.CAN_Prescaler = 12u;
#elif CAN_BAUDRATE == 250 /* 250KBps */
            CAN_InitStructure.CAN_Prescaler = 24u;
#elif CAN_BAUDRATE == 125 /* 125KBps */
            CAN_InitStructure.CAN_Prescaler = 48u;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
            CAN_InitStructure.CAN_Prescaler = 60u;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
            CAN_InitStructure.CAN_Prescaler = 120u;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
            CAN_InitStructure.CAN_Prescaler = 300u;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
            CAN_InitStructure.CAN_Prescaler = 600u;
#else
#error "Please select first the CAN Baudrate in Private defines "
#endif  /* CAN_BAUDRATE == 1000 */            
        
        /* Initializes the CAN1 */
        CAN_Init(CANx, &CAN_InitStructure);        	
        
        /* CAN1 filter init */
        /* 16 bit mask: STDID[10:0], IDE, RTR, EXTDID[17:15] */
        /* 32 bit mask: STDID[10:0], EXTDID[17:0], IDE, RTR,0 */
        CAN_FilterInitStructure.CAN_FilterNumber=0u;	
        CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	
        CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	
        

        CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000u;	
        CAN_FilterInitStructure.CAN_FilterIdLow=0x0000u;
        CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000u;
        CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000u;
        
        CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
        CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
        CAN_FilterInit(&CAN_FilterInitStructure);	
        
        
#if CAN1_RX0_INT_ENABLE 
        /* IT Configuration for CAN1 */ 
        /* FIFO 0 message pending Interrupt ,full Interrupt , overrun Interrupt */
        CAN_ITConfig(CAN1,CAN_IT_FMP0 , ENABLE); 		    
        CAN_ITConfig(CAN1,CAN_IT_FF0, ENABLE); 		    
        CAN_ITConfig(CAN1,CAN_IT_FOV0, ENABLE); 		    
        NVIC_InitStructure.NVIC_IRQChannel = (u8)CAN1_RX0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1u;     
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0u;            
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
#endif             
        CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                
        /* Enable CAN1 TX0 interrupt IRQ channel */
        NVIC_InitStructure.NVIC_IRQChannel = (u8)CAN1_TX_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1u;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1u;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);  
        
    }
    
    return 0u;
}   


/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : This function handles CAN1 RX0 interrupt request.                
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if CAN1_RX0_INT_ENABLE	
			    
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    
    if( CAN_GetITStatus(CAN1,CAN_IT_FF0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
    }
    else if( CAN_GetITStatus(CAN1,CAN_IT_FOV0) != RESET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
    }
    else
    {
        
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
        /** SF data RECEIVE **/
        /* DBL2 UPPER */
        if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
        {   
            if(( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_UPPER_ID1 ) || ( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_UPPER_ID2 ))
            {
                if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
                {
                    can1_receive = 1u;            
                    Can_Receive_Buffer(RxMessage.Data, (u16)RxMessage.ExtId);
                }
            }  
        }
        /* DBL2 LOWER */
        else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
        {   
            if(( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_LOWER_ID1 ) || ( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_LOWER_ID2 ))
            {
                if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
                {
                    can1_receive = 1u;            
                    Can_Receive_Buffer(RxMessage.Data, (u16)RxMessage.ExtId);
                }
            }  
        }     
        /* DBL2 INTERM1 */
        else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
        {   
            if(( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_INTERM1_ID1 ) || ( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_INTERM1_ID2 ))
            {
                if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
                {
                    can1_receive = 1u;            
                    Can_Receive_Buffer(RxMessage.Data, (u16)RxMessage.ExtId);
                }
            }  
        } 
        /* DBL2 INTERM2 */
        else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
        {   
            if(( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_INTERM2_ID1 ) || ( RxMessage.ExtId == CAN1RX_SAFETY_DBL2_INTERM2_ID2 ))
            {
                if( ( RxMessage.DLC == CAN_FRAME_LEN ) && ( RxMessage.IDE == CAN_ID_EXT ))
                {
                    can1_receive = 1u;            
                    Can_Receive_Buffer(RxMessage.Data, (u16)RxMessage.ExtId);
                }
            }  
        }         
        /* Test Mode */        
        else if( ( RxMessage.ExtId == CAN1_TEST_ID ) && ( RxMessage.IDE == CAN_ID_EXT ) )
        {
            can1_receive = 1u;        
            
            Can_Receive_Buffer(RxMessage.Data, (u16)RxMessage.ExtId);
        }  
        else
        {}
        
        CAN_FIFORelease(CAN1,CAN_FIFO0);
    }
}
#endif


/*******************************************************************************
* Function Name  : CAN1_TX_IRQHandler
* Description    : This function handles CAN1 TX interrupt request.               
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void CAN1_TX_IRQHandler(void)
{
    u8 result;
    
    CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP0);
    CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);
       
    /* DBL2 UP */
    if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
    {            
        if( g_u16CANSendFail & 0x01u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_UPPER_ID1, &EscDataToSafety[0][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x01u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendSuccess |= 0x01u;
                g_u16CANSendFail &= ~0x01u;
            }
        }        
        if( g_u16CANSendFail & 0x02u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_UPPER_ID2, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x02u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendSuccess |= 0x02u;
                g_u16CANSendFail &= ~0x02u;
            }
        } 
        if( g_u16CANSendFail & 0x04u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_UPPER_ID2, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendSuccess |= 0x04u;
                g_u16CANSendFail &= ~0x04u;
            }
        }  
        if( g_u16CANSendFail & 0x08u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_UPPER_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
            else
            {
                g_u16CANSendFail &= ~0x08u;
            }
        }   
                
        if( g_u16CANSendSuccess == 0x07u )
        {
            g_u16CANSendSuccess = 0u;
            EscData.HeaderCode[1]++;
        }
    }
    /* DBL2 Lower */
    else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
    {          
        if( g_u16CANSendFail & 0x01u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_LOWER_ID1, &EscDataToSafety[0][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x01u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendSuccess |= 0x01u;
                g_u16CANSendFail &= ~0x01u;
            }
        }        
        if( g_u16CANSendFail & 0x02u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_LOWER_ID2, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x02u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendSuccess |= 0x02u;
                g_u16CANSendFail &= ~0x02u;
            }
        } 
        if( g_u16CANSendFail & 0x04u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_LOWER_ID3, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendSuccess |= 0x04u;
                g_u16CANSendFail &= ~0x04u;
            }
        }  
        if( g_u16CANSendFail & 0x08u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_LOWER_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
            else
            {
                g_u16CANSendFail &= ~0x08u;
            }
        } 
        
        if( g_u16CANSendSuccess == 0x07u )
        {
            g_u16CANSendSuccess = 0u;
            EscData.HeaderCode[1]++;
        }        
    }
    /* DBL2 INTERM1 */
    else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
    {          
        if( g_u16CANSendFail & 0x01u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_ID1, &EscDataToSafety[0][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x01u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendFail &= ~0x01u;
            }
        }        
        if( g_u16CANSendFail & 0x02u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_ID2, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x02u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendFail &= ~0x02u;
            }
        } 
        if( g_u16CANSendFail & 0x04u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_ID3, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendFail &= ~0x04u;
            }
        }  
        if( g_u16CANSendFail & 0x08u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
            else
            {
                g_u16CANSendFail &= ~0x08u;
            }
        }   
        
        if( g_u16CANSendSuccess == 0x07u )
        {
            g_u16CANSendSuccess = 0u;
            EscData.HeaderCode[1]++;
        }          
        
    } 
    /* DBL2 INTERM2 */
    else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
    {          
        if( g_u16CANSendFail & 0x01u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_ID1, &EscDataToSafety[0][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x01u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendFail &= ~0x01u;
            }
        }        
        if( g_u16CANSendFail & 0x02u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_ID2, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x02u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendFail &= ~0x02u;
            }
        } 
        if( g_u16CANSendFail & 0x04u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_ID3, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);  
            } 
            else
            {
                g_u16CANSendFail &= ~0x04u;
            }
        } 
        if( g_u16CANSendFail & 0x08u )
        {
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
            else
            {
                g_u16CANSendFail &= ~0x08u;
            }
        }
        
        if( g_u16CANSendSuccess == 0x07u )
        {
            g_u16CANSendSuccess = 0u;
            EscData.HeaderCode[1]++;
        }          
    }         
    else
    {}
}


/*******************************************************************************
* Function Name  : Can_Send_Msg
* Description    : CAN send data
*                  
* Input          : len: data len(max len is 8)
*                  msg: Data Pointer.
*                  exid: Extended identifier ID.
*                  CANx: CAN1 or CAN2
* Output         : None
* Return         : 0: success
*                  1: fail, no send mailbox 
*******************************************************************************/		 
u8 Can_Send_Msg(CAN_TypeDef* CANx,u32 exid,u8 msg[],u8 len)
{	
	u16 i = 0u;
        u8 result = 0u;        
	CanTxMsg TxMessage;
        
	TxMessage.StdId = 0x12u;			
	TxMessage.ExtId = exid;			
/*	TxMessage.IDE = CAN_Id_Standard; */
        TxMessage.IDE = CAN_Id_Extended; 	        
	TxMessage.RTR = CAN_RTR_Data;		
	TxMessage.DLC = len;			
	for(i = 0u; i < len; i++)
        {
          TxMessage.Data[i] = msg[i];
        }
	if( CAN_TxStatus_NoMailBox == CAN_Transmit(CANx, &TxMessage) )
        {
            result =  1u;
        }
        else
        {
            result =  0u;
        }
        
        return result;
}


/*******************************************************************************
* Function Name  : Can_Receive_Msg
* Description    : CAN receive data                 
* Input          : buf:data cache
* Output         : None
* Return         : 0: no data receive; 
*                  other: Length of the received data;
*******************************************************************************/
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 buf[])
{		   		   
      u32 i;
      u8 result = 0u; 
      CanRxMsg RxMessage;
      
      if( CAN_MessagePending(CANx,CAN_FIFO0)== 0u )
      {
          result =  0u;		 
      }  
      else
      {          
          CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
          
          for(i = 0u; i < RxMessage.DLC; i++)
          {
              buf[i] = RxMessage.Data[i];  
          }
          
          result = RxMessage.DLC;
      }
      return result;	
}

/*******************************************************************************
* Function Name  : Can_Clean_Buffer
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
void Can_Clean_Buffer(void)
{	  
    u8 i;
    
    for( i = 0u; i < 8u; i++ )
    {
        EscDataFromSafetyBuffer[0][i] = 0u;
        EscDataFromSafetyBuffer[1][i] = 0u;
    }
}

/*******************************************************************************
* Function Name  : Can_SetFilter
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
void Can_SetFilter(u16 CAN_Filter_ID1, u16 CAN_Filter_ID2)
{	
    CAN_FilterInitTypeDef  	        CAN_FilterInitStructure;
    
    /* CAN1 filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0u;	
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
    
    CAN_FilterInitStructure.CAN_FilterIdHigh = (u16)((((u32)CAN_Filter_ID1 << 3u) & 0xFFFF0000u ) >> 16u);	
    CAN_FilterInitStructure.CAN_FilterIdLow = (u16)((((u32)CAN_Filter_ID1 << 3u) | CAN_ID_EXT | CAN_RTR_DATA ) & 0xFFFFu);
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (u16)(((((u32)(~( CAN_Filter_ID1 ^ CAN_Filter_ID2 ))) << 3u) & 0xFFFF0000u) >> 16u);
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (u16)(((((u32)(~( CAN_Filter_ID1 ^ CAN_Filter_ID2 ))) << 3u) | CAN_ID_EXT | CAN_RTR_DATA ) & 0xFFFFu);   
    
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

/*******************************************************************************
* Function Name  : Can_Receive_Buffer
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Receive_Buffer(u8 rxmsg[], u16 canid)
{	  
    u8 i;
    
    
    /* DBL2 UPPER */
    if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
    {   
        if( canid == CAN1RX_SAFETY_DBL2_UPPER_ID1 )
        {
            g_u8SafetyNewData |= 0x01u;
        }
        else if( canid == CAN1RX_SAFETY_DBL2_UPPER_ID2 )
        {
            g_u8SafetyNewData |= 0x10u;
        }
        else
        {}
        
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafetyBuffer[canid - CAN1RX_SAFETY_DBL2_UPPER_ID1][i] = rxmsg[i];
        }
    }
    /* DBL2 LOWER */
    else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
    {
        if( canid == CAN1RX_SAFETY_DBL2_LOWER_ID1 )
        {
            g_u8SafetyNewData |= 0x01u;
        }
        else if( canid == CAN1RX_SAFETY_DBL2_LOWER_ID2 )
        {
            g_u8SafetyNewData |= 0x10u;
        }
        else
        {}        
        
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafetyBuffer[canid - CAN1RX_SAFETY_DBL2_LOWER_ID1][i] = rxmsg[i];
        } 
    }     
    /* DBL2 INTERM1 */
    else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
    { 
        if( canid == CAN1RX_SAFETY_DBL2_INTERM1_ID1 )
        {
            g_u8SafetyNewData |= 0x01u;
        }
        else if( canid == CAN1RX_SAFETY_DBL2_INTERM1_ID2 )
        {
            g_u8SafetyNewData |= 0x10u;
        }
        else
        {}        
        
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafetyBuffer[canid - CAN1RX_SAFETY_DBL2_INTERM1_ID1][i] = rxmsg[i];
        } 
    } 
    /* DBL2 INTERM2 */
    else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
    {       
        if( canid == CAN1RX_SAFETY_DBL2_INTERM2_ID1 )
        {
            g_u8SafetyNewData |= 0x01u;
        }
        else if( canid == CAN1RX_SAFETY_DBL2_INTERM2_ID2 )
        {
            g_u8SafetyNewData |= 0x10u;
        }
        else
        {}         
        
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafetyBuffer[canid - CAN1RX_SAFETY_DBL2_INTERM2_ID1][i] = rxmsg[i];
        }
    }
    else
    {}
}

/*******************************************************************************
* Function Name  : Can_Receive_Data
* Description    :                 
* Input          : None 
* Output         : None
* Return         : None 
*******************************************************************************/
void Can_Receive_Data(void)
{	  
    u8 i,j;    
    
    for( j = 0u; j < 2u; j++ )
    {
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafety[j][i] = EscDataFromSafetyBuffer[j][i];
        }  
    }
}

/******************************  END OF FILE  *********************************/














