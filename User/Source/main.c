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
	/* ����ȫ���ж� */
	__set_PRIMASK(1);
	
	/* �ж����ȼ����� */
	NVIC_Config();
	
	/* ����ʱ��ʹ�� */
	RCC_Config();
	
	LED_Init();
	
	/* ��ʼ��SP2 */
	SPI2_Init();
	
	/* ��ʼ��OLEDģ�鲢������ */
	OLED_Init();
	OLED_Config();
	OLED_Clear();

	/* ����ȫ���ж� */
	__set_PRIMASK(0); 
	
	ChipID_Display();
	
	while(1)
	{
		LED2_OR();
		delay_ms(1000);
	}
	
	/* No Retval */
}


static void NVIC_Config(void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	return ;
}

static void RCC_Config(void)
{
	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	return ;
}


void ChipID_Get(u8 *ChipID_Buff)
{
	const u8 *Ptr = (const u8*)(0x1FFFF7E8); 
	
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
