/*******************************************************************************
* File Name          : esc_comm_dualcpu.c
* Author             : lison
* Version            : V1.0
* Date               : 06/20/2016
* Description        : This file contains esc communication between dual cpu.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_comm_dualcpu.h"
#include "spi.h"
#include "crc16.h"
#include "esc_error_process.h"
#include "bsp_iocfg.h"
#include "initial_devices.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CPU_COMM_TIMEOUT  5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void CPU_Comm(void);

u16 comm_num = 0;
u8 onetime = 0;
#ifndef GEC_DBL2_MASTER
static u16 comm_timeout = 100;
#endif

u8 cpu_senddata_buffer[250];
u8 cpu_recvdata_buffer[250];
u8 recvlen = 0;

/*******************************************************************************
* Function Name  : Communication_CPU
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Communication_CPU(void)
{
    static u8 comm_cpu_tms = 0;
    
    CPU_Comm();
    
    comm_cpu_tms++;
#ifdef GEC_DBL2_MASTER
    if( comm_cpu_tms * ( SYSTEMTICK * 4 ) >= 200 )
    {
        comm_cpu_tms = 0;
        Receive_IO_status_from_CPU();
    }
#else
    if( comm_cpu_tms * ( SYSTEMTICK * 2 ) >= 200 )
    {
        comm_cpu_tms = 0;
        Receive_IO_status_from_CPU();
    }    
    
#endif
}

/*******************************************************************************
* Function Name  : Send_state_to_CPU2
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Send_state_to_CPU(void)
{
    /* 1. esc Rtdata --------------------------*/
    for( u8 i = 0; i < 100; i++)
    {
        cpu_senddata_buffer[i] = EscRTBuff[i];
    }    
    
}

/*******************************************************************************
* Function Name  : Send_state_to_CPU2
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Receive_state_from_CPU(void)
{
    if( recvlen == 100 )
    {
        /* 1. esc Rtdata receive--------------------------*/
        for( u8 i = 0; i < 100; i++)
        {
            McRxBuff[i] = cpu_recvdata_buffer[i];
        }
           
    }
}

/*******************************************************************************
* Function Name  : Send_state_to_CPU2
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Receive_IO_status_from_CPU(void)
{
      static u8 receive_io_error = 0;
    
      for( u8 i = 4; i < 8; i++ )
      {
          if( pcOMC_EscRTBuff[i] != EscRTBuff[i] )
          {
              receive_io_error++;
              break;
          }
      }
      
      if( receive_io_error > 5 )
      {
          NVIC_SystemReset();
      }
      else
      {
          receive_io_error = 0;
      }
}

/*******************************************************************************
* Function Name  : CPU_Comm
* Description    : Cpu1 and cpu2 communication.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void CPU_Comm(void)
{

#ifdef GEC_DBL2_MASTER
    if( onetime == 0 )
    {
        onetime++;

        Send_state_to_CPU();
        CPU_Exchange_Data(cpu_senddata_buffer, 100);
    }
    else
    {
        CPU_Data_Check(cpu_recvdata_buffer, &recvlen);
        Receive_state_from_CPU();
               
        Send_state_to_CPU();        
        CPU_Exchange_Data(cpu_senddata_buffer, 100);
    }
#else  
    comm_timeout--;

    if( comm_timeout == 0 )
    {
        /* CPU_Comm---comm_timeout */
        EN_ERROR7 |= 0x01;
        ESC_SPI_Error_Process();
        comm_timeout = CPU_COMM_TIMEOUT;
    }
    if ( ( DMA_GetFlagStatus(DMA1_IT_TC2) ) != RESET )
    {
        
        comm_timeout = CPU_COMM_TIMEOUT;

        CPU_Data_Check(cpu_recvdata_buffer, &recvlen);
        Receive_state_from_CPU();
                
        Send_state_to_CPU();
        CPU_Exchange_Data(cpu_senddata_buffer, 100);
        
        EN_ERROR7 &= ~0x01;
    }   
#endif
}

/*******************************************************************************
* Function Name  : CPU_Data_Check
* Description    : Check the receive data.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void CPU_Data_Check( u8 *buffer, u8 *len )
{ 
       
    /* communication buffer */
    comm_num = buffersize;  
       
    DMA_Check_Flag(10000000);    
        
    if(!MB_CRC16(SPIx_RX_Data, comm_num))
    {
        
        EN_ERROR_SYS3 = 0;    
        EN_ERROR7 &= ~0x02;
        
        *len = SPIx_RX_Data[0];       
        for( u8 i = 0; i < *len; i++ )
        {
            buffer[i] = SPIx_RX_Data[ i + 1 ];
        }
                  
    }
    else
    {
        EN_ERROR_SYS3++;     
        /* MB_CRC16 error */
                       
    }
    
    if(EN_ERROR_SYS3 > 2)
    {
        EN_ERROR_SYS3 = 0;
        /* CPU_Exchange_Data_Check error */
        EN_ERROR7 |= 0x02;
    }          
}

/*******************************************************************************
* Function Name  : CPU_Exchange_Data
* Description    : Cpu send data by spi.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void CPU_Exchange_Data( u8 *buffer, u8 len )
{   
    u16 i;
    
    /* communication buffer */
    comm_num = buffersize;  
    for(i = 0; i < comm_num - 2; i++)
    {
        SPIx_TX_Data[i] = 0;
    }
    
    SPIx_TX_Data[0] = len;
    for( i = 0; i < len; i++ )
    {
        SPIx_TX_Data[i + 1] = buffer[i];
    }
    
    i = MB_CRC16( SPIx_TX_Data, comm_num - 2 );
    SPIx_TX_Data[comm_num - 2] = i;
    SPIx_TX_Data[comm_num - 1] = i >> 8; 
    
    SPIx_DMA_ReceiveSendByte(comm_num);
           
}


/******************************  END OF FILE  *********************************/