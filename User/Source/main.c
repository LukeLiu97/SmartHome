/**
******************************************************************************
  * @file       main.c
  * @brief      ������Դ�ļ�
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
	u8 Red = 0;
	/* ����ȫ���ж� */
	__set_PRIMASK(1);
	
	/* �ж����ȼ����� */
	NVIC_Config();
	
	/* ����ʱ��ʹ�� */
	RCC_Config();
	
	LED_Init();
	RGB_LED_Init();
	
	/* ��ʼ��SP2 */
	SPI2_Init();
	
	/* ��ʼ��OLEDģ�鲢������ */
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	TIM3_Interrupt_Init(1000);
	
	Step_Motor_Init();

	/* ����ȫ���ж� */
	__set_PRIMASK(0); 
	
	ChipID_Display();
	
	while(1)
	{
		RGB_LED_Control(0,0,Red++);
		if(Red > 64)
		{
			Red = 0;
		}
		else
		{
		}
		delay_ms(1000);
		
		switch(Curtain.CurrentPlace)
		{
			case 0:
				Curtain.TargetPlace = 4;
				break;
			case 4:
				Curtain.TargetPlace = 6;
				break;
			case 6:
				Curtain.TargetPlace = 8;
				break;
			case 8:
				Curtain.TargetPlace = 0;
				break;
		}
	}
	
	/* No Retval */
}


static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable and set TIM3 Interrupt to the highest priority */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
	
	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
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

//#ifdef DEBUG
////Redirect fputc function
//int fputc(int ch,FILE *stream)
//{
//	USART_SendData(USART1,ch);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//	{
//	}
//	
//	return ch;
//}

//#endif
    
/**
  * @}
  */
