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
    static u16 PinValue[3u][5u],ByteAnd[5u],ByteOr[5u],read_pin_cnt = 0u;
    u16 i; 
    
    
    read_pin_cnt++;  
    if( read_pin_cnt > 2u ) 
    {
        read_pin_cnt = 0u;  
    } 
    
    /* Read I/O port */                      
    PinValue[read_pin_cnt][0] = GPIO_ReadInputData(GPIOA);
    PinValue[read_pin_cnt][1] = GPIO_ReadInputData(GPIOB);
    PinValue[read_pin_cnt][2] = GPIO_ReadInputData(GPIOC);
    PinValue[read_pin_cnt][3] = GPIO_ReadInputData(GPIOD);
    PinValue[read_pin_cnt][4] = GPIO_ReadInputData(GPIOE);
    
    for(i=0u;i<5u;i++)
    {   
         /* For MISRA C 2004 12.7 */
          ByteAnd[i] = PinValue[0u][i] & PinValue[1u][i];
          ByteAnd[i] &= PinValue[2u][i];
          ByteOr[i] = PinValue[0u][i] | PinValue[1u][i];
          ByteOr[i] |= PinValue[2u][i];
    }
    
#ifdef GEC_DBL2_MASTER
    
    /* INA1 ~ INA8 */
    if(ByteAnd[1u] & 0x0040u) { inBuff[0u] |= 0x01u; } else if(!(ByteOr[1u] & 0x0040u)) { inBuff[0u] &= ~0x01u; }else{}
    if(ByteAnd[1u] & 0x0020u) { inBuff[0u] |= 0x02u; } else if(!(ByteOr[1u] & 0x0020u)) { inBuff[0u] &= ~0x02u; }else{}
    if(ByteAnd[1u] & 0x0010u) { inBuff[0u] |= 0x04u; } else if(!(ByteOr[1u] & 0x0010u)) { inBuff[0u] &= ~0x04u; }else{}
    if(ByteAnd[1u] & 0x0008u) { inBuff[0u] |= 0x08u; } else if(!(ByteOr[1u] & 0x0008u)) { inBuff[0u] &= ~0x08u; }else{}
    if(ByteAnd[3u] & 0x0080u) { inBuff[0u] |= 0x10u; } else if(!(ByteOr[3u] & 0x0080u)) { inBuff[0u] &= ~0x10u; }else{}
    if(ByteAnd[3u] & 0x0040u) { inBuff[0u] |= 0x20u; } else if(!(ByteOr[3u] & 0x0040u)) { inBuff[0u] &= ~0x20u; }else{}
    if(ByteAnd[3u] & 0x0020u) { inBuff[0u] |= 0x40u; } else if(!(ByteOr[3u] & 0x0020u)) { inBuff[0u] &= ~0x40u; }else{}
    if(ByteAnd[3u] & 0x0010u) { inBuff[0u] |= 0x80u; } else if(!(ByteOr[3u] & 0x0010u)) { inBuff[0u] &= ~0x80u; }else{}
    
    /* INA9 ~ INA14 */
    if(ByteAnd[3u] & 0x0008u) { inBuff[1u] |= 0x01u; } else if(!(ByteOr[3u] & 0x0008u)) { inBuff[1u] &= ~0x01u; }else{}
    if(ByteAnd[3u] & 0x0004u) { inBuff[1u] |= 0x02u; } else if(!(ByteOr[3u] & 0x0004u)) { inBuff[1u] &= ~0x02u; }else{}
    if(ByteAnd[3u] & 0x0002u) { inBuff[1u] |= 0x04u; } else if(!(ByteOr[3u] & 0x0002u)) { inBuff[1u] &= ~0x04u; }else{}
    if(ByteAnd[3u] & 0x0001u) { inBuff[1u] |= 0x08u; } else if(!(ByteOr[3u] & 0x0001u)) { inBuff[1u] &= ~0x08u; }else{}
    if(ByteAnd[2u] & 0x1000u) { inBuff[1u] |= 0x10u; } else if(!(ByteOr[2u] & 0x1000u)) { inBuff[1u] &= ~0x10u; }else{}
    if(ByteAnd[2u] & 0x0800u) { inBuff[1u] |= 0x20u; } else if(!(ByteOr[2u] & 0x0800u)) { inBuff[1u] &= ~0x20u;}else{}
    /* SFSW_INA1 ~ SFSW_INA2 */ 
    if(ByteAnd[0] & 0x0010u) { inBuff[1u] |= 0x40u; } else if(!(ByteOr[0] & 0x0010u)) { inBuff[1u] &= ~0x40u; }else{}
    if(ByteAnd[0] & 0x0020u) { inBuff[1u] |= 0x80u; } else if(!(ByteOr[0] & 0x0020u)) { inBuff[1u] &= ~0x80u; }else{}
    
    /* SFSW_INA3 ~ SFSW_INA10 */ 
    if(ByteAnd[0] & 0x0040u) { inBuff[2u] |= 0x01u; } else if(!(ByteOr[0] & 0x0040u)) { inBuff[2u] &= ~0x01u; }else{}
    if(ByteAnd[0] & 0x0080u) { inBuff[2u] |= 0x02u; } else if(!(ByteOr[0] & 0x0080u)) { inBuff[2u] &= ~0x02u; }else{}
    if(ByteAnd[2u] & 0x0010u) { inBuff[2u] |= 0x04u; } else if(!(ByteOr[2u] & 0x0010u)) { inBuff[2u] &= ~0x04u; }else{}
    if(ByteAnd[2u] & 0x0020u) { inBuff[2u] |= 0x08u; } else if(!(ByteOr[2u] & 0x0020u)) { inBuff[2u] &= ~0x08u; }else{}
    if(ByteAnd[1u] & 0x0001u) { inBuff[2u] |= 0x10u; } else if(!(ByteOr[1u] & 0x0001u)) { inBuff[2u] &= ~0x10u; }else{}
    if(ByteAnd[1u] & 0x0002u) { inBuff[2u] |= 0x20u; } else if(!(ByteOr[1u] & 0x0002u)) { inBuff[2u] &= ~0x20u; }else{}
    if(ByteAnd[1u] & 0x0004u) { inBuff[2u] |= 0x40u; } else if(!(ByteOr[1u] & 0x0004u)) { inBuff[2u] &= ~0x40u; }else{}
    if(ByteAnd[4u] & 0x0080u) { inBuff[2u] |= 0x80u; } else if(!(ByteOr[4u] & 0x0080u)) { inBuff[2u] &= ~0x80u;}else{}
    
    /* SFSW_INA11 ~ SFSW_INA18 */ 
    if(ByteAnd[4u] & 0x0100u) { inBuff[3u] |= 0x01u; } else if(!(ByteOr[4u] & 0x0100u)) { inBuff[3u] &= ~0x01u; }else{}
    if(ByteAnd[4u] & 0x0200u) { inBuff[3u] |= 0x02u; } else if(!(ByteOr[4u] & 0x0200u)) { inBuff[3u] &= ~0x02u;}else{}
    if(ByteAnd[4u] & 0x0400u) { inBuff[3u] |= 0x04u; } else if(!(ByteOr[4u] & 0x0400u)) { inBuff[3u] &= ~0x04u;}else{}
    if(ByteAnd[4u] & 0x0800u) { inBuff[3u] |= 0x08u; } else if(!(ByteOr[4u] & 0x0800u)) { inBuff[3u] &= ~0x08u; }else{}
    if(ByteAnd[4u] & 0x1000u) { inBuff[3u] |= 0x10u; } else if(!(ByteOr[4u] & 0x1000u)) { inBuff[3u] &= ~0x10u; }else{}
    if(ByteAnd[4u] & 0x2000u) { inBuff[3u] |= 0x20u; } else if(!(ByteOr[4u] & 0x2000u)) { inBuff[3u] &= ~0x20u;}else{}
    if(ByteAnd[4u] & 0x4000u) { inBuff[3u] |= 0x40u; } else if(!(ByteOr[4u] & 0x4000u)) { inBuff[3u] &= ~0x40u; }else{}
    if(ByteAnd[4u] & 0x8000u) { inBuff[3u] |= 0x80u; } else if(!(ByteOr[4u] & 0x8000u)) { inBuff[3u] &= ~0x80u; }else{}

    
#else  
    
    /* INB1 ~ INB8 */
if(ByteAnd[1u] & 0x0040u){ inBuff[0u] |= 0x01u; } else if(!(ByteOr[1u] & 0x0040u)){ inBuff[0u] &= ~0x01u;}else{} 
if(ByteAnd[1u] & 0x0020u){ inBuff[0u] |= 0x02u; } else if(!(ByteOr[1u] & 0x0020u)){ inBuff[0u] &= ~0x02u; }else{}
if(ByteAnd[1u] & 0x0010u){ inBuff[0u] |= 0x04u; } else if(!(ByteOr[1u] & 0x0010u)){ inBuff[0u] &= ~0x04u; }else{}
if(ByteAnd[1u] & 0x0008u){ inBuff[0u] |= 0x08u; } else if(!(ByteOr[1u] & 0x0008u)) {inBuff[0u] &= ~0x08u; }else{}
if(ByteAnd[3u] & 0x0080u){ inBuff[0u] |= 0x10u; } else if(!(ByteOr[3u] & 0x0080u)) {inBuff[0u] &= ~0x10u; }else{}
if(ByteAnd[3u] & 0x0040u){ inBuff[0u] |= 0x20u; } else if(!(ByteOr[3u] & 0x0040u)) {inBuff[0u] &= ~0x20u; }else{}
if(ByteAnd[3u] & 0x0020u){ inBuff[0u] |= 0x40u; } else if(!(ByteOr[3u] & 0x0020u)) {inBuff[0u] &= ~0x40u; }else{}
if(ByteAnd[3u] & 0x0010u){ inBuff[0u] |= 0x80u; } else if(!(ByteOr[3u] & 0x0010u)) {inBuff[0u] &= ~0x80u; }else{}
    
    /* INB9 ~ INB14 */
if(ByteAnd[3u] & 0x0008u){ inBuff[1u] |= 0x01u; } else if(!(ByteOr[3u] & 0x0008u)){ inBuff[1u] &= ~0x01u; }else{}
if(ByteAnd[3u] & 0x0004u){ inBuff[1u] |= 0x02u; } else if(!(ByteOr[3u] & 0x0004u)){ inBuff[1u] &= ~0x02u; }else{}
if(ByteAnd[3u] & 0x0002u) {inBuff[1u] |= 0x04u; } else if(!(ByteOr[3u] & 0x0002u)){ inBuff[1u] &= ~0x04u; }else{}
if(ByteAnd[3u] & 0x0001u) {inBuff[1u] |= 0x08u; } else if(!(ByteOr[3u] & 0x0001u)) {inBuff[1u] &= ~0x08u; }else{}
if(ByteAnd[2u] & 0x1000u) {inBuff[1u] |= 0x10u; } else if(!(ByteOr[2u] & 0x1000u)){ inBuff[1u] &= ~0x10u; }else{}
if(ByteAnd[2u] & 0x0800u) {inBuff[1u] |= 0x20u; } else if(!(ByteOr[2u] & 0x0800u)) {inBuff[1u] &= ~0x20u;}else{}
    /* SFSW_INB1 ~ SFSW_INB2 */ 
if(ByteAnd[0] & 0x0008u){ inBuff[1u] |= 0x40u; } else if(!(ByteOr[0] & 0x0008u)){ inBuff[1u] &= ~0x40u; }else{}
if(ByteAnd[0] & 0x0010u) {inBuff[1u] |= 0x80u; } else if(!(ByteOr[0] & 0x0010u)) {inBuff[1u] &= ~0x80u; }else{}
    
    /* SFSW_INB3 ~ SFSW_INB10 */ 
    if(ByteAnd[2u] & 0x0010u) { inBuff[2u] |= 0x01u; } else if(!(ByteOr[2u] & 0x0010u)) { inBuff[2u] &= ~0x01u; }else{}
    if(ByteAnd[2u] & 0x0020u) { inBuff[2u] |= 0x02u; } else if(!(ByteOr[2u] & 0x0020u)) { inBuff[2u] &= ~0x02u; }else{}
    if(ByteAnd[1u] & 0x0001u) { inBuff[2u] |= 0x04u; } else if(!(ByteOr[1u] & 0x0001u)) { inBuff[2u] &= ~0x04u; }else{}
    if(ByteAnd[1u] & 0x0002u) { inBuff[2u] |= 0x08u; } else if(!(ByteOr[1u] & 0x0002u)) { inBuff[2u] &= ~0x08u; }else{}
    if(ByteAnd[1u] & 0x0004u) { inBuff[2u] |= 0x10u; } else if(!(ByteOr[1u] & 0x0004u)) { inBuff[2u] &= ~0x10u; }else{}
    if(ByteAnd[4u] & 0x0080u) { inBuff[2u] |= 0x20u; } else if(!(ByteOr[4u] & 0x0080u)) { inBuff[2u] &= ~0x20u; }else{}
    if(ByteAnd[4u] & 0x0100u) { inBuff[2u] |= 0x40u; } else if(!(ByteOr[4u] & 0x0100u)) { inBuff[2u] &= ~0x40u; }else{}
    if(ByteAnd[4u] & 0x0200u) { inBuff[2u] |= 0x80u; } else if(!(ByteOr[4u] & 0x0200u)) { inBuff[2u] &= ~0x80u; }else{}
    
    /* SFSW_INB11 ~ SFSW_INB18 */ 
    if(ByteAnd[4u] & 0x0400u) { inBuff[3u] |= 0x01u; } else if(!(ByteOr[4u] & 0x0400u)) { inBuff[3u] &= ~0x01u; }else{}
    if(ByteAnd[4u] & 0x0800u) { inBuff[3u] |= 0x02u; } else if(!(ByteOr[4u] & 0x0800u)) { inBuff[3u] &= ~0x02u; }else{}
    if(ByteAnd[4u] & 0x1000u) { inBuff[3u] |= 0x04u; } else if(!(ByteOr[4u] & 0x1000u)) { inBuff[3u] &= ~0x04u; }else{}
    if(ByteAnd[4u] & 0x2000u) { inBuff[3u] |= 0x08u; } else if(!(ByteOr[4u] & 0x2000u)) { inBuff[3u] &= ~0x08u; }else{}
    if(ByteAnd[4u] & 0x4000u) { inBuff[3u] |= 0x10u; } else if(!(ByteOr[4u] & 0x4000u)) { inBuff[3u] &= ~0x10u; }else{}
    if(ByteAnd[4u] & 0x8000u) { inBuff[3u] |= 0x20u; } else if(!(ByteOr[4u] & 0x8000u)) { inBuff[3u] &= ~0x20u; }else{}
    if(ByteAnd[1u] & 0x0400u) { inBuff[3u] |= 0x40u; } else if(!(ByteOr[1u] & 0x0400u)) { inBuff[3u] &= ~0x40u; }else{}
    if(ByteAnd[1u] & 0x0800u) { inBuff[3u] |= 0x80u; } else if(!(ByteOr[1u] & 0x0800u)) { inBuff[3u] &= ~0x80u; }else{}
 
    
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
    if(out_buff[0] & 0x01u)  { 	RELAY_OC1_ON(); }	else {RELAY_OC1_OFF(); 	}				  
    if(out_buff[0] & 0x02u)  {	RELAY_OC2_ON(); }	else {RELAY_OC2_OFF(); 	 }					  
    if(out_buff[0] & 0x04u)  {	RELAY_OC3_ON(); }	else {RELAY_OC3_OFF();	} 					  
    if(out_buff[0] & 0x08u)  {	RELAY_OC4_ON(); }	else {RELAY_OC4_OFF(); 	}					  
    if(out_buff[0] & 0x10u)  {	RELAY_OC5_ON(); }	else {RELAY_OC5_OFF(); 	}				  
    if(out_buff[0] & 0x20u)  {	TRAN_OC1_ON(); 	}	else {TRAN_OC1_OFF();	}		  
    if(out_buff[0] & 0x40u)  {	TRAN_OC2_ON(); 	}	else {TRAN_OC2_OFF();	}		  
    if(out_buff[0] & 0x80u)  {	TRAN_OC3_ON(); 	}	else {TRAN_OC3_OFF(); 	}
 
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
    u8 value = 0u;

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
    
    value = ( ((u8)(~swdp[3])&0x01u) << 3u ) | ( ((u8)(~swdp[2])&0x01u) << 2u ) | ( ((u8)(~swdp[1])&0x01u) << 1u ) | ( ((u8)(~swdp[0])&0x01u) << 0u );
    
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

  u8 adr_temp=0u;
  static u8 adr_pre=0u;
  static u16 adr_cnt=0u;
  
  adr_temp = ReadSwDp();
  
  if(adr_temp == adr_pre)
  {
      if(adr_cnt>100u)
      {  
          switch(adr_temp)
          {
             case 0x00u: EscData.SwdpAdr = DBL2_TEST_MODE;break;  
             case 0x01u: EscData.SwdpAdr = DBL2_UPPER_ADDR;break; 
             case 0x02u: EscData.SwdpAdr = DBL2_LOWER_ADDR;break; 
             case 0x04u: EscData.SwdpAdr = DBL2_INTERM1_ADDR;break; 
             case 0x08u: EscData.SwdpAdr = DBL2_INTERM2_ADDR;break;              
             default:EscData.SwdpAdr = 0u; break;
          }
      }
      else
      {
          adr_cnt++;
      }  
  }  
  else
  {
      adr_cnt = 0u;
  }  
  
  adr_pre = adr_temp;
}

/******************************  END OF FILE  *********************************/


