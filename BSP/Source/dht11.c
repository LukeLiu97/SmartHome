/**
******************************************************************************
  * @file       dht11.c
  * @brief      DHT11 Temper Sensor Driver Source File
  * @version    1.0
  * @date       Aug-28-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dht11.h"

/** @addtogroup DHT11 Driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define DHT11_Data_H() (GPIO_SetBits(GPIOB, GPIO_Pin_3))
#define DHT11_Data_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_3))

#define DHT11_Data_Status() (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void DHT11_Init(void)
{
	/* DHT11_DATA PB3 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
    /* Configure PB3 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Set pin */
    GPIO_SetBits(GPIOB, GPIO_Pin_3);
	
	/* 需要等待1s后操作 */

    return;
}

void DHT11_Data_Input(void)
{
	/* DHT11_DATA PB3 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
    /* Configure PB3 in input mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    return;
}

void DHT11_Data_Output(void)
{
	/* DHT11_DATA PB3 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
    /* Configure PB3 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    return;
}

void DHT11_Start(void)
{
	DHT11_Data_Output();
	
	DHT11_Data_L();
	Delay_ms(20); // >18ms
	DHT11_Data_H();
//	Delay_us(30); // 20~40us
	
	return ;
}

u8 DHT11_Data_ContinueSampling(void)
{
	u8 i,Cnt = 0;
	
	for(i = 0;i < 5;i++)
	{
		__NOP;
		if(DHT11_Data_Status() == RESET)
		{
			Cnt++;
		}
		else
		{
		}
		__NOP;
	}
	
	if(Cnt > 3)
	{
		return RESET;
	}
	else
	{
		return SET;
	}
	
}

u8 DHT11_GetAck(void)
{
	u8 Count = 0;
	
	DHT11_Data_Input();
	while(DHT11_Data_ContinueSampling() == SET)
	{
		Delay_us(1);
		if(++Count > 200)
		{
			return 1;
		}
		else
		{
		}
	}
	
	while(DHT11_Data_ContinueSampling() == RESET)
		;
	while(DHT11_Data_ContinueSampling() == SET)
		;
	
	return 0;
}

u8 DHT11_RecvByte(void)
{
	/* 0 50us L + 26~28us H 1 50us L + 70us H */
	u8 Data = 0;
	
	DHT11_Data_Input();
	
	for(u32 i = 0;i < 8;i++)
	{
		while(DHT11_Data_ContinueSampling() != SET);
		Delay_us(40);
		if(DHT11_Data_ContinueSampling() == RESET)
		{
			Data <<= 1;
			Data &= ~(0x01);
		}
		else
		{
			Data <<= 1;
			Data |= (0x01);
		}
		while(DHT11_Data_ContinueSampling() != RESET);
	}
	
	return Data;
}

u8 DHT11_TemperAndHumidity(float *Temperature,float *Humidity)
{
	u8 TemperMantissa,TemperFraction,HumidityMantissa,HumidityFraction,CheckSum;
	
	DHT11_Start();
	
	if(DHT11_GetAck() != 0)
	{
		return 1;
	}
	else
	{
		HumidityMantissa = DHT11_RecvByte();
		HumidityFraction = DHT11_RecvByte();// 始终为0x00
		TemperMantissa = DHT11_RecvByte();
		TemperFraction = DHT11_RecvByte();
		CheckSum = DHT11_RecvByte();
		
		DHT11_Data_Output();
		DHT11_Data_H();
		Delay_us(50);
		
		if(CheckSum != ((TemperMantissa+TemperFraction+HumidityMantissa+HumidityFraction)&0xFF))
		{
			return 1;
		}
		else
		{
			*Temperature = TemperMantissa + TemperFraction/10.0;
			*Humidity = HumidityMantissa;
			return 0;
		}
	}
}

/**
  * @}
  */
