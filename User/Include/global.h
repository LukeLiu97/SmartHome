/**
******************************************************************************
  * @file       global.h
  * @brief      全局类型变量定义
  * @version    1.0 
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
typedef struct CurtainStu_
{
	u8 CurrentPlace;
	u8 TargetPlace;
	s8 MoveDirection;
}CurtainStu;

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;

extern CurtainStu Curtain;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
