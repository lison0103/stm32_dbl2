/*******************************************************************************
* File Name          : esc_comm_safety.c
* Author             : lison
* Version            : V1.0
* Date               : 06/20/2016
* Last modify date   : 10/28/2016
* Description        : This file contains esc safety board communication  with
*                      control board.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_comm_safety.h"
#include "can.h"
#include "hw_test.h"
#include "esc.h"
#include "crc16.h"
#include "initial_devices.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CONNECTION_DBL2_UPPER   0x00u  
#define CONNECTION_DBL2_LOWER   0x01u
#define CONNECTION_DBL2_INTERM1   0x02u    
#define CONNECTION_DBL2_INTERM2   0x03u
#define CAN_COMM_HAND_TIME      5000u

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Can_Send_Data_Process(void);
static void Can_Receive_Data_Process(void);
static void Communication_Can_Filter(void);

u8 g_u8SafetyNewData = 0u;
/* for test */
u8 g_u8SafetyRequest = 0u;

/*******************************************************************************
* Function Name  : Communication_To_Safety
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
static void Communication_Can_Filter(void)
{
    static u8 stat_u8Flag = 0u;
    
    if( stat_u8Flag == 0u )
    {
        /* DBL2 UP */
        if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
        {         
            stat_u8Flag = 1u;
            Can_SetFilter(CAN1RX_SAFETY_DBL2_UPPER_ID1, CAN1RX_SAFETY_DBL2_UPPER_ID2);
        }
        /* DBL2 DOWN */
        else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
        {          
            stat_u8Flag = 1u;
            Can_SetFilter(CAN1RX_SAFETY_DBL2_LOWER_ID1, CAN1RX_SAFETY_DBL2_LOWER_ID2);
        }
        /* DBL2 INTERM1 */
        else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
        {          
            stat_u8Flag = 1u;
            Can_SetFilter(CAN1RX_SAFETY_DBL2_INTERM1_ID1, CAN1RX_SAFETY_DBL2_INTERM1_ID2);
        } 
        /* DBL2 INTERM2 */
        else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
        {          
            stat_u8Flag = 1u;
            Can_SetFilter(CAN1RX_SAFETY_DBL2_INTERM2_ID1, CAN1RX_SAFETY_DBL2_INTERM2_ID2);
        }         
        else
        {}   
    }
}

/*******************************************************************************
* Function Name  : Safety_Send_Data_Process
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
void Safety_Send_Data_Process(u8 respone)
{	
    u32 crc;
    u16 len;
    u8 i;

    
    if( respone )
    {
#ifdef GEC_DBL2_SLAVE
        /* DBL2 UP */
        if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
        {         
            /* CONNECTION_A_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_UPPER;  
            /* CONNECTION_A_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_UPPER;
        }
        /* DBL2 DOWN */
        else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
        {          
            /* CONNECTION_A_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_LOWER;  
            /* CONNECTION_A_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_LOWER; 
        }
        /* DBL2 INTERM1 */
        else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
        {          
            /* CONNECTION_A_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_INTERM1;  
            /* CONNECTION_A_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_INTERM1;
        } 
        /* DBL2 INTERM2 */
        else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
        {          
            /* CONNECTION_A_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_INTERM2;  
            /* CONNECTION_A_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_INTERM2;
        }         
        else
        {}        

        /* FAULT_STATUS_A_1 */
        EscData.SafetySendData[0] |= (u8)((EscData.DBL2FaultStatus << 2u) & 0xfcu);
        /* SAFETY_SENSOR_INPUTS_A_1 (8inputs =8x1 bit ) */
        EscData.SafetySendData[1] = EscData.DBL2InputData[0];
        /* SAFETY_SENSOR_INPUTS_A_1 (6inputs =6x1 bit ) */
        /* SAFETY_SWITCH_INPUTS_A_1 (2inputs =2x1 bit ) */
        EscData.SafetySendData[2] = EscData.DBL2InputData[1];  
        /* SEQN_A_1 */
        EscData.SafetySendData[3] = EscData.SEQN;

        /* FAULT_STATUS_A_2 */
        EscData.SafetySendData[4] |= (u8)((EscData.DBL2FaultStatus << 2u) & 0xfcu);
        /* SAFETY_SENSOR_INPUTS_A_2 (8inputs =8x1 bit ) */
        EscData.SafetySendData[5] = EscData.DBL2InputData[0];
        /* SAFETY_SENSOR_INPUTS_A_2 (6inputs =6x1 bit ) */
        /* SAFETY_SWITCH_INPUTS_A _2 (2inputs =2x1 bit ) */
        EscData.SafetySendData[6] = EscData.DBL2InputData[1];  
        /* SEQN_A_2 */
        EscData.SafetySendData[7] = EscData.SEQN;        
          
        /* CRC_A */
        len = 12u;
        crc = MB_CRC32( &EscData.SafetySendData[0], len - 4u, SAFETY_POLYNOMIALS );
        EscData.SafetySendData[len - 4u] = (u8)(crc >> 24u);
        EscData.SafetySendData[len - 3u] = (u8)(crc >> 16u);     
        EscData.SafetySendData[len - 2u] = (u8)(crc >> 8u);
        EscData.SafetySendData[len - 1u] = (u8)crc;        
#else
        /* DBL2 UP */
        if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
        {         
            /* CONNECTION_B_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_UPPER;  
            /* CONNECTION_B_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_UPPER;
        }
        /* DBL2 DOWN */
        else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
        {          
            /* CONNECTION_B_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_LOWER;  
            /* CONNECTION_B_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_LOWER; 
        }
        /* DBL2 INTERM1 */
        else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
        {          
            /* CONNECTION_B_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_INTERM1;  
            /* CONNECTION_B_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_INTERM1;
        } 
        /* DBL2 INTERM2 */
        else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
        {          
            /* CONNECTION_B_1 */
            EscData.SafetySendData[0] |= CONNECTION_DBL2_INTERM2;  
            /* CONNECTION_B_2 */
            EscData.SafetySendData[4] |= CONNECTION_DBL2_INTERM2;
        }         
        else
        {}  
        

        /* FAULT_STATUS_B_1 */
        EscData.SafetySendData[0] |= (u8)((EscData.DBL2FaultStatus << 2u) & 0xfcu);
        /* SAFETY_SENSOR_INPUTS_B_1 (8inputs =8x1 bit ) */
        EscData.SafetySendData[1] = EscData.DBL2InputData[0];
        /* SAFETY_SENSOR_INPUTS_B_1 (6inputs =6x1 bit ) */
        /* SAFETY_SWITCH_INPUTS_B_1 (2inputs =2x1 bit ) */
        EscData.SafetySendData[2] = EscData.DBL2InputData[1];  
        /* SEQN_B_1 */
        EscData.SafetySendData[3] = EscData.SEQN;

        /* FAULT_STATUS_B_2 */
        EscData.SafetySendData[4] |= (u8)((EscData.DBL2FaultStatus << 2u) & 0xfcu);
        /* SAFETY_SENSOR_INPUTS_B_2 (8inputs =8x1 bit ) */
        EscData.SafetySendData[5] = EscData.DBL2InputData[0];
        /* SAFETY_SENSOR_INPUTS_B_2 (6inputs =6x1 bit ) */
        /* SAFETY_SWITCH_INPUTS_B _2 (2inputs =2x1 bit ) */
        EscData.SafetySendData[6] = EscData.DBL2InputData[1];  
        /* SEQN_B_2 */
        EscData.SafetySendData[7] = EscData.SEQN;        
          
        /* CRC_B */
        len = 12u;
        crc = MB_CRC32( &EscData.SafetySendData[0], len - 4u, SAFETY_POLYNOMIALS );
        EscData.SafetySendData[len - 4u] = (u8)(crc >> 24u);
        EscData.SafetySendData[len - 3u] = (u8)(crc >> 16u);     
        EscData.SafetySendData[len - 2u] = (u8)(crc >> 8u);
        EscData.SafetySendData[len - 1u] = (u8)crc;
#endif       
    }
    else
    {
        for( i = 0u; i < 12u; i++ )
        {
            EscData.SafetySendData[i] = 0u;
        }
#ifndef GEC_DBL2_SLAVE        
        EscData.SafetyValidateResult = 0x00u;
#endif        
    }    
}

/*******************************************************************************
* Function Name  : Safety_Receive_Data_Process
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
void Safety_Receive_Data_Process(void)
{	    
    u8 i;
    u8 u8_ValidateError = 0u;

#ifdef GEC_DBL2_SLAVE
        if(( EscData.SafetyValidateResult == 0x01u ) && ( OmcEscData.SafetyValidateResult == 0x01u ))
        {
            /* data ok */
            g_u8SafetyRequest = 1u;
                       
            EscData.SafetyReset = EscData.SafetyReceiveDataA[0] & 0x0cu;
            EscData.SEQN = EscData.SafetyReceiveDataA[1];
            EscData.DBL2OutputData = EscData.SafetyReceiveDataA[3];
            
            EscData.SafetyValidateResult = 0x00u;  
            OmcEscData.SafetyValidateResult = 0x00u;
            
            /* clear receive data */
            for( i = 0u; i < 8u; i++ )
            {
                EscData.SafetyReceiveDataA[i] = 0u;
            }
        }
        else
        {
        
        }
#else       
        /* Validation Request: Check CONNECTION, CRC and compare _1 and _2 data */       
        /* check CRC */
        if( !MB_CRC32(&OmcEscData.SafetyReceiveDataB[0], 8u, SAFETY_POLYNOMIALS ))
        { 
            /* compare CONNECTION */
            if((OmcEscData.SafetyReceiveDataB[0] & 0x03u) != ((OmcEscData.SafetyReceiveDataB[0] >> 4u) & 0x03u))
            {
                u8_ValidateError = 1u;
            }
            /* compare RESET */
            else if((OmcEscData.SafetyReceiveDataB[0] & 0x0cu) != ((OmcEscData.SafetyReceiveDataB[0] >> 4u) & 0x0cu))
            {
                u8_ValidateError = 1u;
            }            
            /* compare SEQN */
            else if( OmcEscData.SafetyReceiveDataB[1] != OmcEscData.SafetyReceiveDataB[2] )
            {
                u8_ValidateError = 1u;
            }
            else
            {   
                /* check CONNECTION */
                /* DBL2 UPPER */
                if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
                {                      
                    if( (OmcEscData.SafetyReceiveDataB[0] & 0x03u) != CONNECTION_DBL2_UPPER )
                    {
                        u8_ValidateError = 1u;
                    }
                }
                /* DBL2 DOWN */
                else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
                {     
                    if( (OmcEscData.SafetyReceiveDataB[0] & 0x03u) != CONNECTION_DBL2_LOWER )
                    {
                        u8_ValidateError = 1u;
                    }
                }
                /* DBL2 INTERM1 */
                else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
                {     
                    if( (OmcEscData.SafetyReceiveDataB[0] & 0x03u) != CONNECTION_DBL2_INTERM1 )
                    {                  
                        u8_ValidateError = 1u;
                    }
                } 
                /* DBL2 INTERM2 */
                else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
                {          
                    if( (OmcEscData.SafetyReceiveDataB[0] & 0x03u) != CONNECTION_DBL2_INTERM2 )
                    {
                        u8_ValidateError = 1u;
                    }
                }         
                else
                {
                    u8_ValidateError = 1u;
                }                  
                
                if( u8_ValidateError == 0u )
                {  
                    /* validate data */
                    EscData.SafetyValidateResult = 0x01u;   
                    
                    if( OmcEscData.SafetyValidateResult == 0x01u )
                    {
                        EscData.SafetyReset = OmcEscData.SafetyReceiveDataB[0] & 0x0cu;
                        EscData.SEQN = OmcEscData.SafetyReceiveDataB[1];
                        
                        g_u8SafetyRequest = 1u;
                        Safety_Send_Data_Process(1u);
                    }
                }
            }
        }   
        
        /* clear data */
        for( i = 0u; i < 8u; i++ )
        {
            OmcEscData.SafetyReceiveDataB[i] = 0u;
        }  
                     
#endif   
}


/*******************************************************************************
* Function Name  : Can_Send_Data_Process
* Description    :                  
*
    MESSAGE 1
    Byte	Bits (7 is MSB)	Data
    0	0-1	CONNECTION_A_1
    0	2-8	FAULT_STATUS_A_1
    1	0-7	SAFETY_SENSOR_INPUTS_A_1 (8inputs =8x1 bit )
    2	0-5	SAFETY_SENSOR_INPUTS_A_1 (6inputs =6x1 bit )
    2	6-7	SAFETY_SWITCH_INPUTS_A_1 (2inputs =2x1 bit )
    3	0-7	SEQN_A_1
    4	0-1	CONNECTION_A_2
    4	2-8	FAULT_STATUS_A_2
    5	0-7	SAFETY_SENSOR_INPUTS_A_2 (8inputs =8x1 bit )
    6	0-5	SAFETY_SENSOR_INPUTS_A_2 (6inputs =6x1 bit )
    6	6-7	SAFETY_SWITCH_INPUTS_A _2 (2inputs =2x1 bit )
    7	0-7	SEQN_A_2

    MESSAGE 2
    Byte	Bits (7 is MSB)	Data
    0	0-1	CONNECTION_B_1
    0	2-8	FAULT_STATUS_B_1
    1	0-7	SAFETY_SENSOR_INPUTS_B_1 (8inputs =8x1 bit )
    2	0-5	SAFETY_SENSOR_INPUTS_B_1 (6inputs =6x1 bit )
    2	6-7	SAFETY_SWITCH_INPUTS_B_1 (2inputs =2x1 bit )
    3	0-7	SEQN_B_1
    4	0-1	CONNECTION_B_2
    4	2-8	FAULT_STATUS_B_2
    5	0-7	SAFETY_SENSOR_INPUTS_B_2 (8inputs =8x1 bit )
    6	0-5	SAFETY_SENSOR_INPUTS_B_2 (6inputs =6x1 bit )
    6	6-7	SAFETY_SWITCH_INPUTS_B_2 (2inputs =2x1 bit )
    7	0-7	SEQN_B_2
        
    MESSAGE 3
    Byte	Bits (7 is MSB)	Data
    0,1,2,3	0-31	CRC_A
    4,5,6,7	0-31	CRC_B

    CRC_A: CONNECTION_A , FAULT_STATUS_A, SAFETY_SENSOR_INPUTS_A, 
    SAFETY_SWITCH_INPUTS_A, SEQN_A

    CRC_B: CONNECTION_B , FAULT_STATUS_B, SAFETY_SENSOR_INPUTS_B, 
    SAFETY_SWITCH_INPUTS_B, SEQN_B
*
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Send_Data_Process(void)
{	
    u8 result,i;
    static u8 stat_u8TimerSendNonSafety = 0u;
  

    /* SF_B SEQN_B_1 and SEQN_B_2 has data, it has respone */
    if((!MB_CRC32( &OmcEscData.SafetySendData[0], 12u, SAFETY_POLYNOMIALSB )) && (g_u8SafetyRequest == 1u) )
    {
        g_u8SafetyRequest = 0u;
        
        Safety_Send_Data_Process(1u);
        
        EscDataToSafety[0][0] = EscData.SafetySendData[0];
        EscDataToSafety[0][1] = EscData.SafetySendData[1]; 
        EscDataToSafety[0][2] = EscData.SafetySendData[2];   
        EscDataToSafety[0][3] = EscData.SafetySendData[3];
        EscDataToSafety[0][4] = EscData.SafetySendData[4]; 
        EscDataToSafety[0][5] = EscData.SafetySendData[5];      
        EscDataToSafety[0][6] = EscData.SafetySendData[6];   
        EscDataToSafety[0][7] = EscData.SafetySendData[7]; 
        
        EscDataToSafety[1][0] = OmcEscData.SafetySendData[0];   
        EscDataToSafety[1][1] = OmcEscData.SafetySendData[1]; 
        EscDataToSafety[1][2] = OmcEscData.SafetySendData[2];   
        EscDataToSafety[1][3] = OmcEscData.SafetySendData[3]; 
        EscDataToSafety[1][4] = OmcEscData.SafetySendData[4];   
        EscDataToSafety[1][5] = OmcEscData.SafetySendData[5]; 
        EscDataToSafety[1][6] = OmcEscData.SafetySendData[6];   
        EscDataToSafety[1][7] = OmcEscData.SafetySendData[7];    

        EscDataToSafety[2][0] = EscData.SafetySendData[8];   
        EscDataToSafety[2][1] = EscData.SafetySendData[9]; 
        EscDataToSafety[2][2] = EscData.SafetySendData[10];   
        EscDataToSafety[2][3] = EscData.SafetySendData[11]; 
        EscDataToSafety[2][4] = OmcEscData.SafetySendData[8];   
        EscDataToSafety[2][5] = OmcEscData.SafetySendData[9]; 
        EscDataToSafety[2][6] = OmcEscData.SafetySendData[10];   
        EscDataToSafety[2][7] = OmcEscData.SafetySendData[11];

        /******************* DBL2 UP **********************/
        if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
        {         
            /* MESSAGE 1 */
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
            }
            
            /* MESSAGE 2 */
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
            }
            
            /* MESSAGE 3 */
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_UPPER_ID3, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);                 
            }       
            else
            {
                g_u16CANSendSuccess |= 0x04u;
            }
            
            /* for debug */
            if( g_u16CANSendSuccess == 0x07u )
            {
                g_u16CANSendSuccess = 0u;
                EscData.HeaderCode[1]++;
            }
        }
        /****************** DBL2 DOWN ********************/
        else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
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
            }
            
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
            }
            
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
            }
            
            /* for debug */
            if( g_u16CANSendSuccess == 0x07u )
            {
                g_u16CANSendSuccess = 0u;
                EscData.HeaderCode[1]++;
            }
        }
        /**************** DBL2 INTERM1 **********************/
        else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
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
                g_u16CANSendSuccess |= 0x01u;
            }
            
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_ID2, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x02u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
            else
            {
                g_u16CANSendSuccess |= 0x02u;
            }
            
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_ID3, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            } 
            else
            {
                g_u16CANSendSuccess |= 0x04u;
            }
            
            /* for debug */
            if( g_u16CANSendSuccess == 0x07u )
            {
                g_u16CANSendSuccess = 0u;
                EscData.HeaderCode[1]++;
            }            
        } 
        /********************** DBL2 INTERM2 ***********************/
        else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
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
                g_u16CANSendSuccess |= 0x01u;
            }
            
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_ID2, &EscDataToSafety[1][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x02u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }   
            else
            {
                g_u16CANSendSuccess |= 0x02u;
            }
            
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_ID3, &EscDataToSafety[2][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x04u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            } 
            else
            {
                g_u16CANSendSuccess |= 0x04u;
            }
            
            /* for debug */
            if( g_u16CANSendSuccess == 0x07u )
            {
                g_u16CANSendSuccess = 0u;
                EscData.HeaderCode[1]++;
            }            
        }         
        else
        {}

        
        
        /* clear data */
        Safety_Send_Data_Process(0u);
        for( i = 0u; i < 12u; i++ )
        {
            OmcEscData.SafetySendData[i] = 0u;
        }
    }
    
/*
NON SAFE DATA     
Byte	Bits (7 is MSB)	Data
0-1	0-11	ANALOG INPUT 1 (mA*100)	 
1-2	12-23	ANALOG INPUT 2 (mA*100)	 
3	0-7	PT100 (oC)	 
4	0-7	SAFETY SWITCH INPUTS_A (1-8 inputs)	 
5	0-7	SAFETY SWITCH INPUTS_A (9-16 inputs)	 
6	0-7	SAFETY SWITCH INPUTS_B (1-8 inputs)	 
7	0-7	SAFETY SWITCH INPUTS_B (9-16 inputs)	
*/   
#if 1
    stat_u8TimerSendNonSafety++;
    /* message cycle: 100ms */
    if(( stat_u8TimerSendNonSafety * SYSTEMTICK ) >= 100u )
    {
        stat_u8TimerSendNonSafety = 0u;       
        /* for debug */
        EscDataToSafety[3][4] = EscData.HeaderCode[0];
        EscDataToSafety[3][5] = EscData.HeaderCode[1];
        EscDataToSafety[3][3] = EscData.HeaderCode[2];
        
        EscDataToSafety[3][0] = (u8)EscData.DBL2AnalogData[0];
        EscDataToSafety[3][1] |= (u8)((EscData.DBL2AnalogData[0] >> 8u) & 0x0fu);
        EscDataToSafety[3][1] |= (u8)((EscData.DBL2AnalogData[1] << 4u) & 0xf0u);
        EscDataToSafety[3][2] = (u8)((EscData.DBL2AnalogData[1] >> 4u) & 0xffu);  
        /*
        EscDataToSafety[3][3] = (u8)EscData.DBL2AnalogData[2];
        EscDataToSafety[3][4] = EscData.DBL2InputData[2];
        EscDataToSafety[3][5] = EscData.DBL2InputData[3];
        */
        EscDataToSafety[3][6] = OmcEscData.DBL2InputData[2];       
        EscDataToSafety[3][7] = OmcEscData.DBL2InputData[3];        
        
        
        /* DBL2 UP */
        if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
        {         
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_UPPER_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }          
        }
        /* DBL2 DOWN */
        else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
        {          
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_LOWER_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
        }
        /* DBL2 INTERM1 */
        else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
        {          
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM1_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
        } 
        /* DBL2 INTERM2 */
        else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
        {          
            result = Can_Send_Msg(CAN1, CAN1TX_DBL2_INTERM2_NONSAFETY_ID, &EscDataToSafety[3][0], CAN_FRAME_LEN ); 
            if( result )
            {
                /* No mail box, send fail */
                g_u16CANSendFail |= 0x08u;
                CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
            }  
        }         
        else
        {}
    }
#endif  
}


/*******************************************************************************
* Function Name  : Can_Receive_Data_Process
* Description    :                  
* Input          : None
* Output         : None
* Return         : None 
*******************************************************************************/
static void Can_Receive_Data_Process(void)
{	
    u8 i;
    u8 u8_ValidateError = 0u;
    
/*
MESSAGE 1
Byte	Bits (7 is MSB)	Data
0	0-1	CONNECTION_A_1
0	2-3	RESET_A_1
0	4-5	CONNECTION_B_1
0	6-7	RESET_B_1
1	0-7	SEQN_A_1
2	0-7	SEQN_B_1
3	0-1	CONNECTION_A_2
3	2-3	RESET_A_2
3	4-5	CONNECTION_B_2
3	6-7	RESET_B_2
4	0-7	SEQN_A_2
5	0-7	SEQN_B_2
6	0-7	OUTPUTS (No safety relevant)
7	0-7	NOT USED

MESSAGE 2
Byte	Bits (7 is MSB)	Data
0,1,2,3	0-31	CRC_A
4,5,6,7	0-31	CRC_B

CRC_A: RESET_A, SEQN_A, CONNECTION_A, OUTPUTS
CRC_B: RESET_B, SEQN_B, CONNECTION_B
    
*/     
    /* SF SEQN_A_1 and SEQN_B_1 has data, it has request */
    if( (EscDataFromSafety[0][1]) && (EscDataFromSafety[0][2]) && 
       ( (EscDataFromSafety[1][0]) || (EscDataFromSafety[1][1]) ) && ( (EscDataFromSafety[1][4]) || (EscDataFromSafety[1][5]) ))
    {
        Can_Clean_Buffer();
        
        /* for debug */
        EscData.HeaderCode[0]++;
        EscData.HeaderCode[2] = EscDataFromSafety[0][1];
        
        EscData.SafetyReceiveDataA[0] |= (EscDataFromSafety[0][0] & 0x0fu);
        EscData.SafetyReceiveDataA[0] |= (u8)((EscDataFromSafety[0][3] << 4u) & 0xf0u);     
        EscData.SafetyReceiveDataA[1] = EscDataFromSafety[0][1];     
        EscData.SafetyReceiveDataA[2] = EscDataFromSafety[0][4];      
        EscData.SafetyReceiveDataA[3] = EscDataFromSafety[0][6];
        EscData.SafetyReceiveDataA[4] = EscDataFromSafety[1][0];
        EscData.SafetyReceiveDataA[5] = EscDataFromSafety[1][1];
        EscData.SafetyReceiveDataA[6] = EscDataFromSafety[1][2];
        EscData.SafetyReceiveDataA[7] = EscDataFromSafety[1][3];
        
        EscData.SafetyReceiveDataB[0] |= (u8)((EscDataFromSafety[0][0] >> 4u) & 0x0fu);
        EscData.SafetyReceiveDataB[0] |= (EscDataFromSafety[0][3] & 0xf0u);     
        EscData.SafetyReceiveDataB[1] = EscDataFromSafety[0][2];     
        EscData.SafetyReceiveDataB[2] = EscDataFromSafety[0][5]; 
        EscData.SafetyReceiveDataB[3] = EscDataFromSafety[0][7];
        EscData.SafetyReceiveDataB[4] = EscDataFromSafety[1][4];
        EscData.SafetyReceiveDataB[5] = EscDataFromSafety[1][5];
        EscData.SafetyReceiveDataB[6] = EscDataFromSafety[1][6];
        EscData.SafetyReceiveDataB[7] = EscDataFromSafety[1][7];
        
        /* Validation Request: Check CONNECTION, CRC and compare _1 and _2 data */       
        /* check CRC */
        if( !MB_CRC32( &EscData.SafetyReceiveDataA[0], 8u, SAFETY_POLYNOMIALS ))
        {  
            /* compare CONNECTION */
            if((EscData.SafetyReceiveDataA[0] & 0x03u) != ((EscData.SafetyReceiveDataA[0] >> 4u) & 0x03u))
            {
                u8_ValidateError = 1u;
            }
            /* compare RESET */
            else if((EscData.SafetyReceiveDataA[0] & 0x0cu) != ((EscData.SafetyReceiveDataA[0] >> 4u) & 0x0cu))
            {
                u8_ValidateError = 1u;
            }            
            /* compare SEQN */
            else if( EscData.SafetyReceiveDataA[1] != EscData.SafetyReceiveDataA[2] )
            {
                u8_ValidateError = 1u;
            }
            else
            {   
                /* check CONNECTION */
                /* DBL2 UPPER */
                if( EscData.SwdpAdr == DBL2_UPPER_ADDR )
                {                      
                    if( (EscData.SafetyReceiveDataA[0] & 0x03u) != CONNECTION_DBL2_UPPER )
                    {
                        u8_ValidateError = 1u;
                    }
                }
                /* DBL2 DOWN */
                else if( EscData.SwdpAdr == DBL2_LOWER_ADDR )
                {     
                    if( (EscData.SafetyReceiveDataA[0] & 0x03u) != CONNECTION_DBL2_LOWER )
                    {
                        u8_ValidateError = 1u;
                    }
                }
                /* DBL2 INTERM1 */
                else if( EscData.SwdpAdr == DBL2_INTERM1_ADDR )
                {     
                    if( (EscData.SafetyReceiveDataA[0] & 0x03u) != CONNECTION_DBL2_INTERM1 )
                    {                  
                        u8_ValidateError = 1u;
                    }
                } 
                /* DBL2 INTERM2 */
                else if( EscData.SwdpAdr == DBL2_INTERM2_ADDR )
                {          
                    if( (EscData.SafetyReceiveDataA[0] & 0x03u) != CONNECTION_DBL2_INTERM2 )
                    {
                        u8_ValidateError = 1u;
                    }
                }         
                else
                {
                    u8_ValidateError = 1u;
                }                  
                
                if( u8_ValidateError == 0u )
                {  
                    /* validate data */
                    EscData.SafetyValidateResult = 0x01u;                        
                }
            }
        }
        else
        {
            u8_ValidateError = 1u;
        }
        
        /* receive finish, clear the data */
        for( i = 0u; i < 8u; i++ )
        {
            EscDataFromSafety[0][i] = 0u;
            EscDataFromSafety[1][i] = 0u;
        }
    }
}

/*******************************************************************************
* Function Name  : Communication_To_Safety
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Communication_To_Safety(void)
{
    static u8 can1_comm_timeout = 0u;
    static u16 stat_u16TimerCan1CommWait = 0u, stat_u16Can1HandshakeSuccess = 0u;
    
    if( can1_receive == 1u )
    {
        can1_receive = 0u;
        can1_comm_timeout = 0u;
        stat_u16Can1HandshakeSuccess = 1u;
    }
    else if( stat_u16Can1HandshakeSuccess )
    {
        if( ++can1_comm_timeout * SYSTEMTICK >= 100u )
        {
            /*  can communication timeout process */
            EscData.SafetyReset = 0u;
            EscData.SEQN = 0u;
            EscData.DBL2OutputData = 0u;
        }
    }
    else
    {
        /* for debug */
        EscData.HeaderCode[0] = 0u;
        EscData.HeaderCode[1] = 0u;
        
        Communication_Can_Filter();
        
        stat_u16TimerCan1CommWait++;
        if( stat_u16TimerCan1CommWait * SYSTEMTICK > CAN_COMM_HAND_TIME )
        {
            /*  can communication handshake timeout when power on */

        }            
    }  
    
    if( g_u8SafetyNewData == 0x11u )
    {
        g_u8SafetyNewData = 0u;
        /* Receive data from SF */
        Can_Receive_Data();    
        Can_Receive_Data_Process();
    }
    
    /* Send data to SF */
    Can_Send_Data_Process();        
}




/******************************  END OF FILE  *********************************/