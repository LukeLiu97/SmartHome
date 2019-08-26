/**
******************************************************************************
  * @file       led.c
  * @brief      LED��غ���Դ�ļ�
  * @version    1.1
  * @date       Tue 06-08-2019
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
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  LED��ʼ������
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

/**
  * @}
  */
