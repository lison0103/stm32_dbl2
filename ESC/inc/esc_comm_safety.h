/*******************************************************************************
* File Name          : esc_comm_safety.h
* Author             : lison
* Version            : V1.0
* Date               : 06/20/2016
* Last modify date   : 10/28/2016
* Description        : This file contains prototype of esc DBL2 communication  
*                      with safety board.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESC_COMM_SAFETY_H
#define __ESC_COMM_SAFETY_H

/* Includes ------------------------------------------------------------------*/
#include "esc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Communication_To_Safety(void);
void Safety_Receive_Data_Process(void);
void Safety_Send_Data_Process(u8 respone);


#endif  /* __ESC_COMM_SAFETY_H */


/******************************  END OF FILE  *********************************/



