/**
******************************************************************************
  * @file       tim.c
  * @brief      通用定时器驱动源文件
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

void TIM2_InputCapture_Channel2_Init(void)
{
	 uint16_t PrescalerValue = 0;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t)(SystemCoreClock / 1000000) - 1;

	TIM_DeInit(TIM2);
	
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 50000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //通道2
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;  
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI2上 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//无预分频器，捕获输入口上检测到的每一个边沿都触发一次捕获
	TIM_ICInitStructure.TIM_ICFilter = 0x0F; //滤波器
	TIM_ICInit(TIM2, &TIM_ICInitStructure); //初始化输入捕获

	TIM_ClearFlag(TIM2, TIM_FLAG_Update | TIM_FLAG_CC2);   //清除中断标志
	
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE); //开启TIM2的中断
	
	/* 不使用影子寄存器 */
    TIM_ARRPreloadConfig(TIM2, DISABLE);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
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
