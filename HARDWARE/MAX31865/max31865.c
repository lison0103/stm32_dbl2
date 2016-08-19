/*******************************************************************************
* File Name          : max31865.c
* Author             : lison
* Version            : V1.0
* Date               : 08/07/2016
* Description        : Contains the driver of max31865.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "max31865.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData);

#define SPI_MAX31865_SPI                           SPI2
#define SPI_MAX31865_SPI_CLK                       RCC_APB1Periph_SPI2

#define SPI_MAX31865_SPI_SCK_PIN                   GPIO_Pin_13                 /* PB.13 */
#define SPI_MAX31865_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SPI_MAX31865_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SPI_MAX31865_SPI_SCK_SOURCE                GPIO_PinSource13

#define SPI_MAX31865_SPI_MISO_PIN                  GPIO_Pin_14                 /* PB.14 */
#define SPI_MAX31865_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_MAX31865_SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SPI_MAX31865_SPI_MISO_SOURCE               GPIO_PinSource14

#define SPI_MAX31865_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PB.15 */
#define SPI_MAX31865_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_MAX31865_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SPI_MAX31865_SPI_MOSI_SOURCE               GPIO_PinSource15

#define SPI_MAX31865_CS_PIN                        GPIO_Pin_9                 /* PA.9 */
#define SPI_MAX31865_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
#define SPI_MAX31865_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOA

#define SPI_MAX31865_DR_PIN                        GPIO_Pin_8                  /* PA.8 */
#define SPI_MAX31865_DR_GPIO_PORT                  GPIOA                       /* GPIOA */
#define SPI_MAX31865_DR_GPIO_CLK                   RCC_AHBPeriph_GPIOA

/* Exported macro ------------------------------------------------------------*/
/* Select SPI MAX31865: Chip Select pin low  */
#define SPI_MAX31865_CS_LOW()       GPIO_ResetBits(SPI_MAX31865_CS_GPIO_PORT, SPI_MAX31865_CS_PIN)

/* Deselect SPI MAX31865: Chip Select pin high */
#define SPI_MAX31865_CS_HIGH()      GPIO_SetBits(SPI_MAX31865_CS_GPIO_PORT, SPI_MAX31865_CS_PIN)


/*******************************************************************************
* Function Name  : SPI_MAX31865_Init
* Description    : MAX31865 Init.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 SPI_MAX31865_Init(void)
{
#if 1
    
      GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* sEE_CS_GPIO, sEE_MOSI_GPIO, sEE_MISO_GPIO and sEE_SCK_GPIO 
     Periph clock enable */
  RCC_AHBPeriphClockCmd(SPI_MAX31865_CS_GPIO_CLK | SPI_MAX31865_SPI_MOSI_GPIO_CLK | SPI_MAX31865_SPI_MISO_GPIO_CLK |
                        SPI_MAX31865_SPI_SCK_GPIO_CLK , ENABLE);

  /* sEE Periph clock enable */
  RCC_APB1PeriphClockCmd(SPI_MAX31865_SPI_CLK, ENABLE); 

  /* Configure sEE pins: SCK */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SPI_MAX31865_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* Configure sEE pins: MISO */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_SPI_MISO_PIN;
  GPIO_Init(SPI_MAX31865_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* Configure sEE pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_SPI_MOSI_PIN;
  GPIO_Init(SPI_MAX31865_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Configure sEE_CS_PIN pin: sEE Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI_MAX31865_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Connect PXx to sEE_SCK */
  GPIO_PinAFConfig(SPI_MAX31865_SPI_SCK_GPIO_PORT, SPI_MAX31865_SPI_SCK_SOURCE, GPIO_AF_5);

  /* Connect PXx to sEE_MISO */
  GPIO_PinAFConfig(SPI_MAX31865_SPI_MISO_GPIO_PORT, SPI_MAX31865_SPI_MISO_SOURCE, GPIO_AF_5); 

  /* Connect PXx to sEE_MOSI */
  GPIO_PinAFConfig(SPI_MAX31865_SPI_MOSI_GPIO_PORT, SPI_MAX31865_SPI_MOSI_SOURCE, GPIO_AF_5);  
  
  SPI_MAX31865_CS_HIGH();
  
  /* sEE SPI Config */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_MAX31865_SPI, &SPI_InitStructure);

  SPI_RxFIFOThresholdConfig(SPI_MAX31865_SPI, SPI_RxFIFOThreshold_QF);

  
  SPI_I2S_ITConfig(SPI_MAX31865_SPI, SPI_I2S_IT_RXNE  , ENABLE);
NVIC_InitTypeDef NVIC_InitStructure;
 	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
        
  /* sEE_SPI enable */
  SPI_Cmd(SPI_MAX31865_SPI, ENABLE);
  
#else
  
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB1PeriphClockCmd(SPI_MAX31865_SPI_CLK, ENABLE);
    
    RCC_AHBPeriphClockCmd(SPI_MAX31865_SPI_MOSI_GPIO_CLK | SPI_MAX31865_SPI_MISO_GPIO_CLK |
                          SPI_MAX31865_SPI_SCK_GPIO_CLK,  ENABLE);
    
    RCC_APB1PeriphClockCmd(SPI_MAX31865_CS_GPIO_CLK, ENABLE);
    
    /*!< Configure SPI_MAX31865_SPI_CS_PIN pin: SPI_MAX31865 Card CS pin */
    GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(SPI_MAX31865_CS_GPIO_PORT, &GPIO_InitStructure);  
    
    GPIO_PinAFConfig(SPI_MAX31865_SPI_SCK_GPIO_PORT, SPI_MAX31865_SPI_SCK_SOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(SPI_MAX31865_SPI_MISO_GPIO_PORT, SPI_MAX31865_SPI_MISO_SOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(SPI_MAX31865_SPI_MOSI_GPIO_PORT, SPI_MAX31865_SPI_MOSI_SOURCE, GPIO_AF_5);
    
    /*!< Configure SPI_MAX31865_SPI pins: SCK */
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    
    /*!< Configure SPI_MAX31865_SPI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_SPI_SCK_PIN;
    GPIO_Init(SPI_MAX31865_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /*!< Configure SPI_MAX31865_SPI pins: MISO */
    GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_SPI_MISO_PIN;
    GPIO_Init(SPI_MAX31865_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    /*!< Configure SPI_MAX31865_SPI pins: MOSI */
    GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_SPI_MOSI_PIN;
    GPIO_Init(SPI_MAX31865_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure DRDS */
    RCC_AHBPeriphClockCmd(SPI_MAX31865_DR_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
    GPIO_InitStructure.GPIO_Pin = SPI_MAX31865_DR_PIN;
    GPIO_Init(SPI_MAX31865_DR_GPIO_PORT, &GPIO_InitStructure);
    
    /* Deselect the MAX31865: Chip Select high */
    SPI_MAX31865_CS_HIGH();
    
    /* SPI2 configuration */
    /* SPI ADS1118: data input on the DIO pin is sampled on the rising edge of the CLK. 
    Data on the DO and DIO pins are clocked out on the falling edge of CLK.*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;       
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                                                
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                                                                   
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                                                         
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                                                                                  
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                                                                     
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;         
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                                                            
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI_MAX31865_SPI, &SPI_InitStructure);

    SPI_RxFIFOThresholdConfig(SPI_MAX31865_SPI, SPI_RxFIFOThreshold_QF);
    
    /* Enable SPI1  */
    SPI_Cmd(SPI_MAX31865_SPI, ENABLE);
#endif
    
    /* wait for MAX31865 to set up */
    delay_ms(200);
    
//    SPI_ReadWriteByte(SPI_MAX31865_SPI, 0xff);
    u16 read_value = 0;
//    SPI_MAX31865_CS_LOW();
    while(read_value != 0xc2)
    {
        SPI_MAX31865_Write( 0x80,0xc2 );
        
        delay_ms(10);
        
        read_value = SPI_MAX31865_Read(0x03);
        
        delay_ms(10);
        
    }
    
    return 1;

}

void SPI2_IRQHandler(void)
{
    u16 value = 0;
    if(SPI_I2S_GetITStatus(SPI_MAX31865_SPI, SPI_I2S_IT_RXNE) != RESET)
    {
        value = SPI2->DR;
    }
}

/**
  * @brief  Reads a byte from the SPI EEPROM.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI EEPROM.
  */
//uint8_t sEE_ReadByte(void)
//{
//  return (sEE_SendByte(0x00));
//}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint16_t sEE_SendByte(uint16_t value)
{
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI peripheral */
  SPI2->DR = value;
  

  /*!< Wait to receive a byte */
//  while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_RXNE) == RESET);
//
//  /*!< Return the byte read from the SPI bus */
//  return SPI2->DR;
}


/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 SPI_ReadWriteByte(SPI_TypeDef* SPIx,u8 TxData)
{		
	u16 retry=0;				 
	while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_TXE) == RESET)		
	{
		retry++;
		if(retry>=0XFFFE)
                {
                    return 0; 	
                }
	}			  
	SPI_SendData8(SPI_MAX31865_SPI, TxData);
        
	retry=0;
        
	while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_RXNE) == RESET) 		
	{
		retry++;
		if(retry>=0XFFFE)
                {
                    return 0;	
                }
	}	
	return SPI_ReceiveData8(SPI_MAX31865_SPI);          						    
}

/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 TxData)
{		
	u16 retry=0;				 
	while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_TXE) == RESET)		
	{
		retry++;
		if(retry>=0XFFFE)
                {
                    return 0; 	
                }
	}			  
	SPI_SendData8(SPI_MAX31865_SPI, TxData);	
        
        
	  						    
	return 1;          						    
}

/*******************************************************************************
* Function Name  : SPI_MAX31865_Write
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_MAX31865_Write(u8 address, u8 value)         
{
    SPI_MAX31865_CS_LOW();   
    sEE_SendByte(0x80c2);                   
//    sEE_SendByte(value); 
    SPI_MAX31865_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_MAX31865_Read
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t SPI_MAX31865_Read(u8 address)         
{
    u16 value = 0;
    
    SPI_MAX31865_CS_LOW();  
//    sEE_SendByte(address);                   
    value = sEE_SendByte(0x0000);   
    SPI_MAX31865_CS_HIGH();
    
    return value;
}



/******************************  END OF FILE  *********************************/