/*******************************************************************************
* File Name          : esc_comm_dualcpu.c
* Author             : lison
* Version            : V1.0
* Date               : 06/20/2016
* Last modify date   : 10/28/2016
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
#define CPU_COMM_TIMEOUT  50u

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void CPU_Comm(void);
static void Send_state_to_CPU(void);
static void Receive_state_from_CPU(void);
static void Receive_IO_status_from_CPU(void);
static void CPU_Data_Check( u8 buffer[], u8 *len, u32 times );
static void CPU_Exchange_Data( u8 buffer[], u8 len );

static u16 comm_num = 0u;
static u8 onetime = 0u;
#ifndef GEC_DBL2_MASTER
static u16 comm_timeout = 100u;
#endif

static u8 *cpu_senddata_buffer;
static u8 *cpu_recvdata_buffer;
static u8 recvlen = 0u;

/*******************************************************************************
* Function Name  : Communication_CPU
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
void Communication_CPU(void)
{
    static u8 comm_cpu_tms = 0u;
    
    CPU_Comm();
    
    comm_cpu_tms++;
#ifdef GEC_DBL2_MASTER
    if( comm_cpu_tms * ( SYSTEMTICK * 4u ) >= 200u )
    {
        comm_cpu_tms = 0u;
        Receive_IO_status_from_CPU();
    }
#else
    if( comm_cpu_tms * ( SYSTEMTICK * 2u ) >= 200u )
    {
        comm_cpu_tms = 0u;
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
static void Send_state_to_CPU(void)
{
    cpu_senddata_buffer = (u8*)&EscData;        
}

/*******************************************************************************
* Function Name  : Send_state_to_CPU2
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
static void Receive_state_from_CPU(void)
{
    cpu_recvdata_buffer = (u8*)&OmcEscData;
}

/*******************************************************************************
* Function Name  : Send_state_to_CPU2
* Description    : None
* Input          : None          
* Output         : None
* Return         : None
*******************************************************************************/
static void Receive_IO_status_from_CPU(void)
{
      static u8 receive_io_error = 0u;
      u8 i;
    
      for( i = 0u; i < 4u; i++ )
      {
          if( OmcEscData.DBL2InputData[i] != EscData.DBL2InputData[i] )
          {
              receive_io_error++;
              break;
          }
      }
      
      if( receive_io_error > 5u )
      {
          
      }
      else
      {
          receive_io_error = 0u;
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
static void CPU_Comm(void)
{
    u8 i;
#ifdef GEC_DBL2_MASTER

    Send_state_to_CPU();        
    CPU_Exchange_Data(cpu_senddata_buffer, ESC_RT_DATA_LEN);
    
    Receive_state_from_CPU();
    CPU_Data_Check(cpu_recvdata_buffer, &recvlen, 1000u );              

#else  
    comm_timeout--;

    if( comm_timeout == 0u )
    {
        /* CPU_Comm---comm_timeout */
        EN_ERROR7 |= 0x01u;
        ESC_SPI_Error_Process();
        comm_timeout = CPU_COMM_TIMEOUT;
    }
    
    if( onetime == 0u )
    {
        onetime++;
        
        Send_state_to_CPU();
        CPU_Exchange_Data(cpu_senddata_buffer, ESC_RT_DATA_LEN);
    }
    
    if ( ( DMA_GetFlagStatus(DMA1_IT_TC2) ) != RESET )
    {
        
        comm_timeout = CPU_COMM_TIMEOUT;

        Receive_state_from_CPU();
        CPU_Data_Check(cpu_recvdata_buffer, &recvlen, 1000u );        
                
        Send_state_to_CPU();
        CPU_Exchange_Data(cpu_senddata_buffer, ESC_RT_DATA_LEN);
        
        EN_ERROR7 &= ~0x01u;
        
        /* clear receive data */
        for( i = 0u; i < 8u; i++ )
        {
            EscData.SafetyReceiveDataB[i] = 0u;       
        }        
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
static void CPU_Data_Check( u8 buffer[], u8 *len, u32 times )
{ 
    static u8 stat_u8CheckError = 0u;
    u8 i;
    
    /* communication buffer */
    comm_num = buffersize;  
       
    DMA_Check_Flag(times);    
        
#if 0    
    if(!MB_CRC16(SPIx_RX_Data, comm_num))
#else
    if(!STM_CRC32( (u32*)&SPIx_RX_Data, ( (u32)comm_num >> 2u )))
#endif 
    {
        
        stat_u8CheckError = 0u;    
        g_u8LedFreq = FREQ_0_5HZ;
        
        *len = SPIx_RX_Data[0];       
        for( i = 0u; i < *len; i++ )
        {
            buffer[i] = SPIx_RX_Data[ i + 1u ];
        }
                  
    }
    else
    {
        stat_u8CheckError++;     
        /* MB_CRC16 error */
#ifdef GEC_DBL2_SLAVE         
        SPIx_Configuration(SPI1);               
#endif                        
    }
    
    if(stat_u8CheckError > 3u)
    {
        stat_u8CheckError = 0u;
        /* CPU_Exchange_Data_Check error */
        EN_ERROR7 |= 0x02u;
        g_u8LedFreq = FREQ_2HZ;
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
static void CPU_Exchange_Data( u8 buffer[], u8 len )
{   
    u32 i;
    
    /* communication buffer */
    comm_num = buffersize;  
    for(i = 0u; i < comm_num - 2u; i++)
    {
        SPIx_TX_Data[i] = 0u;
    }
    
    SPIx_TX_Data[0] = len;
    for( i = 0u; i < len; i++ )
    {
        SPIx_TX_Data[i + 1u] = buffer[i];
    }
    
#if 0    
    i = MB_CRC16( SPIx_TX_Data, comm_num - 2u );
    SPIx_TX_Data[comm_num - 2u] = (u8)i;
    SPIx_TX_Data[comm_num - 1u] = (u8)(i >> 8u);       
#else
    i = STM_CRC32( (u32*)&SPIx_TX_Data, (( comm_num - 4u ) >> 2u ) );
    SPIx_TX_Data[comm_num - 1u] = (u8)(i >> 24u);
    SPIx_TX_Data[comm_num - 2u] = (u8)(i >> 16u);
    SPIx_TX_Data[comm_num - 3u] = (u8)(i >> 8u);
    SPIx_TX_Data[comm_num - 4u] = (u8)i;

#endif
    
    SPIx_DMA_ReceiveSendByte(comm_num);
           
}


/******************************  END OF FILE  *********************************/