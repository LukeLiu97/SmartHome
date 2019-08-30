/**
******************************************************************************
  * @file       motor.c
  * @brief      �������Դ�ļ�
  * @version    1.1
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motor.h"

/** @addtogroup Motor control drive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define M_OUT1(x) (GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)x))
#define M_OUT2(x) (GPIO_WriteBit(GPIOA,GPIO_Pin_6,(BitAction)x))
#define M_OUT3(x) (GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)x))
#define M_OUT4(x) (GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)x))

#define M_BEAT0() do{M_OUT1(0);M_OUT2(0);M_OUT3(0);M_OUT4(0);}while(0)
#define M_BEAT1() do{M_OUT1(1);M_OUT2(0);M_OUT3(0);M_OUT4(1);}while(0)
#define M_BEAT2() do{M_OUT1(0);M_OUT2(1);M_OUT3(0);M_OUT4(1);}while(0)
#define M_BEAT3() do{M_OUT1(0);M_OUT2(1);M_OUT3(1);M_OUT4(0);}while(0)
#define M_BEAT4() do{M_OUT1(1);M_OUT2(0);M_OUT3(1);M_OUT4(0);}while(0)
/* Private variables ---------------------------------------------------------*/
StepMotorStu TargetStepMotor;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  ֱ��������Ƴ�ʼ������
  * @param  NULL
  * @return NULL
  */
void Motor_Init(void)
{
	// ʹ��TIM3 PWM�������
    /* MOTOR_IA PB5 MOTOR_IB PB4(��JTAG���ų�ͻ����Ҫ��ӳ��) */
    GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);

    /* Configure PB4(CH1) and PB5(CH2) in TIM3 Alternate Function mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����TIM3 ���� */
	
    return ;
}

void Motor_Control(s8 Motor)
{
	if(Motor == 0)
	{
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,0);
		TIM_Cmd(TIM3,DISABLE);
	}
	else if(Motor > 0)
	{
		TIM_SetCompare1(TIM3,Motor * 8);
		TIM_SetCompare2(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
	}
	else
	{
		TIM_SetCompare1(TIM3,0);
		TIM_SetCompare2(TIM3,Motor * -1 * 8);
		TIM_Cmd(TIM3,ENABLE);
	}
	
	return ;
}



/**
  * @brief  ����������Ƴ�ʼ������
  * @param  NULL
  * @return NULL
  */
void StepMotor_Init(void)
{
    /* MOTOR_IA PA7 MOTOR_I^A PA6 MOTOR_IB PA5 MOTOR_I^B PA4 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PA4 and PA5 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Pull low thr pin voltage*/
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7);

    return;
}

void StepMotor_Roll(void)
{
	s8 Direction = SysInfo.Curtain.MoveDirection;
	static u8 CurrentBeat = 0;
	
	if(Direction != 0)
	{
		if(Direction == 1)
		{
			CurrentBeat++;
			if(CurrentBeat == 5)
			{
				CurrentBeat = 1;
			}
			else
			{
			}
		}
		else
		{
			CurrentBeat--;
			if(CurrentBeat == 0 || CurrentBeat > 5)
			{
				CurrentBeat = 4;
			}
			else
			{
			}
		}
	}
	else
	{
		CurrentBeat = 0;
	}
	
	switch(CurrentBeat)
	{
		case 1:
			M_BEAT1();
			break;
		case 2:
			M_BEAT2();
			break;
		case 3:
			M_BEAT3();
			break;
		case 4:
			M_BEAT4();
			break;
		default:
			M_BEAT0();
	}
}


/**
  * @}
  */
