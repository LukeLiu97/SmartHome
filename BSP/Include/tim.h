/**
******************************************************************************
  * @file       tim.h
  * @brief      通用定时器驱动头文件
  * @version    1.0
  * @date       Aug-12-2019 Mon
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void TIM2_InputCapture_Channel2_Init(void);
void TIM3_PWM_Init(u16 us);
void TIM4_Interrupt_Init(u16 us);

#ifdef __cplusplus
}
#endif

#endif
