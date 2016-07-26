/*******************************************************************************
* File Name          : hw_test.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Contains some test function.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "hw_test.h"
#include "led.h"
#include "spi.h"
#include "ewdt.h"
#include "crc16.h"
#include "safety_test.h"
#include "esc.h"
#include "esc_comm_dualcpu.h"
#ifdef GEC_DBL2_MASTER  
#include "can.h"
#endif


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 sflag = 0,inputnum = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


    
        

/*******************************************************************************
* Function Name  : Input_Check
* Description    : Monitor the input pin status and test.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Input_Check(void)
{
  
        u32 *ulPt_Input1,*ulPt_Input2;
        u8 i;


        if( testmode == 1 )
        {
            ulPt_Input1 = (u32*)&EscRTBuff[4];       
            ulPt_Input2 = (u32*)&EscRTBuff[8];
            sflag = 0;
            inputnum = 0;        
            
            
            for( i = 0; i < 29; i++ )
            {
                if( *ulPt_Input1 & ((u32)( 1 << i )))
                {
                    sflag++;
                    inputnum = i + 1;
                }
            }
            
            for( i = 0; i < 17; i++ )
            {
                if( *ulPt_Input2 & ((u32)( 1 << i )))
                {
                    sflag++;
                    inputnum = i + 30;
                }
            }   
            
#ifdef GEC_DBL2_MASTER             
            CAN1_TX_Data[0] = inputnum;
            CAN1_TX_Data[1] = sflag;            
#endif         
            
            if (( inputnum == 0 ) || ( sflag > 1 ))
            {
                SF_RELAY_OFF(); 
                AUX_RELAY_OFF();
            }
#ifdef GEC_DBL2_MASTER   
            else if( inputnum & 0x0A )
            {
                if ( inputnum & 0x08 )
                {
                    SF_RELAY_ON(); 
                }
                
                if ( inputnum & 0x02 )
                {
                    AUX_RELAY_ON(); 
                } 
            }
#else
            else if( inputnum & 0x05 )
            {
                if ( inputnum & 0x04 )
                {
                    SF_RELAY_ON(); 
                }
                
                if ( inputnum & 0x01 )
                {
                    AUX_RELAY_ON(); 
                }   
            }
#endif
            else
            {
                SF_RELAY_OFF(); 
                AUX_RELAY_OFF();                
            }
            
        }
}


/*******************************************************************************
* Function Name  : CrossCommCPUCheck
* Description    : CrossCommCPU and Data Integrity test
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void CrossCommCPUCheck(void)
{    

    u16 i = 0;  
    u8 number = 0;
    u8 data_error = 0;
    u32 test_cnt = 100;  
    u16 comm_num = buffersize;
  
#ifdef GEC_DBL2_MASTER      
    u8 result = 0;
#endif
        
    number++;

    for( i = 0; i < comm_num; i++)
    {
        SPI1_TX_Data[i] = number;
    }
    
    i = MB_CRC16( SPI1_TX_Data, comm_num - 2 );
    SPI1_TX_Data[comm_num - 2] = i;
    SPI1_TX_Data[comm_num - 1] = i>>8;
    
    
    SPI1_DMA_ReceiveSendByte(comm_num);

#ifdef GEC_DBL2_MASTER
    DMA_Check_Flag(100000);
#else
    DMA_Check_Flag(100000000);
#endif
    
        
        while(test_cnt--)
        {
            
            EWDT_TOOGLE(); 
            IWDG_ReloadCounter();             
        
#ifdef GEC_DBL2_MASTER              
              
              delay_ms(2);
              
              number++;
              if(number >= 255)number = 0;

              for( i = 0; i < comm_num - 2; i++ )
              {
                    SPI1_TX_Data[i] = number;
              }
              
              i = MB_CRC16( SPI1_TX_Data, comm_num - 2 );
              SPI1_TX_Data[comm_num - 2] = i;
              SPI1_TX_Data[comm_num - 1] = i>>8;
              
              SPI1_DMA_ReceiveSendByte(comm_num);
                        
              DMA_Check_Flag(40000);
              
              if(!MB_CRC16(SPI1_RX_Data, comm_num))
              {

                  for( i=0; i < comm_num - 2; i++ )
                  {
                      result = SPI1_RX_Data[i]^(SPI1_TX_Data[i] - 1);
                      if( result )
                      {
                          data_error++;                            
                          break;

                      }
                  
                  }
              } 
              else
              {
                  data_error++;                 
  
              }
              
              if( data_error > 2 )
              {
                  break;
              }               
              
              
#else
              if(!MB_CRC16(SPI1_RX_Data, comm_num))
              {                 
                  for( i=0; i < comm_num - 2; i++ )
                  {
                      SPI1_TX_Data[i] = SPI1_RX_Data[i];
                  }   
                  
                  i = MB_CRC16( SPI1_TX_Data, comm_num - 2 );
                  SPI1_TX_Data[comm_num - 2] = i;
                  SPI1_TX_Data[comm_num - 1] = i>>8;
              } 
              else
              {
                  data_error++;
                  if( data_error > 2 )
                  {
                      break;
                  }
              }
              
              SPI1_DMA_ReceiveSendByte(comm_num);

              DMA_Check_Flag(40000);
#endif              
        }      
        

#ifdef GEC_DBL2_MASTER 
#else
        SPI1_DMA_ReceiveSendByte(comm_num);
#endif
    if( data_error > 2 )
    {
        /* SPI1_DMA_Check error */
        EN_ERROR7 |= 0x01;

    }


}


#ifdef GEC_DBL2_MASTER 
/*******************************************************************************
* Function Name  : HardwareTEST
* Description    : Test the board.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void HardwareTEST(void)
{
    u8 testdata1[10];
    u8 testerror = 0;
    u8 len = 0, len1 = 0;
    u16 waittms = 0;
    u8 senddata[50],recvdata[50];
    
    CAN1_TX_Data[0] = 0xf1;
    testdata1[0] = 0xf1;
    for( u8 i = 1; i < 10 ; i++ )
    {
        CAN1_TX_Data[i] = i + 0x90;
        testdata1[i] = i + 0x90;       
    }
    BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);
    
    do
    {
        len1 = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
        delay_ms(1);
        EWDT_TOOGLE();
        waittms++;
        if( waittms > 5000 )
        {
            waittms = 0;
            break;
        }
    }
    while( len1 != 10 || CAN1_RX_Data[0] != 0xf1 );      
    
    if( len1 == 10 && CAN1_RX_Data[0] == 0xf1 )
    {
        waittms = 0;
        for( u8 i = 2; i < 10 ; i++ )
        {
            CAN1_TX_Data[i] = CAN1_RX_Data[i];
        }
        BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);                
        
        waittms = 0;
        do
        {
            len1 = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
            delay_ms(1);
            EWDT_TOOGLE();
            waittms++;
            if( waittms > 2000 )
            {
                waittms = 0;
                break;
            }
        }
        while( len1 != 10 || CAN1_RX_Data[0] != 0xf1 );      
        
        if( len1 == 10 && CAN1_RX_Data[0] == 0xf1 )
        {
            for( u8 i = 2; i < 10 ; i++ )
            {
                if( CAN1_RX_Data[i] != testdata1[i] )
                {
                    testerror++;
                    break;
                }
            }

 
            
            if( testerror == 0 )
            {
                if( ( CAN1_RX_Data[1] == 0xc1 ) 
                   || ( CAN1_RX_Data[1] == 0xf1 ) )
                {
                    testmode = 1;
                }
            }
        } 
        
        
    }   
    else
    {

	        
    }
    

    senddata[0] = 0xbc;
    if( testmode == 1 )
    {
        senddata[1] = 0x01;
    }
    else
    {
        senddata[1] = 0x02;
    }
    CPU_Exchange_Data(senddata, 2);//send
    CPU_Data_Check(recvdata, &len);
    
}
#else
/*******************************************************************************
* Function Name  : HardwareTEST
* Description    : Test the board.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void HardwareTEST(void)
{

    u8 len = 0;
    u8 senddata[10],recvdata[10];

    
    
    senddata[0] = 0xbc;
    if( testmode == 1 )
    {
        senddata[1] = 0x01;
    }
    else
    {
        senddata[1] = 0x02;
    }
    CPU_Exchange_Data(senddata, 2);
    CPU_Data_Check(recvdata, &len);//recv  
    
    if( len == 0x02 && recvdata[0] == 0xbc )
    {
        if( recvdata[1] == 1 )
        {
            testmode = 1;
        }
        else if( recvdata[1] == 2 )
        {
            testmode = 0;
        }
    } 
    
}


#endif


/******************************  END OF FILE  *********************************/



