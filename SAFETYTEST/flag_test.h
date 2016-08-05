/**
 * Company	: ThyssenKrupp PDC
 * @file	: config_tes.h
 * @purpose     :
 * @brief	: header for c based Register flag bit test POST configure test
 *                  IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 13-May-2014
 * @author	: Paul
*/

#ifndef _FLAG_TEST_H__
#define _FLAG_TEST_H__

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "iec61508.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  /* base 0 */
  unsigned int Flag_Pass_Cnt;     /*!< tests passed */
  /* base 4 */
  unsigned int Flag_Err_Cnt;      /*!< tests failed */
}IEC61508_FlagCheck_struct;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
type_testResult_t flag_test(void);
#endif  /* _FLAG_TEST_H__ */

/************************************** EOF *********************************/
