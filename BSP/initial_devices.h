/*******************************************************************************
* File Name          : initial_devices.h
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : Contains prototype of the devices initinal.
*			          
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INITIAL_DEVICES_H
#define __INITIAL_DEVICES_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h"    
#include "led.h"
#include "hw_test.h"
#include "spi.h"
#include "ewdt.h"
#include "timer.h"
#include "safety_test.h"
#ifdef GEC_DBL2_SLAVE 
#include "can.h"
#include "adc.h"
#include "max31865.h"
#endif


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SELF_TEST       ( 0 )
#define SYSTEMTICK      ( 2u )

/* Exported functions ------------------------------------------------------- */
void Initial_Device(void);







#endif  /* __INITIAL_DEVICES_H */


/******************************  END OF FILE  *********************************/





