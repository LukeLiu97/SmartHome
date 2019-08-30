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
typedef struct StepMotorStu_
{
	u8 CurrentPlace;
	u8 TargetPlace;
	s8 MoveDirection;
}StepMotorStu;

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

typedef struct SysInfoStu_
{
	u8 SysSN[13];
	RGBStu RGB_LED;
	StepMotorStu Curtain;
	s8 FanMotor;
	u8 Socket;
	float Temper;
	float Humidity;
	u8 WIFI_Status;
}SysInfoStu;

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;

//extern CurtainStu Curtain;

extern RemoteStu Remote;

//extern RGBStu RGB;

extern union UserData_ UserData;

extern u8 RecvBuff[1024];
extern u32 RecvCount;
extern u8 RecvEnd;

extern SysInfoStu SysInfo;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
