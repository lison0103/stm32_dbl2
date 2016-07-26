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


#define INPUT_PORT1_8       EscRTBuff[4]
#define INPUT_PORT9_16      EscRTBuff[5]
#define INPUT_PORT17_24     EscRTBuff[6]
#define INPUT_PORT24_28     EscRTBuff[7]
#define INPUT_PORT_EX1_8    EscRTBuff[8]
#define INPUT_PORT_EX9_16   EscRTBuff[9]
#define INPUT_PORT_EX17     EscRTBuff[10]

#define INPUT_PORT1_MASK        ( 1 << 0 )
#define INPUT_PORT2_MASK        ( 1 << 1 )
#define INPUT_PORT3_MASK        ( 1 << 2 )
#define INPUT_PORT4_MASK        ( 1 << 3 )
#define INPUT_PORT5_MASK        ( 1 << 4 )
#define INPUT_PORT6_MASK        ( 1 << 5 )
#define INPUT_PORT7_MASK        ( 1 << 6 )
#define INPUT_PORT8_MASK        ( 1 << 7 )

#define INPUT_PORT9_MASK        ( 1 << 0 )
#define INPUT_PORT10_MASK        ( 1 << 1 )
#define INPUT_PORT11_MASK        ( 1 << 2 )
#define INPUT_PORT12_MASK        ( 1 << 3 )
#define INPUT_PORT13_MASK        ( 1 << 4 )
#define INPUT_PORT14_MASK        ( 1 << 5 )
#define INPUT_PORT15_MASK        ( 1 << 6 )
#define INPUT_PORT16_MASK        ( 1 << 7 )

#define INPUT_PORT17_MASK        ( 1 << 0 )
#define INPUT_PORT18_MASK        ( 1 << 1 )
#define INPUT_PORT19_MASK        ( 1 << 2 )
#define INPUT_PORT20_MASK        ( 1 << 3 )
#define INPUT_PORT21_MASK        ( 1 << 4 )
#define INPUT_PORT22_MASK        ( 1 << 5 )
#define INPUT_PORT23_MASK        ( 1 << 6 )
#define INPUT_PORT24_MASK        ( 1 << 7 )

#define INPUT_PORT25_MASK        ( 1 << 0 )
#define INPUT_PORT26_MASK        ( 1 << 1 )
#define INPUT_PORT27_MASK        ( 1 << 2 )
#define INPUT_PORT28_MASK        ( 1 << 3 )

#define INPUT_PORT_EX1_MASK        ( 1 << 0 )
#define INPUT_PORT_EX2_MASK        ( 1 << 1 )
#define INPUT_PORT_EX3_MASK        ( 1 << 2 )
#define INPUT_PORT_EX4_MASK        ( 1 << 3 )
#define INPUT_PORT_EX5_MASK        ( 1 << 4 )
#define INPUT_PORT_EX6_MASK        ( 1 << 5 )
#define INPUT_PORT_EX7_MASK        ( 1 << 6 )
#define INPUT_PORT_EX8_MASK        ( 1 << 7 )

#define INPUT_PORT_EX9_MASK        ( 1 << 0 )
#define INPUT_PORT_EX10_MASK        ( 1 << 1 )
#define INPUT_PORT_EX11_MASK        ( 1 << 2 )
#define INPUT_PORT_EX12_MASK        ( 1 << 3 )
#define INPUT_PORT_EX13_MASK        ( 1 << 4 )
#define INPUT_PORT_EX14_MASK        ( 1 << 5 )
#define INPUT_PORT_EX15_MASK        ( 1 << 6 )
#define INPUT_PORT_EX16_MASK        ( 1 << 7 )

#define INPUT_PORT_EX17_MASK        ( 1 << 0 )


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
extern u16 pcOMC_SfBase_EscState;
extern u8 *const pcErrorBuff;
extern u8 *const pcEscErrorCodeBuff;
extern u8 *const pcEscErrorBuff;
extern u8 *const pcOmcErrorBuff;




#endif  /* __ESC_H */


/******************************  END OF FILE  *********************************/








