/**
 * Company	: ThyssenKrupp PDC
 * @file	: iec61508.h
 * @purpose     :
 * @brief	: IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 22-April-2014
 * @author	: Paul
*/

#ifndef __IEC61508_
#define __IEC61508_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef enum testResult_enum
{
  IEC61508_testFailed     = 0,              /*!< test result failed replacement */
  IEC61508_testPassed     = 1,              /*!< test result passed replacement */
  IEC61508_testInProgress = 2               /*!< test is still in progress replacement */
} type_testResult_t;

typedef enum testState_enum
{
  IEC61508_stateDone       = 0,             /*!< test is done replacement */
  IEC61508_statePending    = 1,             /*!< test is pending replacement */
  IEC61508_stateInProgress = 2              /*!< test is still in progress replacement */
} type_testState_t;

typedef enum testStart_enum
{
  IEC61508_stop  = 0,                       /*!< test command stop */
  IEC61508_start = 1                        /*!< test command start */
} type_testStart_t;  
  

/*	User configuration	*/
  
/* clock test */
typedef struct ClkTest_struct
{
  uint32_t Ratio;                           /*!< ration of timer ticks to RTC ticks, defines the number of timer calls in the test*/
  uint32_t Tolerance;                       /*!< tolerance of timer ticks, defines the deviation acceptable in the test*/
  uint32_t timerCounter;                    /*!< The counter Variable in the test */
  uint32_t baseTicks;                       /*!< basic counter occurrences */
  uint32_t result;                          /*!< result after test finished = (testResult_t + 1) */
} ClockTest_t;  
  
/* Interrupt test */
#define _VAL_A    0x01234567                /*!< values fixed to 32 bit regardless of compiler */
#define _VAL_B    0x12345678
#define _VAL_C    0x23456789
#define _VAL_D    0x3456789A
#define _VAL_E    0x456789AB
#define _VAL_F    0x56789ABC
#define _VAL_G    0x6789ABCD
/* possible return values */
enum _ReturnValues
{
  VAL_A = _VAL_A,
  VAL_B = _VAL_B,
  VAL_C = _VAL_C,
  VAL_D = _VAL_D,
  VAL_E = _VAL_E,
  VAL_F = _VAL_F,
  VAL_G = _VAL_G,
};
typedef enum _ReturnValues   PCTest_ReturnValues;

/* Program Counter test */
struct _Data
{
  PCTest_ReturnValues ret;
  uint32_t            number;        /*!< action type */
};
typedef struct _Data   Data_t;

typedef PCTest_ReturnValues (*Exec_t)(Data_t *data);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
