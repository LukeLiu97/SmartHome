/**
******************************************************************************
  * @file       esp8266.c
  * @brief      ESP8266驱动与云平台连接使用的HTTP函数
  * @version    1.0
  * @date       Aug-29-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "esp8266.h"

/** @addtogroup ESP8266
  * @{
  */


/* Global variables ----------------------------------------------------------*/

u8 SmartHome_EventReg[Event_Max] =
{
	TypeOfu8,// RGB RedValue
	TypeOfu8,// RGB GreenValue
	TypeOfu8,// RGB BlueValue
	TypeOfs8,// Fan 
	TypeOfu8,// Crutain
	TypeOfu8,// Socket
};
EventInfoStu EventInfo[Event_Max];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
u8 ESP8266_SendATCmd(u8 *SendBuff,u8 *RespondBuff,u32 Timeout)
{
	u32 Count = 0;
	
	memset(RecvBuff,0,sizeof(RecvBuff));
	RecvCount = 0;
	UART2_SendString(SendBuff);
	
	while(1)
	{
		Delay_ms(1);
		if(++Count > Timeout)
		{
			return 1;
		}
		else
		{
			if(strstr((const char *)RecvBuff,(const char *)RespondBuff) != NULL)
			{
				return 0;
			}
			else
			{
			}
		}
	}
}

void ESP8266_Exit_PassThroughMode(void)
{
	UART2_SendString((u8 *)"+++");
	
	return ;
}

u8 ESP8266_HTTP_Init(void)
{
	u8 Retval;
	ESP8266_Exit_PassThroughMode();
	Delay_ms(100);
	
	Retval = ESP8266_SendATCmd((u8 *)"AT+RESTORE\r\n",(u8 *)"OK",1000);// 恢复出厂设置，返回ready,重启完成
	
	if(Retval == 1)
	{
		return 1;
	}
	else
	{	
	}
	
	/* 等待重启 */
	Delay_ms(1000);
	
	Retval = ESP8266_SendATCmd((u8 *)"AT+CWMODE_DEF=1\r\n",(u8 *)"OK",1000);// 设置为单STA模式
	if(Retval == 1)
	{
		return 1;
	}
	else
	{	
	}
	
	Retval = ESP8266_SendATCmd((u8 *)"AT+CIPMODE=1\r\n",(u8 *)"OK",60000);//设置透传
	if(Retval == 1)
	{
		return 1;
	}
	else
	{
	}
	
	return 0;
}

u8 ESP8266_ConnectWIFI(u8 *SSID,u8 *Password)
{
	u8 Retval;
	u8 Buff[100]={"AT+CWJAP_DEF="};//连接AP 保存到flash
	
	Retval = ESP8266_SendATCmd((u8 *)"AT+CWQAP\r\n",(u8 *)"OK",1000);//断开与AP的连接
	if(Retval == 1)
	{
		return 1;
	}
	else
	{
		
	}
	
	strcat((char *)Buff,"\"");
	strcat((char *)Buff,(const char *)SSID);
	strcat((char *)Buff,"\",\"");
	strcat((char *)Buff,(const char *)Password);
	strcat((char *)Buff,"\"\r\n");
		
	Retval = ESP8266_SendATCmd((u8 *)Buff,(u8 *)"OK",60000);//连接AP 保存到flash
	if(Retval == 1)
	{
		return 1;
	}
	else
	{
		
	}
	
	return 0;
}



u8 ESP8266_ConnetServer(u8* tppe,u8 *RemoteIP,u8 *RemotePort)
{
	u8 Retval;
	
	u8 Buff[100]={"AT+CIPSTART="};
	
	strcat((char *)Buff,"\"");
	strcat((char *)Buff,(const char *)tppe);
	strcat((char *)Buff,"\",\"");
	strcat((char *)Buff,(const char *)RemoteIP);
	strcat((char *)Buff,"\",");
	strcat((char *)Buff,(const char *)RemotePort);
	strcat((char *)Buff,"\r\n");
	
    Retval = ESP8266_SendATCmd((u8 *)Buff,(u8 *)"OK",1000);//建立TCP连接，UDP传输或者SSL连接
	if(Retval == 1)
	{
		return 1;
	}
	else
	{
	}
	
	Retval = ESP8266_SendATCmd((u8 *)"AT+CIPSEND\r\n",(u8 *)"OK",1000);//启动发送数据
	
	if(Retval == 1)
	{
		return 1;
	}
	else
	{
	}
	
	return 0;
}

u8 ESP8266_HTTPRequestGet(u8 *RemoteIP)
{
	u8 Retval;
	
	u8 DeviceSN[30];
	
	u8 Buff[100];
	
	sprintf((char *)DeviceSN,
		"0X%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		SysInfo.SysSN[11],
		SysInfo.SysSN[10],
		SysInfo.SysSN[9],
		SysInfo.SysSN[8],
		SysInfo.SysSN[7],
		SysInfo.SysSN[6],
		SysInfo.SysSN[5],
		SysInfo.SysSN[4],
		SysInfo.SysSN[3],
		SysInfo.SysSN[2],
		SysInfo.SysSN[1],
		SysInfo.SysSN[0]);
	
	sprintf((char *)Buff,(const char *)"GET /cgi-bin/getconfig.cgi?deviceid=%s HTTP/1.1\r\nHost: %s\r\nConnection: Keep-Alive\r\nUser-Agent: MWH\r\n\r\n",DeviceSN,RemoteIP);


	Retval = ESP8266_SendATCmd(Buff,(u8 *)"HTTP/1.0 200 OK",1000);
	
	Delay_ms(1000);
		
	if(Retval == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

s16 DataPoint_StringToDec(u8 *Buff)
{
	s16 Data = 0;
	s8 Flag = 1;
	
	while(*Buff!=',' && (*Buff!='\r' && *(Buff+1)!='\n'))
	{
		if(*Buff=='-')
		{
			Flag = -1;
		}
		else
		{
			Data= Data*10 + (*Buff-'0');
		}
		Buff++;
	}
	
	Data *= Flag;
	
	return Data;
}

u8 ESP8266_GetEventInfo(SysInfoStu SysInfo)
{
	char DeviceSN[30];
	sprintf(DeviceSN,
		"0X%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		SysInfo.SysSN[11],
		SysInfo.SysSN[10],
		SysInfo.SysSN[9],
		SysInfo.SysSN[8],
		SysInfo.SysSN[7],
		SysInfo.SysSN[6],
		SysInfo.SysSN[5],
		SysInfo.SysSN[4],
		SysInfo.SysSN[3],
		SysInfo.SysSN[2],
		SysInfo.SysSN[1],
		SysInfo.SysSN[0]);
	
	char *Addr = NULL;
	u8 ID;
	char Pattern[100];
	
	DataPointStu DataPonit_Temp;
	
	/* 检验设备SN */
	Addr = strstr((const char *)RecvBuff,(const char *)"DEVICE_SN:");
	
	if(Addr != NULL)
	{
		Addr = Addr + strlen("DEVICE_SN:");
	
		if(strstr((const char *)RecvBuff,DeviceSN) != NULL)
		{
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
	
	/* 获取EventID */
	Addr = strstr((const char *)RecvBuff,(const char *)"ID:");
	if(Addr == NULL)
	{
		return 1;
	}
	else
	{
		Addr = Addr + strlen("ID:");
		ID = DataPoint_StringToDec((u8 *)Addr);
	}
	
	/* 受控事件数据 */
	Addr = strstr((const char *)RecvBuff,(const char *)"DATAPOINT_VALUE:");
	if(Addr == NULL)
	{
		return 1;
	}
	else
	{
	}
	
	/* 提取事件数据 */
	for(u32 i=0;i<Event_Max;i++)
	{
		if((ID & (0x01 << i)) != 0)
		{
			sprintf(Pattern,"V%d:",i+1);
			Addr = strstr((const char *)RecvBuff,(const char *)Pattern);
			Addr = Addr + strlen(Pattern);
			
			switch(SmartHome_EventReg[i])
			{
				case TypeOfs8:
					DataPonit_Temp.S8_DataPoint = DataPoint_StringToDec((u8 *)Addr);
					if(DataPonit_Temp.S8_DataPoint != EventInfo[i].DataPonit.S8_DataPoint)
					{
						EventInfo[i].DataPonitType = TypeOfs8; 
						EventInfo[i].DataPonit.S8_DataPoint = DataPonit_Temp.S8_DataPoint;
						EventInfo[i].Update = 1;
					}
					else
					{
						EventInfo[i].Update = 0;
					}
					break;
				case TypeOfu8:
					DataPonit_Temp.U8_DataPoint = DataPoint_StringToDec((u8 *)Addr);
					if(DataPonit_Temp.U8_DataPoint != EventInfo[i].DataPonit.U8_DataPoint)
					{
						EventInfo[i].DataPonitType = TypeOfu8; 
						EventInfo[i].DataPonit.U8_DataPoint = DataPonit_Temp.U8_DataPoint;
						EventInfo[i].Update = 1;
					}
					else
					{
						EventInfo[i].Update = 0;
					}
					break;
			}
		}
		else
		{
		}
	}
	
	memset(RecvBuff,0,sizeof(RecvBuff));
	RecvCount = 0;
	
	return 0;
}

void ESP8266_EventInfoHandle(SysInfoStu *SysInfo)
{
	/* 提取事件数据 */
	for(u32 i=0;i<Event_Max;i++)
	{
		if(EventInfo[i].Update != 0)
		{
			switch(i)
			{
				case Event_1://  RGB RedValue TypeOfu8
					SysInfo->RGB_LED.Red = EventInfo[i].DataPonit.U8_DataPoint;
					break;
				case Event_2://  RGB GreenValue TypeOfu8
					SysInfo->RGB_LED.Green = EventInfo[i].DataPonit.U8_DataPoint;
					break;
				case Event_3://  RGB BlueValue TypeOfu8
					SysInfo->RGB_LED.Blue = EventInfo[i].DataPonit.U8_DataPoint;
					break;
				case Event_4://  Fan TypeOfs8,
					SysInfo->FanMotor = EventInfo[i].DataPonit.S8_DataPoint;
					break;
				case Event_5://  Crutain TypeOfu8
					SysInfo->Curtain.TargetPlace = EventInfo[i].DataPonit.U8_DataPoint;
					break;
				case Event_6://  Socket TypeOfu8
					SysInfo->Socket = EventInfo[i].DataPonit.U8_DataPoint;
					break;
			}
		}
		else
		{
		}
	}
}

u8 ESP8266_HTTPRequestReport(u8 *RemoteIP,SysInfoStu SysInfo)
{
	u8 ReportBuff[512] = {0};
	
	u8 DeviceSN[40];
	sprintf((char *)DeviceSN,
		"0X%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		SysInfo.SysSN[11],
		SysInfo.SysSN[10],
		SysInfo.SysSN[9],
		SysInfo.SysSN[8],
		SysInfo.SysSN[7],
		SysInfo.SysSN[6],
		SysInfo.SysSN[5],
		SysInfo.SysSN[4],
		SysInfo.SysSN[3],
		SysInfo.SysSN[2],
		SysInfo.SysSN[1],
		SysInfo.SysSN[0]);
	
	//上报设备序列号及设备当前所有状态
	sprintf((char *)ReportBuff,(const char *)"GET /cgi-bin/getstatus.cgi?deviceid=%s&temperature=%02f&humidity=%02f&ledclore=%02x%02x%02x&motor=%d&step_motor=%d&relay=%d HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
		DeviceSN,
		SysInfo.Temper,
		SysInfo.Humidity,
		SysInfo.RGB_LED.Red,
		SysInfo.RGB_LED.Green,
		SysInfo.RGB_LED.Blue,
		SysInfo.FanMotor,
		SysInfo.Curtain.CurrentPlace,
		SysInfo.Socket,
		RemoteIP);
	
	
	
	UART2_SendString(ReportBuff);

	Delay_ms(500);

	
	return 0;
}



/**
  * @}
  */
