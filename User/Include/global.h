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

typedef struct RemoteStu_
{
	u8 Start;
	u32 RemoteCode;
	u8 	End;
}RemoteStu;

typedef struct RGBStu_
{
	u8 Blue;
	u8 Green;
	u8 Red;
}RGBStu;

union UserData_
{
	u32 Data;
	u8 DataArray[4];
};

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;

extern CurtainStu Curtain;

extern RemoteStu Remote;

extern RGBStu RGB;

extern union UserData_ UserData;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
