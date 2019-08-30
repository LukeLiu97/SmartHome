/**
******************************************************************************
  * @file       font.h
  * @brief      字体头文件
  * @version    1.0
  * @date       Aug-10-2019 Sat
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONT_H
#define __FONT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" //需要u8类型定义

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const u8 HexChar_8x16[16][16];

extern const u8 SymbolChar_8x16[2][16];
extern const u8 SymbolChar_16x16[2][32];

extern const u8 SuccessString_16x16[4][16];
extern const u8 FailString_16x16[4][16];
extern const u8 TemperString_16x16[4][16];
extern const u8 HumidityString_16x16[4][16];
extern const u8 SerialNumberString_16x16[8][16];
extern const u8 NetStatusString_16x16[8][16];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
