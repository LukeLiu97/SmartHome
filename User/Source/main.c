/**
******************************************************************************
  * @file       main.c
  * @brief      智能家居工程源文件
  * @version    1.0
  * @date       Aug-26-2019 Mon
  * @update     
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup main 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);
static void NVIC_Config(void);
static void RCC_Config(void);
static void Peripherals_Init(void);

void DebugMode_PrintfSysInfo(void);
void ChipID_Get(u8 *ChipID_Buff);
void OLED_Display(void);
void Remote_Control(u8 IR_Value);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	u8 IR_Data = 0;
	
	__set_PRIMASK(1);
	
	NVIC_Config();
	RCC_Config();
	JTAG_Disable();
	
#ifdef DEBUG
	USART1_Init(115200);
	DebugMode_PrintfSysInfo();
#endif
	
	Peripherals_Init();
	
	__set_PRIMASK(0); 
	
	ChipID_Get(SysInfo.SysSN);
	
	/* 复位步进电机 */
	SysInfo.Curtain.CurrentPlace = 12;
	
	Delay_ms(500);
	
	SysInfo.WIFI_Status |= ESP8266_HTTP_Init();
	
	SysInfo.WIFI_Status |= ESP8266_ConnectWIFI((u8 *)"MI 6",(u8 *)"25892589");

	SysInfo.WIFI_Status |= ESP8266_ConnetServer((u8*)"TCP",(u8*)"134.175.80.178",(u8*)"8080");
	

	while(1)
	{
#ifdef RELEASE
		UserData_Update();
#endif
		SysInfo.WIFI_Status |= ESP8266_HTTPRequestGet((u8*)"134.175.80.178");
		
		SysInfo.WIFI_Status |= ESP8266_GetEventInfo(SysInfo);
		
		ESP8266_EventInfoHandle(&SysInfo);
		
		DHT11_TemperAndHumidity(&SysInfo.Temper,&SysInfo.Humidity);
		
		IR_Data = IR_Cell_ReadData();	
		Remote_Control(IR_Data);
		
		Motor_Control(SysInfo.FanMotor);
		RGB_LED_Control(SysInfo.RGB_LED);
	
		SysInfo.WIFI_Status |= ESP8266_HTTPRequestReport((u8*)"134.175.80.178",SysInfo);
				
		OLED_Display();
		
	}
	
	/* No Retval */
}

static void JTAG_Disable(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	return ;
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable and set TIM2 Interrupt to the highest priority */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set TIM4 Interrupt to the high priority */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set USART1 Interrupt to the middle priority */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	return ;
}

static void RCC_Config(void)
{
	/* AFIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* USART2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	return ;
}

static void Peripherals_Init(void)
{
	USART2_Init(115200);
	
	LED_Init();
	RGB_LED_Init();
	
	SPI2_Init();
	
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	TIM4_Interrupt_Init(1000);
	
	StepMotor_Init();

	IR_Cell_Init();
	
	TIM2_InputCapture_Channel2_Init();
	
	Motor_Init();
	
	TIM3_PWM_Init(1000);
	
	DHT11_Init();
}

void DebugMode_PrintfSysInfo(void)
{
#ifdef DEBUG
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);
	
	printf("-----------------------------------------------\r\n");
	printf("Debug Mode\r\n");
	printf("SYSCLK_Frequency:\t%d\r\n",RCC_Clocks.SYSCLK_Frequency);
	printf("HCLK_Frequency:\t\t%d\r\n",RCC_Clocks.HCLK_Frequency);
	printf("PCLK1_Frequency:\t%d\r\n",RCC_Clocks.PCLK1_Frequency);
	printf("PCLK2_Frequency:\t%d\r\n",RCC_Clocks.PCLK2_Frequency);
	printf("ADCCLK_Frequency:\t%d\r\n",RCC_Clocks.ADCCLK_Frequency);
	printf("-----------------------------------------------\r\n");
#endif
	
	return ;
}


void ChipID_Get(u8 *ChipID_Buff)
{
	volatile const u8 *Ptr = (__I u8*)(0x1FFFF7E8); 
	
	for(u32 i = 0;i < 12;i++)
	{
		*(ChipID_Buff + i) = *(Ptr + i);
	}
	return ;
}

void OLED_Display(void)
{
	u8 Temper = SysInfo.Temper,Humidity = SysInfo.Humidity;
	
	/* 显示序列号 */
	OLED_ShowString(0,2,64,&SerialNumberString_16x16[0][0],4);
	
	for(u32 i = 0;i  < 3;i++)
	{
		for(u32 j = 0;j < 4;j++)
		{
			OLED_Show_XxN8_Character(i * 2 + 2,j * 8 * 2 + 64,2,8,&HexChar_8x16[SysInfo.SysSN[11 - (i * 4 + j)] >> 4][0]);
			OLED_Show_XxN8_Character(i * 2 + 2,j * 8 * 2 + 8 + 64,2,8,&HexChar_8x16[SysInfo.SysSN[11 - (i * 4 + j)]&0x0F][0]);
		}
	}
	
	/* 显示温度与湿度 */
	OLED_ShowString(0,2,0,&TemperString_16x16[0][0],2);
	OLED_Show_XxN8_Character(0,32,2,8,&HexChar_8x16[Temper/10][0]);
	OLED_Show_XxN8_Character(0,32+8,2,8,&HexChar_8x16[Temper%10][0]);
	OLED_Show_XxN8_Character(0,32+16,2,16,& SymbolChar_16x16[0][0]);
	OLED_ShowString(2,2,0,&HumidityString_16x16[0][0],2);
	OLED_Show_XxN8_Character(2,32,2,8,&HexChar_8x16[Humidity/10][0]);
	OLED_Show_XxN8_Character(2,32+8,2,8,&HexChar_8x16[Humidity%10][0]);
	OLED_Show_XxN8_Character(2,32+16,2,16,& SymbolChar_16x16[1][0]);
	
	/* WIFI 状态 */
	OLED_ShowString(4,2,0,&NetStatusString_16x16[0][0],4);
	if(SysInfo.WIFI_Status == 0)
	{
		OLED_ShowString(6,2,0,&SuccessString_16x16[0][0],2);
	}
	else
	{
		OLED_ShowString(6,2,0,&FailString_16x16[0][0],2);
		SysInfo.WIFI_Status  = 0;
	}
	
	return ;
}

void Remote_Control(u8 IR_Value)
{
	if(IR_Value >= '0' && IR_Value <= '9')
	{
		SysInfo.Curtain.TargetPlace = IR_Value - '0';
	}
	else if(IR_Value >= 24 && IR_Value <= 27)
	{
		switch(IR_Value)
		{
			case 24:
				SysInfo.RGB_LED.Blue = 64;
				SysInfo.RGB_LED.Green = 0;
				SysInfo.RGB_LED.Red = 0;
				break;
			case 25:
				SysInfo.RGB_LED.Blue = 0;
				SysInfo.RGB_LED.Green = 64;
				SysInfo.RGB_LED.Red = 0;
				break;
			case 26:
				SysInfo.RGB_LED.Blue = 0;
				SysInfo.RGB_LED.Green = 0;
				SysInfo.RGB_LED.Red = 64;
				break;
			case 27:
				SysInfo.RGB_LED.Blue = 64;
				SysInfo.RGB_LED.Green = 64;
				SysInfo.RGB_LED.Red = 64;
				break;
		}
	}
	else if(IR_Value == '\n')
	{
		SysInfo.RGB_LED.Blue = 0;
		SysInfo.RGB_LED.Green = 0;
		SysInfo.RGB_LED.Red = 0;
		SysInfo.Curtain.TargetPlace = 0;
		SysInfo.FanMotor = 0;
	}
	else
	{
	}
	
	return ;
}

/* Exported functions --------------------------------------------------------*/

#ifdef DEBUG
//Redirect fputc function
int fputc(int ch,FILE *stream)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ch;
}

#endif
    
/**
  * @}
  */
