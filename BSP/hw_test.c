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
#include "bsp_input.h"
#ifdef GEC_DBL2_SLAVE  
#include "can.h"
#endif


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef GEC_DBL2_SLAVE
static u8 sflag1 = 0u,inputnum1 = 0u;
static u8 sflag2 = 0u,inputnum2 = 0u;
#endif
u8 testmode = 0u;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


    
        
#ifdef GEC_DBL2_SLAVE
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
        u8 *ulPt_Output;
        u8 Dip_value1,Dip_value2;
        u8 i;

        if( testmode == 1u )        
        {
            ulPt_Input2 = (u32*)&EscData.DBL2InputData[0];
            ulPt_Input1 = (u32*)&OmcEscData.DBL2InputData[0];
            ulPt_Output = &EscData.DBL2OutputData;
            
            
            sflag1 = 0u;
            inputnum1 = 0u;      
            sflag2 = 0u;
            inputnum2 = 0u;            
            
            for( i = 0u; i < 32u; i++ )
            {
                if( *ulPt_Input1 & (( 1u << i )))
                {
                    sflag1++;
                    inputnum1 = i + 1u;
                }
            }        

            for( i = 0u; i < 32u; i++ )
            {
                if( *ulPt_Input2 & (( 1u << i )))
                {
                    sflag2++;
                    inputnum2 = i + 1u;
                }
            } 
            
            if(( sflag1 != sflag2 ) || ( inputnum1 != inputnum2 ))
            {
                sflag2 = 2u;
            }
            
            Dip_value2 = ReadSwDp();
            for( i = 0u; i < 4u; i++ )
            {
                if( Dip_value2 & ((u8)( 1u << i )))
                {
                    sflag2++;
                    inputnum2 = i + 33u;
                }
            }  
            
            Dip_value1 = OmcEscData.SwdpAdr;
            for( i = 0u; i < 4u; i++ )
            {
                if( Dip_value1 & ((u8)( 1u << i )))
                {
                    sflag2++;
                    inputnum2 = i + 37u;
                }
            } 
            
            if(( inputnum2 == 0u ) || ( sflag2 > 1u ))
            {
                
                *ulPt_Output = 0u;
                
            }        
            else
            {                        
                
                if( inputnum2 <= 36u )
                {
                    *ulPt_Output |= ( inputnum2 );
                }
                else
                {
                    switch( Dip_value1 )
                    {
                       case 0x01u: *ulPt_Output |= ( inputnum2 );break; 
                       case 0x02u: *ulPt_Output |= ( inputnum2 + 26u );break; 
                       case 0x04u: *ulPt_Output |= ( inputnum2 + 89u );break;
                       case 0x08u: *ulPt_Output |= ( inputnum2 + 152u );break;
                       default: *ulPt_Output = 0u; break;
                    }
                }
            }
        }    
 
}
#endif

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

    u32 i = 0u;  
    u8 number = 0u;
    u8 data_error = 0u;
    u32 test_cnt = 100u;  
    u16 comm_num = buffersize;
  
#ifdef GEC_SF_MASTER      
    u8 testresult = 0u;
#endif
        
    number++;

    for( i = 0u; i < comm_num; i++)
    {
        SPIx_TX_Data[i] = number;
    }
    
    i = MB_CRC32( SPIx_TX_Data, comm_num - 4u, MAIN_POLYNOMIALS );
    SPIx_TX_Data[comm_num - 4u] = (u8)(i >> 24u);
    SPIx_TX_Data[comm_num - 3u] = (u8)(i >> 16u);     
    SPIx_TX_Data[comm_num - 2u] = (u8)(i >> 8u);
    SPIx_TX_Data[comm_num - 1u] = (u8)i;     
    
    
    SPIx_DMA_ReceiveSendByte(comm_num);

#ifdef GEC_SF_MASTER
    DMA_Check_Flag(100000u);
#else
    DMA_Check_Flag(10000000u);
#endif
    
        
        while(test_cnt--)
        {
            
            EWDT_TOOGLE(); 
            IWDG_ReloadCounter();             
        
#ifdef GEC_SF_MASTER              
              
              delay_ms(2u);
              
              number++;
              if(number >= 255u)
              {
                  number = 0u;
              }
              for( i = 0u; i < comm_num - 4u; i++ )
              {
                    SPIx_TX_Data[i] = number;
              }
              
              i = MB_CRC32( SPIx_TX_Data, comm_num - 4u, MAIN_POLYNOMIALS );
              SPIx_TX_Data[comm_num - 4u] = (u8)(i >> 24u);
              SPIx_TX_Data[comm_num - 3u] = (u8)(i >> 16u);     
              SPIx_TX_Data[comm_num - 2u] = (u8)(i >> 8u);
              SPIx_TX_Data[comm_num - 1u] = (u8)i;     
              
              SPIx_DMA_ReceiveSendByte(comm_num);
                        
              DMA_Check_Flag(40000u);
              
              if(!MB_CRC32(SPIx_RX_Data, comm_num, MAIN_POLYNOMIALS))
              {

                  for( i = 0u; i < comm_num - 4u; i++ )
                  {
                      testresult = SPIx_RX_Data[i]^(SPIx_TX_Data[i] - 1u);
                      if( testresult )
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
              
              if( data_error > 2u )
              {
                  break;
              }               
              
              
#else
              if(!MB_CRC32(SPIx_RX_Data, comm_num, MAIN_POLYNOMIALS))
              {                 
                  for( i = 0u; i < comm_num - 4u; i++ )
                  {
                      SPIx_TX_Data[i] = SPIx_RX_Data[i];
                  }   
                  
                  i = MB_CRC32( SPIx_TX_Data, comm_num - 4u, MAIN_POLYNOMIALS );
                  SPIx_TX_Data[comm_num - 4u] = (u8)(i >> 24u);
                  SPIx_TX_Data[comm_num - 3u] = (u8)(i >> 16u);     
                  SPIx_TX_Data[comm_num - 2u] = (u8)(i >> 8u);
                  SPIx_TX_Data[comm_num - 1u] = (u8)i; 
              } 
              else
              {
                  data_error++;
                  if( data_error > 2u )
                  {
                      break;
                  }
              }
              
              SPIx_DMA_ReceiveSendByte(comm_num);

              DMA_Check_Flag(40000u);
#endif              
        }      
        

#ifdef GEC_DBL2_MASTER 
#else
        /* SPIx_DMA_ReceiveSendByte(comm_num);*/
#endif
    if( data_error > 2u )
    {
        /* SPI1_DMA_Check error */
        EN_ERROR7 |= 0x01u;

    }


}


#ifdef GEC_DBL2_SLAVE 
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
#if 0     
    u8 testdata1[10];
    u8 testerror = 0;
    u8 len = 0, len1 = 0;
    u16 waittms = 0;
    u8 senddata[50],recvdata[50];
    
    CAN1_TX_Data[0] = 0xf1;
    testdata1[0] = 0xf1;
    for( u8 i = 1; i < 10 ; i++ )
    {
        CAN1_TX_Data[i] = i + 0xa0;
        testdata1[i] = i + 0xa0;       
    }
    BSP_CAN_Send(CAN1, &CAN1_TX_Normal, CAN1_TEST_ID, CAN1_TX_Data, 10);
    
    do
    {
        len1 = BSP_CAN_Receive(CAN1, &CAN1_RX_Normal, CAN1_RX_Data, 0);
        delay_ms(1);
        EWDT_TOOGLE();
        IWDG_ReloadCounter(); 
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
            IWDG_ReloadCounter(); 
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
                if( CAN1_RX_Data[1] == 0xb1 )
                {
                    testmode = 1;
                }               
            }
        } 
        
        
    }   
    else
    {

	        
    }
    
    /* no CAN */
//    testmode = 1;
    /*********/
    
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
    
    while( len != 2 || recvdata[0] != 0xbc )
    {
        SPIx_Configuration(SPI1);
        CPU_Exchange_Data(senddata, 2);
        CPU_Data_Check(recvdata, &len);//recv  
        delay_ms(100);
        EWDT_TOOGLE();
        IWDG_ReloadCounter(); 
    }

    if( len == 0x02 && recvdata[0] == 0xbc )
    {
        CPU_Exchange_Data(senddata, 2);
    }
#endif    
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
#if 0
    u8 len = 0;
    u8 senddata[10],recvdata[10];

    
    senddata[0] = 0xbc;
    senddata[1] = 0x01;

    CPU_Exchange_Data(senddata, 2);//send
    CPU_Data_Check(recvdata, &len); 
    while( len != 2 || recvdata[0] != 0xbc )
    {
        CPU_Exchange_Data(senddata, 2);//send
        CPU_Data_Check(recvdata, &len); 
        delay_ms(100);
        EWDT_TOOGLE();
        IWDG_ReloadCounter(); 
    }
    
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
#endif    
}


#endif


/******************************  END OF FILE  *********************************/




