/**
******************************************************************************
  * @file       esp8266.h
  * @brief      ESP8266驱动与云平台连接使用的HTTP函数
  * @version    1.0
  * @date       Aug-29-2019 Thu
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "uart.h"
#include "string.h"
#include "time.h"
#include "global.h"

/* Global variables ----------------------------------------------------------*/
extern u8 ATCmd_String[];
extern u8 ATRespond[];

/* Exported types ------------------------------------------------------------*/
typedef enum
{
	Event_1 = 0,
	Event_2 ,
	Event_3 ,
	Event_4 ,
	Event_5 ,
	Event_6 ,
	Event_Max
}EventTable;

typedef enum
{
	TypeOfs8 = 1,
	TypeOfu8,
}DataPointTypeStu;

typedef struct DataPointStu_
{
	s8 S8_DataPoint;
	u8 U8_DataPoint;
}DataPointStu;

typedef struct EventInfoStu_
{
	u8 Update;
	u8 DeviceSN[13];
	u8 ID;
	u8 DataPonitType;
	DataPointStu DataPonit;
}EventInfoStu;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 ESP8266_SendATCmd(u8 *SendBuff,u8 *RespondBuff,u32 Timeout);
u8 ESP8266_HTTP_Init(void);
u8 ESP8266_ConnectWIFI(u8 *SSID,u8 *Password);
u8 ESP8266_ConnetServer(u8* tppe,u8 *RemoteIP,u8 *RemotePort);
u8 ESP8266_HTTPRequestGet(u8 *RemoteIP);
u8 ESP8266_GetEventInfo(SysInfoStu SysInfo);
void ESP8266_EventInfoHandle(SysInfoStu *SysInfo);
u8 ESP8266_HTTPRequestReport(u8 *RemoteIP,SysInfoStu SysInfo);

#ifdef __cplusplus
}
#endif

#endif
