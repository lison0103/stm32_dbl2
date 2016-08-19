/*******************************************************************************
* File Name          : spi.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : The file contains spi driver functions.
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "delay.h"
#include "ewdt.h"
#include "esc_error_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SPI1_DR_Addr ( (u32)0x4001300C )
#define SPI2_DR_Addr ( (u32)0x4000380C )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void SPIx_DMA_Configuration( void );

u8 SPIx_TX_Buff[buffersize] = { 0 };
u8 SPIx_RX_Buff[buffersize] = { 0 };
u8 SPIx_TX_Data[buffersize] = { 0 };
u8 SPIx_RX_Data[buffersize] = { 0 };
DMA_InitTypeDef     DMA_InitStructure;
static u16 waitus = 0;



/*******************************************************************************
* Function Name  : SPIx_Configuration
* Description    : Configure spix register.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void SPIx_Configuration(SPI_TypeDef* SPIx)
{       
        SPI_InitTypeDef  SPI_InitStructure;	
        
        SPI_I2S_DeInit(SPIx);

        /* Setting unidirectional or bidirectional SPI data mode: SPI is set to double two-way full-duplex */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      
#ifdef GEC_DBL2_MASTER
        /* Setting SPI mode: setting master SPI */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                
#else
        /* Setting SPI mode: setting slave SPI */
        SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
#endif
        /* SPI data set size: SPI transmission and reception of 8-bit frame structure */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		        
        /* Select the serial clock Steady: Clock floating high */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		              
        /* Data capture on the second clock edge */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        
        /* NSS signal by hardware (NSS pin) or software (using SSI bit) management: internal NSS-bit control signal has SSI */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                
        /* Defines the baud rate prescaler values: Baud Rate Prescaler is 32, speed is about 72M / 32 = 2.25M / s */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;	
        /* Specifies the data transmission from the MSB or LSB bit first: data transmission start from the MSB */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        /* CRC polynomial value calculation */
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        
	SPI_Init(SPIx, &SPI_InitStructure);   
        
#ifdef GEC_DBL2_SLAVE 
        SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
#endif
        
        //DMA 
        SPIx_DMA_Configuration();       
        SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
        SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
           
        //CRC
        SPI_CalculateCRC(SPIx, ENABLE);
        
        //SPI enable
	SPI_Cmd(SPIx, ENABLE); 
        
} 


/*******************************************************************************
* Function Name  : SPIx_Init
* Description    : Intialization SPI.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void SPIx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;        	

#ifdef GEC_DBL2_MASTER        
        
        /* SPI2 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
        GPIO_Init(GPIOB, &GPIO_InitStructure);
             
        SPIx_Configuration(SPI2);
#else
        /* SPI1 */      
        /* SPI pin mappings */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
        
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        
        /* SPI SCK pin configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    
        SPIx_Configuration(SPI1);
#endif           

}    


/*******************************************************************************
* Function Name  : SPIx_DMA_Configuration
* Description    : Configuring SPI1_RX DMA channel 2, SPI1_TX DMA channel 3 for slave
*                  Configuring SPI2_RX DMA channel 4, SPI2_TX DMA channel 5 for master.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPIx_DMA_Configuration( void )
{  
      
#ifdef GEC_DBL2_MASTER
    
      DMA_DeInit(DMA1_Channel4);
      DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_DR_Addr;
      /* Set the DMA memory address */
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPIx_RX_Buff;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
      /* A transmission number is set to buffersize */
      DMA_InitStructure.DMA_BufferSize = buffersize; 
      
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
      DMA_Init(DMA1_Channel4, &DMA_InitStructure);  
      
      
      DMA_DeInit(DMA1_Channel5);
      DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_DR_Addr;
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPIx_TX_Buff;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
      DMA_InitStructure.DMA_BufferSize = buffersize; 
      
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
      DMA_Init(DMA1_Channel5, &DMA_InitStructure);    
    
    
#else
      DMA_DeInit(DMA1_Channel2);
      DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Addr;
      /* Set the DMA memory address */
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPIx_RX_Buff;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
      /* A transmission number is set to buffersize */
      DMA_InitStructure.DMA_BufferSize = buffersize; 
      
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
      DMA_Init(DMA1_Channel2, &DMA_InitStructure);  
      
      
      DMA_DeInit(DMA1_Channel3);
      DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Addr;
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPIx_TX_Buff;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
      DMA_InitStructure.DMA_BufferSize = buffersize; 
      
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
      DMA_Init(DMA1_Channel3, &DMA_InitStructure);    
#endif       
  
}

/*******************************************************************************
* Function Name  : SPI1_DMA_ReceiveSendByte
* Description    : Enable SPI1 DMA send and receive data.
* Input          : num: the number of send and receive data.
* Output         : None
* Return         : None
*******************************************************************************/
void SPIx_DMA_ReceiveSendByte( u16 num )
{
      
    /* copy data to buff */
    for(u16 i = 0; i < num; i++)
    {
        SPIx_TX_Buff[i] = SPIx_TX_Data[i];
    }    

#ifdef GEC_DBL2_MASTER
    
    waitus = 0;
    while( ( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < 2000 ) )
    {
        waitus++;
        delay_us(1);
    }
    
    if( waitus >= 2000 )
    {
        /* TXE timeout! */
    }
      
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);      
    
    DMA1_Channel4->CNDTR = 0x0000;	
    DMA1_Channel4->CNDTR = num;
    DMA1_Channel5->CNDTR = 0x0000;	
    DMA1_Channel5->CNDTR = num;      
    
    
    DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
    DMA_ClearFlag(DMA1_FLAG_GL4|DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);
      
    /* Read once before the shuttle SPI1-> DR, ensure that the receiving buffer is empty */
    SPI2->DR ;						
      

    waitus = 0;
    while( ( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < 2000 ) )
    {
        waitus++;
        delay_us(1);
    }
    
    if( waitus >= 2000 )
    {
        /* TXE timeout! */
    }
    
    DMA_Cmd(DMA1_Channel4, ENABLE);    
    DMA_Cmd(DMA1_Channel5, ENABLE);

#else
    
    waitus = 0;
    while( ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < 2000 ) )
    {
        waitus++;
        delay_us(1);
    }
    
    if( waitus >= 2000 )
    {
        printf("1 TXE timeout! \r\n");
    }
      
    DMA_Cmd(DMA1_Channel2, DISABLE);
    DMA_Cmd(DMA1_Channel3, DISABLE);      
    
    DMA1_Channel2->CNDTR = 0x0000;	
    DMA1_Channel2->CNDTR = num;
    DMA1_Channel3->CNDTR = 0x0000;	
    DMA1_Channel3->CNDTR = num;      
    
    
    DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
    DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);
      
    /* Read once before the shuttle SPI1-> DR, ensure that the receiving buffer is empty */
    SPI1->DR ;						
      

    waitus = 0;
    while( ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < 2000 ) )
    {
        waitus++;
        delay_us(1);
    }
    
    if( waitus >= 2000 )
    {
        printf("2 TXE timeout! \r\n");
    }
    
    DMA_Cmd(DMA1_Channel2, ENABLE);    
    DMA_Cmd(DMA1_Channel3, ENABLE);    
    
#endif
    
}

/*******************************************************************************
* Function Name  : DMA_Check_Flag
* Description    : Check the DMA flag register.
*                  Judging whether data transmission and reception is completed
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_Check_Flag(u32 times)
{         

#ifdef GEC_DBL2_MASTER
    
          waitus = 0;
          /* 10us */
          while( ( !DMA_GetFlagStatus(DMA1_IT_TC4) ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
              EWDT_TOOGLE();
              IWDG_ReloadCounter();  
          }
          
          if( waitus >= times )
          {
              printf("DMA1_IT_TC2 wait timeout!!! \r\n");
          }
          waitus = 0;
          while( ( !DMA_GetFlagStatus(DMA1_IT_TC5) ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= times )
          {
              printf("DMA1_IT_TC3 wait timeout!!! \r\n");
          }
          waitus = 0;
          while( ( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= times )
          {             
              printf("SPI_I2S_FLAG_TXE wait timeout!!! \r\n");
          }
          waitus = 0;
          while( ( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= times )
          {              
              printf("SPI_I2S_FLAG_BSY wait timeout!!! \r\n");
          } 
 
        DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);
        DMA_Cmd(DMA1_Channel5, DISABLE); 
        DMA_ClearFlag(DMA1_FLAG_GL4|DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);        
        DMA_Cmd(DMA1_Channel4, DISABLE);                 
              
          if( SPI_I2S_GetFlagStatus(SPI2, SPI_FLAG_CRCERR) != RESET)
          {
              SPI_I2S_ClearFlag(SPI2, SPI_FLAG_CRCERR);
              
              /* SPI CRC ERROR */
              /* Channel4 CRCERR */
              EN_ERROR_SYS4++;
             
              if(EN_ERROR_SYS4 > 2)
              {
                ESC_SPI_Error_Process();
              }
          }
          else
          {
              EN_ERROR_SYS4 = 0;
          }      
          
#else
          
          waitus = 0;
          /* 10us */
          while( ( !DMA_GetFlagStatus(DMA1_IT_TC2) ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
              EWDT_TOOGLE();
              IWDG_ReloadCounter();  
          }
          
          if( waitus >= times )
          {
              printf("DMA1_IT_TC2 wait timeout!!! \r\n");
          }
          waitus = 0;
          while( ( !DMA_GetFlagStatus(DMA1_IT_TC3) ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= times )
          {
              printf("DMA1_IT_TC3 wait timeout!!! \r\n");
          }
          waitus = 0;
          while( ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= times )
          {             
              printf("SPI_I2S_FLAG_TXE wait timeout!!! \r\n");
          }
          waitus = 0;
          while( ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET ) && ( waitus < times ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= times )
          {              
              printf("SPI_I2S_FLAG_BSY wait timeout!!! \r\n");
          } 
 
        DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
        DMA_Cmd(DMA1_Channel3, DISABLE); 
        DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);        
        DMA_Cmd(DMA1_Channel2, DISABLE);                 
              
          if( SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_CRCERR) != RESET)
          {
              SPI_I2S_ClearFlag(SPI1, SPI_FLAG_CRCERR);
              
              /* SPI CRC ERROR */
              /* Channel2 CRCERR */
              EN_ERROR_SYS4++;

              SPIx_Configuration(SPI1);
            
              if(EN_ERROR_SYS4 > 2)
              {
                ESC_SPI_Error_Process();
              }
          }
          else
          {
              EN_ERROR_SYS4 = 0;
          }           
          
#endif
        
        /* copy buff to data */
        for(u16 i = 0; i < buffersize; i++)
        {
            SPIx_RX_Data[i] = SPIx_RX_Buff[i];
        }         
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Stream 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{

      if ( ( DMA_GetITStatus( DMA1_IT_TC2 ) ) != RESET )
      {
          DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);         
          DMA_Cmd(DMA1_Channel2, DISABLE);
 
      }

}
/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Stream 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
      if ( ( DMA_GetITStatus( DMA1_IT_TC3 ) ) != RESET )
      {
          
          waitus = 0;
          while( ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) && ( waitus < 2000 ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= 2000 )
          {
              printf("Channel3 TXE timeout! \r\n");
          }
          waitus = 0;
          while( ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET ) && ( waitus < 2000 ) )
          {
              waitus++;
              delay_us(1);
          }
          
          if( waitus >= 2000 )
          {
              printf("Channel3 BSY timeout! \r\n");
          }          
          
          DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
          
          DMA_Cmd(DMA1_Channel3, DISABLE);                               
      }

}



/******************************  END OF FILE  *********************************/
















