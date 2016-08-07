/*******************************************************************************
* File Name          : max31856.c
* Author             : lison
* Version            : V1.0
* Date               : 08/07/2016
* Description        : Contains the driver of max31856.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "max31856.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


#define SPI_MAX31856_SPI                           SPI2
#define SPI_MAX31856_SPI_CLK                       RCC_APB1Periph_SPI2

#define SPI_MAX31856_SPI_SCK_PIN                   GPIO_Pin_13                 /* PB.13 */
#define SPI_MAX31856_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SPI_MAX31856_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SPI_MAX31856_SPI_SCK_SOURCE                GPIO_PinSource13

#define SPI_MAX31856_SPI_MISO_PIN                  GPIO_Pin_14                 /* PB.14 */
#define SPI_MAX31856_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_MAX31856_SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SPI_MAX31856_SPI_MISO_SOURCE               GPIO_PinSource14

#define SPI_MAX31856_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PB.15 */
#define SPI_MAX31856_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SPI_MAX31856_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SPI_MAX31856_SPI_MOSI_SOURCE               GPIO_PinSource15

#define SPI_MAX31856_CS_PIN                        GPIO_Pin_9                 /* PA.9 */
#define SPI_MAX31856_CS_GPIO_PORT                  GPIOB                       /* GPIOA */
#define SPI_MAX31856_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOA

#define SPI_MAX31856_DR_PIN                        GPIO_Pin_8                  /* PA.8 */
#define SPI_MAX31856_DR_GPIO_PORT                  GPIOC                       /* GPIOA */
#define SPI_MAX31856_DR_GPIO_CLK                   RCC_AHBPeriph_GPIOA

/* Exported macro ------------------------------------------------------------*/
/* Select SPI MAX31856: Chip Select pin low  */
#define SPI_MAX31856_CS_LOW()       GPIO_ResetBits(SPI_MAX31856_CS_GPIO_PORT, SPI_MAX31856_CS_PIN)

/* Deselect SPI MAX31856: Chip Select pin high */
#define SPI_MAX31856_CS_HIGH()      GPIO_SetBits(SPI_MAX31856_CS_GPIO_PORT, SPI_MAX31856_CS_PIN)


/*******************************************************************************
* Function Name  : SPI_MAX31856_Init
* Description    : MAX31856 Init.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_MAX31856_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB1PeriphClockCmd(SPI_MAX31856_SPI_CLK, ENABLE);

  RCC_AHBPeriphClockCmd(SPI_MAX31856_SPI_MOSI_GPIO_CLK | SPI_MAX31856_SPI_MISO_GPIO_CLK |
                             SPI_MAX31856_SPI_SCK_GPIO_CLK,  ENABLE);
        
  RCC_APB1PeriphClockCmd(SPI_MAX31856_CS_GPIO_CLK, ENABLE);
  
        /*!< Configure SPI_MAX31856_SPI_CS_PIN pin: SPI_MAX31856 Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31856_CS_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(SPI_MAX31856_CS_GPIO_PORT, &GPIO_InitStructure);  

  GPIO_PinAFConfig(SPI_MAX31856_SPI_SCK_GPIO_PORT, SPI_MAX31856_SPI_SCK_SOURCE, GPIO_AF_5);
  GPIO_PinAFConfig(SPI_MAX31856_SPI_MISO_GPIO_PORT, SPI_MAX31856_SPI_MISO_SOURCE, GPIO_AF_5);
  GPIO_PinAFConfig(SPI_MAX31856_SPI_MOSI_GPIO_PORT, SPI_MAX31856_SPI_MOSI_SOURCE, GPIO_AF_5);
      
  /*!< Configure SPI_MAX31856_SPI pins: SCK */
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        
        /*!< Configure SPI_MAX31856_SPI pins: SCK */
        GPIO_InitStructure.GPIO_Pin = SPI_MAX31856_SPI_SCK_PIN;
  GPIO_Init(SPI_MAX31856_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_MAX31856_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31856_SPI_MISO_PIN;
  GPIO_Init(SPI_MAX31856_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_MAX31856_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31856_SPI_MOSI_PIN;
  GPIO_Init(SPI_MAX31856_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect the MAX31856: Chip Select high */
  SPI_MAX31856_CS_HIGH();
                
  /* SPI2 configuration */
  // SPI ADS1118: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;       
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                                                
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                                                                   
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                                                         
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                                                                  
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                                                                     
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;         
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                                                            
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI_MAX31856_SPI, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI_MAX31856_SPI, ENABLE);
        
        /* Configure DRDS */
  RCC_AHBPeriphClockCmd(SPI_MAX31856_DR_GPIO_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
  GPIO_InitStructure.GPIO_Pin = SPI_MAX31856_DR_PIN;
  GPIO_Init(SPI_MAX31856_DR_GPIO_PORT, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  : SPI_MAX31856_Send
* Description    : SPI send and receive data from MAX31856.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t SPI_MAX31856_Send(uint8_t dat)         
{
    SPI_MAX31856_CS_LOW();
        
          /* 当 SPI发送缓冲器非空时等待 */
    while (SPI_I2S_GetFlagStatus(SPI_MAX31856_SPI, SPI_I2S_FLAG_TXE) == RESET);
  
    /* 通过 SPI2发送一字节数据 */
    SPI_SendData8(SPI_MAX31856_SPI, dat);                

    /* 当SPI接收缓冲器为空时等待 */
//    while (SPI_I2S_GetFlagStatus(SPI_MAX31856_SPI, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_ReceiveData8(SPI_MAX31856_SPI);
        
          //SPI_MAX31856_CS_HIGH();
}

void SPI_MAX31856_SendEx(uint8_t dat)        
{
                /* 当 SPI发送缓冲器非空时等待 */
    while (SPI_I2S_GetFlagStatus(SPI_MAX31856_SPI, SPI_I2S_FLAG_TXE) == RESET);
  
    /* 通过 SPI2发送一字节数据 */
    SPI_SendData8(SPI_MAX31856_SPI, dat);        
}

/******************************  END OF FILE  *********************************/