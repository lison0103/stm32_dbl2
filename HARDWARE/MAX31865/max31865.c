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
u8 SPI_ReadWriteByte(u8 TxData);

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




/**********************************************************
* 函数功能 ---> 模拟SPI初始化GPIO
* 入口参数 ---> none
* 返回参数 ---> none
* 功能说明 ---> none 
**********************************************************/
void Soft_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	 //使能PA端口时钟   
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	 //使能PA端口时钟 
    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //CLK and MOSI
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化指定IO，CLK and MOSI
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    
 	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化指定IO，CLK and MOSI
 	


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;   //MISO
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化指定IO，MISO

    GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);    //设置低电平
}


#define Soft_SPI_MISO_IN()          {GPIOB->MODER &= 0xcfffffff;GPIOB->PUPDR &= 0xcfffffff;GPIOB->PUPDR |= 1<<28;}
/**********************************************************
* 函数功能 ---> 模拟SPI发送数据函数
* 入口参数 ---> send_data: 要发送的数据
* 返回参数 ---> 接收到的数据
* 功能说明 ---> 优先发送MSB，需要先发送LSB的请修改此函数
**********************************************************/
u8 Soft_SPI_ReadWrite_Byte(u8 send_data)
{
    u8 i;
    u8 rcv;

    GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
//    Soft_SPI_MISO_IN(); //设置MISO为输入模式

    for(i = 0;i < 8;i++)
    {
        if(send_data & 0x80)    GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);  //放上数据
        else    GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);

        send_data <<= 1;
        rcv <<= 1;

        GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);   //上升沿发送数据

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))   
            rcv |= 0x01;  //返回来的是"1"
        
//        Soft_SPI_CLK = 0;
    }
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
//    return send_data;   //返回接收到的数据
    return rcv;
}


#define Set_Max_CS(a)   if(a) \
                        GPIO_SetBits(GPIOA,GPIO_Pin_9); \
                        else \
                        GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define Set_Max_CLK(a)  if(a) \
                        GPIO_SetBits(GPIOB,GPIO_Pin_13); \
                        else \
                        GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define Set_Max_Din(a)  if(a) \
                        GPIO_SetBits(GPIOB,GPIO_Pin_15); \
                        else \
                        GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define Read_InData()   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
                            
#define   ON   1
#define   OFF  0
uint8_t Send_Read_Byte(uint8_t Address);
void Send_Write_Byte(uint8_t Address,uint8_t Data);                            
//====================================================================
//===MAX31865初始化===//
void Temp_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA| \
                              RCC_AHBPeriph_GPIOB,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_9);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//定义GPIOA_15脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;    //输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

//    Send_Write_Byte(0x80,0xc1);//初始化
    
    u8 read_value = 1;

    while(read_value != 0xc1)
    {
        Send_Write_Byte( 0x80,0xc1 );
//        
//        delay_us(50);
        
        read_value = Send_Read_Byte(0x00);
        
        delay_ms(10);
        
    }    
}
//====================================================================
//===写数据===//
void Send_Write_Byte(uint8_t Address,uint8_t Data)
{
    uint8_t i;
    Set_Max_CLK(ON);
    Set_Max_CS(OFF);
    for(i = 0;i < 8;i++)
    {        
        if((Address & 0x80) == 0x80)
        {
            Set_Max_Din(ON);
        }
        else
        {
            Set_Max_Din(OFF);
        }
        Set_Max_CLK(OFF);
        Set_Max_CLK(ON);
        Address <<= 1;
    }
    for(i = 0;i < 8;i++)
    {
        
        if((Data & 0x80) == 0x80)
        {
            Set_Max_Din(ON);
        }
        else
        {
            Set_Max_Din(OFF);
        }
        Set_Max_CLK(OFF);
        Set_Max_CLK(ON);
        Data <<= 1;
    }
    Set_Max_CLK(ON);
    Set_Max_CS(ON);
}
//====================================================================
//===读数据===//
uint8_t Send_Read_Byte(uint8_t Address)
{
    uint8_t i,Temp=0;
    Set_Max_CLK(ON);
    Set_Max_CS(OFF);
    for(i = 0;i < 8;i++)
    {
        
        if((Address & 0x80) == 0x80)
        {
            Set_Max_Din(ON);
        }
        else
        {
            Set_Max_Din(OFF);
        }
        Set_Max_CLK(OFF);
        Set_Max_CLK(ON);
        Address <<= 1;
    }
    for(i = 0;i < 8;i++)
    {
        Temp <<= 1;
        if(Read_InData() != RESET)
        {
            Temp |= 0x01;
        }
        else
        {
            Temp &= 0xfe;
        }
        Set_Max_CLK(OFF);
        Set_Max_CLK(ON);
    }
    Set_Max_CLK(ON);
    Set_Max_CS(ON);
    return Temp;
}


/*******************************************************************************
* Function Name  : SPI_MAX31865_Init
* Description    : MAX31865 Init.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 SPI_MAX31865_Init(void)
{

  
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

    /* Deselect the MAX31865: Chip Select high */
    SPI_MAX31865_CS_HIGH();

    
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
    
    
    GPIO_PinAFConfig(SPI_MAX31865_SPI_SCK_GPIO_PORT, SPI_MAX31865_SPI_SCK_SOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(SPI_MAX31865_SPI_MISO_GPIO_PORT, SPI_MAX31865_SPI_MISO_SOURCE, GPIO_AF_5);
    GPIO_PinAFConfig(SPI_MAX31865_SPI_MOSI_GPIO_PORT, SPI_MAX31865_SPI_MOSI_SOURCE, GPIO_AF_5);    
    

    
    /* SPI2 configuration */
    /* SPI ADS1118: data input on the DIO pin is sampled on the rising edge of the CLK. 
    Data on the DO and DIO pins are clocked out on the falling edge of CLK.*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;       
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                                                
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                                                                   
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                                                         
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                                                                  
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                                                                     
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;         
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                                                            
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI_MAX31865_SPI, &SPI_InitStructure);

    SPI_RxFIFOThresholdConfig(SPI_MAX31865_SPI, SPI_RxFIFOThreshold_QF);
    
    /* Enable SPI1  */
    SPI_Cmd(SPI_MAX31865_SPI, ENABLE);
//    Soft_SPI_Init();
//    SPI_MAX31865_CS_HIGH();
    
    /* wait for MAX31865 to set up */
    delay_ms(200);
    
//    Temp_GPIO_Init();

    u8 read_value = 1;

    while(read_value != 0xc2)
    {
        SPI_MAX31865_Write( 0x80,0xc2 );
        
        delay_us(50);
        
        read_value = SPI_MAX31865_Read(0x00);
        
        delay_ms(10);
        
    }
    
    return 1;

}



/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t sEE_SendByte(uint8_t value)
{
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI peripheral */
  SPI_SendData8(SPI_MAX31865_SPI, value);
  

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_ReceiveData8(SPI_MAX31865_SPI);
}


/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 SPI_ReadWriteByte(u8 TxData)
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
* Function Name  : SPI_MAX31865_Write
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_MAX31865_Write(u8 address, u8 value)         
{
    SPI_MAX31865_CS_LOW();   
    sEE_SendByte(address);                   
    sEE_SendByte(value); 
    SPI_MAX31865_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_MAX31865_Read
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t SPI_MAX31865_Read(u8 address)         
{
    u8 value = 0;
    
    SPI_MAX31865_CS_LOW();  
    sEE_SendByte(address);                   
    value = sEE_SendByte(0x00);   
    SPI_MAX31865_CS_HIGH();
    
    return value;
}



/******************************  END OF FILE  *********************************/