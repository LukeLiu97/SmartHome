/**
******************************************************************************
  * @file       uart.h
  * @brief      ���ڲ�����غ���ͷ�ļ�
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

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
/* Exported functions ------------------------------------------------------- */

void USART1_Init(u32 BaudRate);
void USART2_Init(u32 BaudRate);

void UART2_SendByte(u8 Data);
void UART2_SendString(u8 *String);

#ifdef __cplusplus
}
#endif

#endif
