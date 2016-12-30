/*******************************************************************************
* File Name          : max31865.c
* Author             : lison
* Version            : V1.0
* Date               : 08/07/2016
* Last modify date   : 10/27/2016
* Description        : Contains the driver of max31865.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "max31865.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/**
 * Configuration of the MAX31865 from MSB to LSB:
 * BIT      FUNCTION            ASSIGNMENT
 *  7       VBIAS               0=OFF            1=ON
 *  6       Conversion Mode     0=Normally OFF   1=AUTO
 *  5       1-Shot              0= -             1=1-Shot
 *  4       3-Wire              0=2- or 4-Wire   1=3-wire
 * 3,2      Faultdetection      set both to 0
 *  1       Fault Status Clear  set to 1
 *  0       50/60Hz filter      0=60Hz           1=50Hz
 */
#define Configuration  0x80u 						
#define Read_Configuration  0x00u 
#define Read_RTD_MSB  0x01u 
#define Read_RTD_LSB  0x02u 
#define Write_High_Fault_Threshold_MSB  0x83u 
#define Write_High_Fault_Threshold_LSB  0x84u 
#define Read_High_Fault_Threshold_MSB  0x03u 
#define Read_High_Fault_Threshold_LSB  0x04u 
#define Write_Low_Fault_Threshold_MSB  0x85u 
#define Write_Low_Fault_Threshold_LSB  0x86u 
#define Read_Low_Fault_Threshold_MSB  0x05u 
#define Read_Low_Fault_Threshold_LSB  0x06u 
#define Fault_Status  0x07u 

/* Private macro -------------------------------------------------------------*/
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

#define SPI_MAX31865_CS_LOW()       GPIO_ResetBits(SPI_MAX31865_CS_GPIO_PORT, SPI_MAX31865_CS_PIN)
#define SPI_MAX31865_CS_HIGH()      GPIO_SetBits(SPI_MAX31865_CS_GPIO_PORT, SPI_MAX31865_CS_PIN)

/* Reference Resistor */
#define  Reference_Resistor     1000u
/* RTD Resistance at 0 Degrees */
#define  RTD_Resistance         100u

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static u8 SPI_ReadWriteByte(u8 TxData);
static void SPI_MAX31865_Write(u8 address, u8 value);
static u8 SPI_MAX31865_Read(u8 address);
static u8 get_Temperature(void);
static u8 max31865_setup(void);


/*******************************************************************************
* Function Name  : SPI_MAX31865_Init
* Description    : MAX31865 Init.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 SPI_MAX31865_Init(void)
{
    u8 result = 0u;
    
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
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;       
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                                                                
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                                                                   
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                                                         
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                                                                  
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                                                                     
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;         
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                                                            
    SPI_InitStructure.SPI_CRCPolynomial = 7u;
    SPI_Init(SPI_MAX31865_SPI, &SPI_InitStructure);

    SPI_RxFIFOThresholdConfig(SPI_MAX31865_SPI, SPI_RxFIFOThreshold_QF);
    
    /* Enable SPI  */
    SPI_Cmd(SPI_MAX31865_SPI, ENABLE);
    

    result = max31865_setup();
    
    return result;
}



/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u8 SPI_ReadWriteByte(u8 TxData)
{		
    u16 retry = 0u;
    u8 error,result = 0u;				 
    while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_TXE) == RESET)		
    {
        retry++;
        if(retry >= 0XFFFEu)
        {
            error = 1u; 	
        }
    }	
    
    if( !error )
    {
        SPI_SendData8(SPI_MAX31865_SPI, TxData);
        
        retry=0u;
        
        while (SPI_I2S_GetFlagStatus(SPI_MAX31865_SPI, SPI_I2S_FLAG_RXNE) == RESET) 		
        {
            retry++;
            if(retry>=0XFFFEu)
            {
                error = 1u; 	
            }
        }	
    }
    
    if( error == 1u )
    {
        result = 0u;
    }
    else
    {
        result = SPI_ReceiveData8(SPI_MAX31865_SPI); 
    }
    
    return result;         						    
}


/*******************************************************************************
* Function Name  : SPI_MAX31865_Write
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SPI_MAX31865_Write(u8 address, u8 value)         
{
    SPI_MAX31865_CS_LOW();   
    SPI_ReadWriteByte(address);                   
    SPI_ReadWriteByte(value); 
    SPI_MAX31865_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_MAX31865_Read
* Description    : SPI send and receive data from MAX31865.                  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u8 SPI_MAX31865_Read(u8 address)         
{
    u8 value = 0u;
    
    SPI_MAX31865_CS_LOW();  
    SPI_ReadWriteByte(address);                   
    value = SPI_ReadWriteByte(0x00u);   
    SPI_MAX31865_CS_HIGH();
    
    return value;
}


/*******************************************************************************
* Function Name  : max31865_setup
* Description    :                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u8 max31865_setup(void) 
{    
    u8 result = 0u;
    
    SPI_MAX31865_Write(Configuration, 0xd1u);            
    result = SPI_MAX31865_Read(Read_Configuration);            
    if (result == 0xd1u) 
    {
        /* Communication successful with max31865 */
        SPI_MAX31865_Write(Write_High_Fault_Threshold_MSB, 0xffu);    
        SPI_MAX31865_Write(Write_High_Fault_Threshold_LSB, 0xffu);    
        SPI_MAX31865_Write(Write_Low_Fault_Threshold_MSB, 0x00u);        
        SPI_MAX31865_Write(Write_Low_Fault_Threshold_LSB, 0x00u);          
        
        result = 0u;
    } 
    else 
    {
        /* Unable to communicate with the device. Please check your connections and try again */
        result = 1u;
    }
    
    return result;
}

/*******************************************************************************
* Function Name  : get_Temperature
* Description    :                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u8 get_Temperature(void) 
{
    u8 lsb_rtd,msb_rtd,fault_test;
    u16 RTD = 0u;
    u8 temperature = 0u;
    
    lsb_rtd = SPI_MAX31865_Read(Read_RTD_LSB);
    fault_test = lsb_rtd & 0x01u; 
    if( fault_test == 0u ) 								
    {
        msb_rtd = SPI_MAX31865_Read(Read_RTD_MSB);					
        RTD = (((u16)msb_rtd << 7u) + (((u16)lsb_rtd & 0xFEu) >> 1u)); 
        
        temperature = (u8)( RTD / 32u );
    }
    
    return temperature;
}

/*******************************************************************************
* Function Name  : Get_RTD_Value
* Description    :                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 Get_RTD_Value(void) 
{ 
    u8 value = 0u;
    u8 Fault_Error = 0u;
    
    /* for test */
    u8 temp[8] = { 0u };
    temp[0] = SPI_MAX31865_Read(Read_Configuration);
    temp[1] = SPI_MAX31865_Read(Read_RTD_MSB);
    temp[2] = SPI_MAX31865_Read(Read_RTD_LSB);
    temp[3] = SPI_MAX31865_Read(Read_High_Fault_Threshold_MSB);
    temp[4] = SPI_MAX31865_Read(Read_High_Fault_Threshold_LSB);
    temp[5] = SPI_MAX31865_Read(Read_Low_Fault_Threshold_MSB);
    temp[6] = SPI_MAX31865_Read(Read_Low_Fault_Threshold_LSB);
    temp[7] = SPI_MAX31865_Read(Fault_Status);
    
    Fault_Error = SPI_MAX31865_Read(Fault_Status);       
    if( Fault_Error == 0u ) 
    {     
        value = get_Temperature();
    } 
    else 
    {
        /* clear fault */
        SPI_MAX31865_Write(Configuration, 0x82u);
        delay_us(700u); 
        SPI_MAX31865_Write(Configuration, 0xd1u);
    }  
    
    return value;
}

/******************************  END OF FILE  *********************************/