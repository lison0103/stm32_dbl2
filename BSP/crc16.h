/*******************************************************************************
* File Name          : crc16.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Contains the prototype of 16-bit CRC Calculation and verification.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC16_H
#define __CRC16_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#ifdef GEC_DBL2_SLAVE
#define SAFETY_POLYNOMIALS      0x992C1A4Cu
#define SAFETY_POLYNOMIALSB      0x90022004u
#else
#define SAFETY_POLYNOMIALS      0x90022004u
#define SAFETY_POLYNOMIALSB      0x992C1A4Cu
#endif

/* Exported functions ------------------------------------------------------- */
uint16_t MB_CRC16( uint8_t *pucFrame, uint16_t usLen );
u32 MB_CRC32(u8 pucFrame[], u16 usLen, u32 Polynomials);

#endif /* __CRC16_H */


/******************************  END OF FILE  *********************************/
