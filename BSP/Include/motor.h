/**
******************************************************************************
  * @file       motor.h
  * @brief      电机控制驱动头文件
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "tim.h"
#include "global.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Motor_Init(void);
void Motor_Control(s8 Motor);
void StepMotor_Init(void);
void StepMotor_Roll(void);

#ifdef __cplusplus
}
#endif

#endif
