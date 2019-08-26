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

extern const u8 Logo_72x64[8][72];
extern const u8 Fingerprint_64px[8][64];
extern const u8 RFIDCard_64px[8][64];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
