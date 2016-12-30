/*******************************************************************************
* File Name          : spi.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : The file contains prototype of spi driver functions.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define buffersize 100u

/* Exported functions ------------------------------------------------------- */
void SPIx_Init(void);			
void SPIx_DMA_ReceiveSendByte( u16 num );
void DMA_Check_Flag(u32 times);
#ifndef GEC_DBL2_MASTER
void SPIx_Configuration(SPI_TypeDef* SPIx);
#endif

extern u8 SPIx_TX_Data[buffersize] ;
extern u8 SPIx_RX_Data[buffersize] ;




#endif


/******************************  END OF FILE  *********************************/



