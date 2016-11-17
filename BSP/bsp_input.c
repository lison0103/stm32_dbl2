/*******************************************************************************
* File Name          : bsp_input.c
* Author             : lison
* Version            : V1.0
* Date               : 06/16/2016
* Description        : Contains get the input pin value function.
*                      
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "bsp_input.h"
#include "bsp_iocfg.h"
#include "esc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : Get_GpioInput
* Description    : Get GPIO input pin value.
*                  
* Input          : inBuff: read the gpio pin value of data
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void Get_GpioInput(u8 inBuff[])
{     

    static u16 PinValue[3][5],ByteAnd[5],ByteOr[5],read_pin_cnt = 0;
    u16 i; 
    
    /* first, clear the data */
    for(i = 0u; i < 4u; i++)
    {
        inBuff[i] = 0;
    }
    
    read_pin_cnt++;  
    if(read_pin_cnt > 2) read_pin_cnt = 0;  
    
    /* Read I/O port */                      
    PinValue[read_pin_cnt][0] = GPIO_ReadInputData(GPIOA);
    PinValue[read_pin_cnt][1] = GPIO_ReadInputData(GPIOB);
    PinValue[read_pin_cnt][2] = GPIO_ReadInputData(GPIOC);
    PinValue[read_pin_cnt][3] = GPIO_ReadInputData(GPIOD);
    PinValue[read_pin_cnt][4] = GPIO_ReadInputData(GPIOE);
    
    for(i=0;i<5;i++)
    {   
        ByteAnd[i] = PinValue[0][i] & PinValue[1][i] & PinValue[2][i];
        ByteOr[i] = PinValue[0][i] | PinValue[1][i] | PinValue[2][i]; 
    }
    
#ifdef GEC_DBL2_MASTER
    
    /* INA1 ~ INA8 */
    if(ByteAnd[1] & 0x0040) inBuff[0] |= 0x01; else if(!(ByteOr[1] & 0x0040)) inBuff[0] &= ~0x01; 
    if(ByteAnd[1] & 0x0020) inBuff[0] |= 0x02; else if(!(ByteOr[1] & 0x0020)) inBuff[0] &= ~0x02; 
    if(ByteAnd[1] & 0x0010) inBuff[0] |= 0x04; else if(!(ByteOr[1] & 0x0010)) inBuff[0] &= ~0x04; 
    if(ByteAnd[1] & 0x0008) inBuff[0] |= 0x08; else if(!(ByteOr[1] & 0x0008)) inBuff[0] &= ~0x08; 
    if(ByteAnd[3] & 0x0080) inBuff[0] |= 0x10; else if(!(ByteOr[3] & 0x0080)) inBuff[0] &= ~0x10; 
    if(ByteAnd[3] & 0x0040) inBuff[0] |= 0x20; else if(!(ByteOr[3] & 0x0040)) inBuff[0] &= ~0x20; 
    if(ByteAnd[3] & 0x0020) inBuff[0] |= 0x40; else if(!(ByteOr[3] & 0x0020)) inBuff[0] &= ~0x40; 
    if(ByteAnd[3] & 0x0010) inBuff[0] |= 0x80; else if(!(ByteOr[3] & 0x0010)) inBuff[0] &= ~0x80; 
    
    /* INA9 ~ INA14 */
    if(ByteAnd[3] & 0x0008) inBuff[1] |= 0x01; else if(!(ByteOr[3] & 0x0008)) inBuff[1] &= ~0x01; 
    if(ByteAnd[3] & 0x0004) inBuff[1] |= 0x02; else if(!(ByteOr[3] & 0x0004)) inBuff[1] &= ~0x02; 
    if(ByteAnd[3] & 0x0002) inBuff[1] |= 0x04; else if(!(ByteOr[3] & 0x0002)) inBuff[1] &= ~0x04; 
    if(ByteAnd[3] & 0x0001) inBuff[1] |= 0x08; else if(!(ByteOr[3] & 0x0001)) inBuff[1] &= ~0x08; 
    if(ByteAnd[2] & 0x1000) inBuff[1] |= 0x10; else if(!(ByteOr[2] & 0x1000)) inBuff[1] &= ~0x10; 
    if(ByteAnd[2] & 0x0800) inBuff[1] |= 0x20; else if(!(ByteOr[2] & 0x0800)) inBuff[1] &= ~0x20;
    /* SFSW_INA1 ~ SFSW_INA2 */ 
    if(ByteAnd[0] & 0x0010) inBuff[1] |= 0x40; else if(!(ByteOr[0] & 0x0010)) inBuff[1] &= ~0x40; 
    if(ByteAnd[0] & 0x0020) inBuff[1] |= 0x80; else if(!(ByteOr[0] & 0x0020)) inBuff[1] &= ~0x80; 
    
    /* SFSW_INA3 ~ SFSW_INA10 */ 
    if(ByteAnd[0] & 0x0040) inBuff[2] |= 0x01; else if(!(ByteOr[0] & 0x0040)) inBuff[2] &= ~0x01; 
    if(ByteAnd[0] & 0x0080) inBuff[2] |= 0x02; else if(!(ByteOr[0] & 0x0080)) inBuff[2] &= ~0x02; 
    if(ByteAnd[2] & 0x0010) inBuff[2] |= 0x04; else if(!(ByteOr[2] & 0x0010)) inBuff[2] &= ~0x04; 
    if(ByteAnd[2] & 0x0020) inBuff[2] |= 0x08; else if(!(ByteOr[2] & 0x0020)) inBuff[2] &= ~0x08; 
    if(ByteAnd[1] & 0x0001) inBuff[2] |= 0x10; else if(!(ByteOr[1] & 0x0001)) inBuff[2] &= ~0x10; 
    if(ByteAnd[1] & 0x0002) inBuff[2] |= 0x20; else if(!(ByteOr[1] & 0x0002)) inBuff[2] &= ~0x20; 
    if(ByteAnd[1] & 0x0004) inBuff[2] |= 0x40; else if(!(ByteOr[1] & 0x0004)) inBuff[2] &= ~0x40; 
    if(ByteAnd[4] & 0x0080) inBuff[2] |= 0x80; else if(!(ByteOr[4] & 0x0080)) inBuff[2] &= ~0x80;
    
    /* SFSW_INA11 ~ SFSW_INA18 */ 
    if(ByteAnd[4] & 0x0100) inBuff[3] |= 0x01; else if(!(ByteOr[4] & 0x0100)) inBuff[3] &= ~0x01; 
    if(ByteAnd[4] & 0x0200) inBuff[3] |= 0x02; else if(!(ByteOr[4] & 0x0200)) inBuff[3] &= ~0x02;
    if(ByteAnd[4] & 0x0400) inBuff[3] |= 0x04; else if(!(ByteOr[4] & 0x0400)) inBuff[3] &= ~0x04;
    if(ByteAnd[4] & 0x0800) inBuff[3] |= 0x08; else if(!(ByteOr[4] & 0x0800)) inBuff[3] &= ~0x08; 
    if(ByteAnd[4] & 0x1000) inBuff[3] |= 0x10; else if(!(ByteOr[4] & 0x1000)) inBuff[3] &= ~0x10; 
    if(ByteAnd[4] & 0x2000) inBuff[3] |= 0x20; else if(!(ByteOr[4] & 0x2000)) inBuff[3] &= ~0x20;
    if(ByteAnd[4] & 0x4000) inBuff[3] |= 0x40; else if(!(ByteOr[4] & 0x4000)) inBuff[3] &= ~0x40; 
    if(ByteAnd[4] & 0x8000) inBuff[3] |= 0x80; else if(!(ByteOr[4] & 0x8000)) inBuff[3] &= ~0x80; 

    
#else  
    
    /* INB1 ~ INB8 */
    if(ByteAnd[1] & 0x0040) inBuff[0] |= 0x01; else if(!(ByteOr[1] & 0x0040)) inBuff[0] &= ~0x01; 
    if(ByteAnd[1] & 0x0020) inBuff[0] |= 0x02; else if(!(ByteOr[1] & 0x0020)) inBuff[0] &= ~0x02; 
    if(ByteAnd[1] & 0x0010) inBuff[0] |= 0x04; else if(!(ByteOr[1] & 0x0010)) inBuff[0] &= ~0x04; 
    if(ByteAnd[1] & 0x0008) inBuff[0] |= 0x08; else if(!(ByteOr[1] & 0x0008)) inBuff[0] &= ~0x08; 
    if(ByteAnd[3] & 0x0080) inBuff[0] |= 0x10; else if(!(ByteOr[3] & 0x0080)) inBuff[0] &= ~0x10; 
    if(ByteAnd[3] & 0x0040) inBuff[0] |= 0x20; else if(!(ByteOr[3] & 0x0040)) inBuff[0] &= ~0x20; 
    if(ByteAnd[3] & 0x0020) inBuff[0] |= 0x40; else if(!(ByteOr[3] & 0x0020)) inBuff[0] &= ~0x40; 
    if(ByteAnd[3] & 0x0010) inBuff[0] |= 0x80; else if(!(ByteOr[3] & 0x0010)) inBuff[0] &= ~0x80; 
    
    /* INB9 ~ INB14 */
    if(ByteAnd[3] & 0x0008) inBuff[1] |= 0x01; else if(!(ByteOr[3] & 0x0008)) inBuff[1] &= ~0x01; 
    if(ByteAnd[3] & 0x0004) inBuff[1] |= 0x02; else if(!(ByteOr[3] & 0x0004)) inBuff[1] &= ~0x02; 
    if(ByteAnd[3] & 0x0002) inBuff[1] |= 0x04; else if(!(ByteOr[3] & 0x0002)) inBuff[1] &= ~0x04; 
    if(ByteAnd[3] & 0x0001) inBuff[1] |= 0x08; else if(!(ByteOr[3] & 0x0001)) inBuff[1] &= ~0x08; 
    if(ByteAnd[2] & 0x1000) inBuff[1] |= 0x10; else if(!(ByteOr[2] & 0x1000)) inBuff[1] &= ~0x10; 
    if(ByteAnd[2] & 0x0800) inBuff[1] |= 0x20; else if(!(ByteOr[2] & 0x0800)) inBuff[1] &= ~0x20;
    /* SFSW_INB1 ~ SFSW_INB2 */ 
    if(ByteAnd[0] & 0x0008) inBuff[1] |= 0x40; else if(!(ByteOr[0] & 0x0008)) inBuff[1] &= ~0x40; 
    if(ByteAnd[0] & 0x0010) inBuff[1] |= 0x80; else if(!(ByteOr[0] & 0x0010)) inBuff[1] &= ~0x80; 
    
    /* SFSW_INB3 ~ SFSW_INB10 */ 
    if(ByteAnd[2] & 0x0010) inBuff[2] |= 0x01; else if(!(ByteOr[2] & 0x0010)) inBuff[2] &= ~0x01; 
    if(ByteAnd[2] & 0x0020) inBuff[2] |= 0x02; else if(!(ByteOr[2] & 0x0020)) inBuff[2] &= ~0x02; 
    if(ByteAnd[1] & 0x0001) inBuff[2] |= 0x04; else if(!(ByteOr[1] & 0x0001)) inBuff[2] &= ~0x04; 
    if(ByteAnd[1] & 0x0002) inBuff[2] |= 0x08; else if(!(ByteOr[1] & 0x0002)) inBuff[2] &= ~0x08; 
    if(ByteAnd[1] & 0x0004) inBuff[2] |= 0x10; else if(!(ByteOr[1] & 0x0004)) inBuff[2] &= ~0x10; 
    if(ByteAnd[4] & 0x0080) inBuff[2] |= 0x20; else if(!(ByteOr[4] & 0x0080)) inBuff[2] &= ~0x20; 
    if(ByteAnd[4] & 0x0100) inBuff[2] |= 0x40; else if(!(ByteOr[4] & 0x0100)) inBuff[2] &= ~0x40; 
    if(ByteAnd[4] & 0x0200) inBuff[2] |= 0x80; else if(!(ByteOr[4] & 0x0200)) inBuff[2] &= ~0x80;
    
    /* SFSW_INB11 ~ SFSW_INB18 */ 
    if(ByteAnd[4] & 0x0400) inBuff[3] |= 0x01; else if(!(ByteOr[4] & 0x0400)) inBuff[3] &= ~0x01; 
    if(ByteAnd[4] & 0x0800) inBuff[3] |= 0x02; else if(!(ByteOr[4] & 0x0800)) inBuff[3] &= ~0x02;
    if(ByteAnd[4] & 0x1000) inBuff[3] |= 0x04; else if(!(ByteOr[4] & 0x1000)) inBuff[3] &= ~0x04;
    if(ByteAnd[4] & 0x2000) inBuff[3] |= 0x08; else if(!(ByteOr[4] & 0x2000)) inBuff[3] &= ~0x08; 
    if(ByteAnd[4] & 0x4000) inBuff[3] |= 0x10; else if(!(ByteOr[4] & 0x4000)) inBuff[3] &= ~0x10; 
    if(ByteAnd[4] & 0x8000) inBuff[3] |= 0x20; else if(!(ByteOr[4] & 0x8000)) inBuff[3] &= ~0x20;
    if(ByteAnd[1] & 0x0400) inBuff[3] |= 0x40; else if(!(ByteOr[1] & 0x0400)) inBuff[3] &= ~0x40; 
    if(ByteAnd[1] & 0x0800) inBuff[3] |= 0x80; else if(!(ByteOr[1] & 0x0800)) inBuff[3] &= ~0x80; 
 
    
#endif    

}

#ifdef GEC_DBL2_SLAVE
/*******************************************************************************
* Function Name  : output_driver
* Description    : Enable or disable relay output.
*                  
* Input          : out_buff: set the gpio pin value of data
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void output_driver(u8 out_buff[])
{
	if(out_buff[0] & 0x01) 		RELAY_OC1_ON(); 		else RELAY_OC1_OFF(); 					  
	if(out_buff[0] & 0x02) 		RELAY_OC2_ON(); 		else RELAY_OC2_OFF(); 	 					  
	if(out_buff[0] & 0x04) 		RELAY_OC3_ON(); 		else RELAY_OC3_OFF();	 					  
	if(out_buff[0] & 0x08)  	RELAY_OC4_ON(); 		else RELAY_OC4_OFF(); 						  
	if(out_buff[0] & 0x10)  	RELAY_OC5_ON(); 		else RELAY_OC5_OFF(); 					  
	if(out_buff[0] & 0x20)  	TRAN_OC1_ON(); 		else TRAN_OC1_OFF();				  
	if(out_buff[0] & 0x40)  	TRAN_OC2_ON(); 		else TRAN_OC2_OFF();				  
	if(out_buff[0] & 0x80)		TRAN_OC3_ON(); 		else TRAN_OC3_OFF(); 	
 
}
#endif

/*******************************************************************************
* Function Name  : ReadSwDp
* Description    : Read swdp data.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : swdp value.
*******************************************************************************/
u8 ReadSwDp(void)
{
    u8 swdp[4] = {0};
    u8 value;

#ifdef GEC_DBL2_MASTER 
    swdp[0] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
    swdp[1] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9);
    swdp[2] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10);
    swdp[3] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);

#else    
    swdp[0] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
    swdp[1] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);
    swdp[2] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
    swdp[3] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
#endif
    
    value = ( ((~swdp[3])&0x01) << 3 ) | ( ((~swdp[2])&0x01) << 2 ) | ( ((~swdp[1])&0x01) << 1 ) | ( ((~swdp[0])&0x01) << 0 );
    
    return   value;
}

/*******************************************************************************
* Function Name  : DBL2GetAdr
* Description    : Get the DBL2 up or down configuration.
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void DBL2GetAdr(void)
{

  u8 adr_temp=0;
  static u8 adr_pre=0;
  static u16 adr_cnt=0;
  
  adr_temp = ReadSwDp();
  
  if(adr_temp == adr_pre)
  {
      if(adr_cnt>100)
      {  
          switch(adr_temp)
          {
             case 0x00u: EscData.SwdpAdr = DBL2_TEST_MODE;break;  
             case 0x01u: EscData.SwdpAdr = DBL2_UPPER_ADDR;break; 
             case 0x02u: EscData.SwdpAdr = DBL2_LOWER_ADDR;break; 
             case 0x04u: EscData.SwdpAdr = DBL2_INTERM1_ADDR;break; 
             case 0x08u: EscData.SwdpAdr = DBL2_INTERM2_ADDR;break;              
             default:EscData.SwdpAdr = 0; 
          }
      }
      else
      {
          adr_cnt++;
      }  
  }  
  else
  {
      adr_cnt = 0;
  }  
  
  adr_pre = adr_temp;
}

/******************************  END OF FILE  *********************************/


