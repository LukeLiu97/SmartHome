/**
******************************************************************************
  * @file       main.c
  * @brief      
  * @version    1.0
  * @date       Aug-26-2019 Mon
  * @update     
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void NVIC_Config(void);
static void RCC_Config(void);

void DebugMode_PrintfSysInfo(void);

void ChipID_Get(u8 *ChipID_Buff);
void ChipID_Display(void);

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
	
#ifdef DEBUG
	USART1_Init(115200);
	DebugMode_PrintfSysInfo();
#endif
	
	
	LED_Init();
	RGB_LED_Init();
	
	SPI2_Init();
	
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	TIM3_Interrupt_Init(1000);
	
	Step_Motor_Init();

	IR_Cell_Init();
	
	TIM2_InputCapture_Channel2_Init();
	
	__set_PRIMASK(0); 
	
	ChipID_Display();
	
	Delay(3000);
	
	while(1)
	{
		UserData_Updata();
		
		IR_Data = IR_Cell_ReadData();
		
		if(IR_Data >= '0' && IR_Data <= '9')
		{
			Curtain.TargetPlace = IR_Data - '0';
		}
		else if((IR_Data >= 24 && IR_Data <= 27) || IR_Data == '\n')
		{
			switch(IR_Data)
			{
				case 24:
					RGB.Blue = 100;
					RGB.Green = 0;
					RGB.Red = 0;
					break;
				case 25:
					RGB.Blue = 0;
					RGB.Green = 64;
					RGB.Red = 0;
					break;
				case 26:
					RGB.Blue = 0;
					RGB.Green = 0;
					RGB.Red = 64;
					break;
				case 27:
					RGB.Blue = 64;
					RGB.Green = 64;
					RGB.Red = 64;
					break;
				default:
					RGB.Blue = 0;
					RGB.Green = 0;
					RGB.Red = 0;
			}
		}
		
		RGB_LED_Control(RGB.Blue,RGB.Green,RGB.Red);

		delay_ms(1000);
	}
	
	/* No Retval */
}


static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable and set TIM2 Interrupt to the highest priority */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set TIM3 Interrupt to the high priority */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	return ;
}

static void RCC_Config(void)
{
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
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

void ChipID_Display(void)
{
	u8 ChipID[12];
	
	ChipID_Get(ChipID);
	
	for(u32 i = 0;i  < 3;i++)
	{
		for(u32 j = 0;j < 4;j++)
		{
			OLED_Show_XxN8_Character(i * 2 + 1,j * 8 * 2 + 32,2,8,&HexChar_8x16[ChipID[12 - (i * 4 + j)] >> 4][0]);
			OLED_Show_XxN8_Character(i * 2 + 1,j * 8 * 2 + 8 + 32,2,8,&HexChar_8x16[ChipID[12 - (i * 4 + j)]&0x0F][0]);
		}
		
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
