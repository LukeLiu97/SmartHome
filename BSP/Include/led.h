/**
******************************************************************************
  * @file       led.h
  * @brief      LED相关函数头文件
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "tim.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define LED2_OFF() (GPIO_SetBits(GPIOB, GPIO_Pin_14))
#define LED2_ON() (GPIO_ResetBits(GPIOB, GPIO_Pin_14))
#define LED2_OR() (GPIOB->ODR ^= (0x1 << 14))

/* Exported functions ------------------------------------------------------- */
void LED_Init(void);
void RGB_LED_Init(void);
void RGB_LED_Control(u8 Blue, u8 Green, u8 Red);

#ifdef __cplusplus
}
#endif

#endif
