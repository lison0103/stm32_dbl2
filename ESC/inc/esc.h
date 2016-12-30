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
#include "bsp_input.h"

/* Exported types ------------------------------------------------------------*/
typedef struct dbl2escdata 
{
    /* Header code and comm times, byte 0~5 */
    u8 HeaderCode[6];
    
    /* Input/output*/
    u8 DBL2InputData[4];
    u8 DBL2OutputData;
    
    /* ANALOG/PT100 */
    u16 DBL2AnalogData[3];
    
    /* swdp switch value */
    u8 SwdpAdr;
    
    /* SEQN */
    u8 SEQN;
    
    /* reset */
    u8 SafetyReset;    

    /* fault status */
    u8 DBL2FaultStatus; 
    
    /* fault */
    u8 ErrorBuff[10];

    /* Esc receive safety data*/
    u8 SafetyReceiveDataA[8];
    u8 SafetyReceiveDataB[8];
    
    /* Esc send data to safety */
    u8 SafetySendData[12];
    
    /* Esc Safety validate success */
    u8 SafetyValidateResult;

	
}DBL2EscData;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define         EN_ERROR1   	EscData.ErrorBuff[0]
#define  	EN_ERROR2   	EscData.ErrorBuff[1]
#define 	EN_ERROR3   	EscData.ErrorBuff[2]
#define 	EN_ERROR4   	EscData.ErrorBuff[3]
#define 	EN_ERROR5   	EscData.ErrorBuff[4]
#define 	EN_ERROR6   	EscData.ErrorBuff[5]
#define 	EN_ERROR7   	EscData.ErrorBuff[6]
#define 	EN_ERROR8   	EscData.ErrorBuff[7]
#define 	EN_ERROR9   	EscData.ErrorBuff[8]
#define 	EN_ERROR10   	EscData.ErrorBuff[9]

/* led flash freq */
#define FREQ_0_5HZ      500u
#define FREQ_2HZ        125u


#define ESC_RT_DATA_LEN    sizeof(DBL2EscData)

/* Exported functions ------------------------------------------------------- */

extern DBL2EscData EscData;
extern DBL2EscData OmcEscData;

extern u16 g_u8LedFreq;

/* safety board data */
extern u8 EscDataToSafety[4][8];
extern u8 EscDataFromSafety[2][8];

extern u8 g_u8SafetyNewData;

extern u16 g_u16CANSendFail;
extern u16 g_u16CANSendSuccess;

#endif  /* __ESC_H */


/******************************  END OF FILE  *********************************/








