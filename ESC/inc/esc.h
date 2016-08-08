/*******************************************************************************
* File Name          : esc.h
* Author             : lison
* Version            : V1.0
* Date               : 05/12/2016
* Description        : This file contains esc parameters.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_H
#define __ESC_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* ESC STATE */
#define ESC_STATE_STOP        ((u16)0x0001)
#define ESC_STATE_INSP        ((u16)0x0002)
#define ESC_STATE_RUNNING     ((u16)0x0004)
#define ESC_STATE_FAST        ((u16)0x0008)
#define ESC_STATE_SLOW        ((u16)0x0010)
#define ESC_STATE_INTT        ((u16)0x0020)
#define ESC_STATE_SPEEDUP     ((u16)0x0040)
#define ESC_STATE_RUN5S       ((u16)0x0080)
#define ESC_STATE_UP          ((u16)0x0100)
#define ESC_STATE_DOWN        ((u16)0x0200)
#define ESC_STATE_READY       ((u16)0x0400)
#define ESC_STATE_NORMAL      ((u16)0x0800)
#define ESC_STATE_FAULT       ((u16)0x1000)

#define         EN_ERROR1   	pcErrorBuff[0]
#define  	EN_ERROR2   	pcErrorBuff[1]
#define 	EN_ERROR3   	pcErrorBuff[2]
#define 	EN_ERROR4   	pcErrorBuff[3]
#define 	EN_ERROR5   	pcErrorBuff[4]
#define 	EN_ERROR6   	pcErrorBuff[5]
#define 	EN_ERROR7   	pcErrorBuff[6]
#define 	EN_ERROR8   	pcErrorBuff[7]
#define 	EN_ERROR9   	pcErrorBuff[8]
#define 	EN_ERROR10   	pcErrorBuff[9]



/* Exported functions ------------------------------------------------------- */

extern u8 EscRTBuff[200];
extern u8 McRxBuff[1000];
extern u8 Modbuff[3000];
extern u8 *const pcOMC_EscRTBuff;
extern u8 *const pcErrorBuff;;
extern u8 *const pcOmcErrorBuff;




#endif  /* __ESC_H */


/******************************  END OF FILE  *********************************/








