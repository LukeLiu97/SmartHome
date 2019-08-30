/**
******************************************************************************
  * @file       led.c
  * @brief      LED相关函数源文件
  * @version    2.0
  * @date       Aug-26-2019 Mon      
  * @update		
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RGB_LED_CLK_H() (GPIO_SetBits(GPIOB, GPIO_Pin_8))
#define RGB_LED_CLK_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_8))

#define RGB_LED_DATA_H() (GPIO_SetBits(GPIOB, GPIO_Pin_9))
#define RGB_LED_DATA_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_9))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  LED初始化函数
  * @param  NULL
  * @return NULL
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PB14 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Set pin */
    GPIO_SetBits(GPIOB, GPIO_Pin_14);

    return;
}

void RGB_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	/* RGB_CLK PB8 RGB_DATA PB9 */
    /* Configure PB8 and PB9 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	return ;
}

u32 RGB_LED_DataFrame(u8 Blue, u8 Green, u8 Red)
{
	u8 FirstByte = 0;
	u32 Retval = 0;
	
	FirstByte |= ((Blue		& 0xC0) 	>> 2);
	FirstByte |= ((Green	& 0xC0)	>> 4);
	FirstByte |= ((Red		& 0xC0)	>> 6);
	FirstByte = ~FirstByte;
	
	Retval |= FirstByte << (8 * 3);
	Retval |= Blue 		<< (8 * 2);
	Retval |= Green 	<< (8 * 1);
	Retval |= Red 		<< (8 * 0);
	
	return Retval;
}

void RGB_LED_Send8Byte(u32 DataFrame)
{
	for(u32 i = 0;i < 32;i++)
	{
		RGB_LED_CLK_L();
		RGB_LED_DATA_L();
		
		if((DataFrame & 0x80000000) != 0)
		{
			RGB_LED_DATA_H();
		}
		else
		{
			RGB_LED_DATA_L();
		}
		DataFrame <<= 1;
		
		RGB_LED_CLK_H();
		Delay_us(10);
		RGB_LED_CLK_L();
		Delay_us(10);
	}
	RGB_LED_CLK_L();
	RGB_LED_DATA_L();
	
	return ;
}

void RGB_LED_Control(RGBStu RGB)
{
	RGB_LED_Send8Byte(0);
	RGB_LED_Send8Byte(RGB_LED_DataFrame(RGB.Blue,RGB.Green,RGB.Red));
	
	return ;
}

/**
  * @}
  */
