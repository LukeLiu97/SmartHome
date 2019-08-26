/**
******************************************************************************
  * @file       tim.c
  * @brief      ͨ�ö�ʱ������Դ�ļ�
  * @version    1.1
  * @date       Aug-12-2019 Mon
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/** @addtogroup General-purpose timer 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void TIM2_Delay_ms(u16 ms)
{
    uint16_t PrescalerValue = 0;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t)(SystemCoreClock / 10000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = ms * 10 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);

    while (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET)
    {
    }

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, DISABLE);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

void TIM3_Interrupt_Init(u16 us)
{
    uint16_t PrescalerValue = 0;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t)(SystemCoreClock / 1000000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = us - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

    TIM_ARRPreloadConfig(TIM3,ENABLE);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* TIM IT enable */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Delay_us(u16 us)
{
    uint16_t PrescalerValue = 0;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t)(SystemCoreClock / 1000000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = us - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);

    /* TIM4 enable counter */
    TIM_Cmd(TIM4, ENABLE);

    while (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET)
    {
    }

    /* TIM4 enable counter */
    TIM_Cmd(TIM4, DISABLE);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
}


/**
  * @}
  */
