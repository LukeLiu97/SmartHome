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
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //ͨ��2
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;  
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI2�� 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//��Ԥ��Ƶ��������������ϼ�⵽��ÿһ�����ض�����һ�β���
	TIM_ICInitStructure.TIM_ICFilter = 0x0F; //�˲���
	TIM_ICInit(TIM2, &TIM_ICInitStructure); //��ʼ�����벶��

	TIM_ClearFlag(TIM2, TIM_FLAG_Update | TIM_FLAG_CC2);   //����жϱ�־
	
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE); //����TIM2���ж�
	
	/* ��ʹ��Ӱ�ӼĴ��� */
    TIM_ARRPreloadConfig(TIM2, DISABLE);

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

void TIM3_PWM_Init(u16 us)
{
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, DISABLE);	
	
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

	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
//    TIM_ARRPreloadConfig(TIM3,ENABLE);
	
//	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
	TIM_Cmd(TIM3,DISABLE);
}

void TIM4_Interrupt_Init(u16 us)
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

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);

    TIM_ARRPreloadConfig(TIM4,ENABLE);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);

    /* TIM IT enable */
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /* TIM4 enable counter */
    TIM_Cmd(TIM4, ENABLE);
}




/**
  * @}
  */
