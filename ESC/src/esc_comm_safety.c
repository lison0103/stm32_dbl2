/*******************************************************************************
* File Name          : esc_comm_safety.c
* Author             : lison
* Version            : V1.0
* Date               : 06/20/2016
* Description        : This file contains esc safety board communication  with
*                      control board.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_comm_safety.h"
#include "can.h"
#include "hw_test.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : Communication_To_Safety
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Communication_To_Safety(void)
{
    static u8 can1_comm_timeout = 0;
    u8 len = 0;
    
    if( can1_receive == 1 )
    {
        can1_receive = 0;
        can1_comm_timeout = 0;
        EN_ERROR7 &= ~0x04;
    }
    else if( ++can1_comm_timeout >= 3 )
    {
        /*  can communication timeout process */
        EN_ERROR7 |= 0x04;
    }  
     
    
    /** receive a data packet -----------------------------------------------**/ 
    len = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
    
    /* just for test */
    if( len > 0 )
    {
        CAN1_TX_Data[0] = 0;
        CAN1_TX_Data[1] = 0;
//        CAN1_TX_Data[2] = 0;
//        CAN1_TX_Data[3] = 0;        
        CAN1_TX_Data[4] = CAN1_RX_Data[0];
        CAN1_TX_Data[5] = CAN1_RX_Data[1];
    }
    
    for( u8 i = 6; i < 50; i++ )
    {
        CAN1_TX_Data[i] = EscRTBuff[30+i] ;//| McRxBuff[30+i];
    }
    for( u8 i = 0; i < 10; i++ )
    {
        CAN1_TX_Data[50+i] = EscRTBuff[4+i] | McRxBuff[4+i];
    }   
    for( u8 i = 80; i < 124; i++ )
    {
        CAN1_TX_Data[i] =  McRxBuff[ i - 44 ];
    } 
    

    
    
    if( testmode == 0 )
    {
        /** CAN1 send data ------------------------------------------------------**/
        /** CB normal SEND ID:0x1314, CB URGE SEND ID:0x1234 **/
        BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1TX_NORMAL_ID, CAN1_TX_Data, 100);
    }
    else if( testmode == 1 )
    {
        BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);
    }
}




/******************************  END OF FILE  *********************************/