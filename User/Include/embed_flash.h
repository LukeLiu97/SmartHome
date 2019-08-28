/**
******************************************************************************
  * @file       embed_flash.h
  * @brief      
  * @version    1.0
  * @date       Aug-27-2019 Tue
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EMBED_FLASH_H
#define __EMBED_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "global.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define FLASH_PAGE_SIZE    		((uint16_t)0x400)
#define USER_FLASH_START_ADDR	((u32)(0x08000000 + FLASH_PAGE_SIZE * 127))
#define USER_FLASH_END_ADDR		((u32)(0x08000000 + FLASH_PAGE_SIZE * 128 - 1))
/* Exported functions ------------------------------------------------------- */

u8 EmbedFlash_Write(u32 Addr,const u32 *Data,size_t DataSize);
void UserData_Updata(void);

#ifdef __cplusplus
}
#endif

#endif
