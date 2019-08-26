/**
******************************************************************************
  * @file       time.c
  * @brief      �򵥼�ʱ����Դ�ļ�
  * @version    2.0
  * @date       Sun 18-08-2019
  * @update     Ϊsystick��ʱ��������ӹ��������Լ��ж��ֳ�����
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "time.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u8 SystickLock = 0;
static u8 SystickValueSave = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  ����ʱ����
  * @note   72MHz
  * @param  ��ʱʱ��
  * @return NULL
  */
void Delay(u32 t)
{
    while (t--)
    {
        for (u32 i = 0; i < 100; i++)
        {
            for (u32 j = 0; j < 100; j++)
            {
            }
        }
    }

    return;
}

void delay_ms(u32 xms)
{
    if (SystickLock == 1)
    {
        if (SystickValueSave == 0)
        {
            SystickValueSave = SysTick->LOAD - SysTick->VAL;
        }
        else
        {
        }

        SysTick->VAL = 0;
        SysTick->LOAD = (72000 / 8) * xms;
        SysTick->CTRL |= (1 << 0); //������ʱ
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //��ֹ��ʱ

        SysTick->VAL = 0;
        SysTick->LOAD = SystickValueSave + 1;
        SysTick->CTRL |= (1 << 0); //������ʱ

        return;
    }
    else
    {
        SystickLock = 1;

        SysTick->VAL = 0;
        SysTick->LOAD = (72000 / 8) * xms;
        SysTick->CTRL |= (1 << 0); //������ʱ
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //��ֹ��ʱ

        SystickValueSave = 0;

        SystickLock = 0;
    }

    return;
}

void delay_us(u32 xus)
{
    //

    if (SystickLock == 1)
    {
        if (SystickValueSave == 0)
        {
            SystickValueSave = SysTick->LOAD - SysTick->VAL;
        }
        else
        {
        }

        SysTick->VAL = 0;
        SysTick->LOAD = (72 / 8) * xus;
        SysTick->CTRL |= (1 << 0); //������ʱ
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //��ֹ��ʱ

        SysTick->VAL = 0;
        SysTick->LOAD = SystickValueSave + 1;
        SysTick->CTRL |= (1 << 0); //������ʱ

        return;
    }
    else
    {
        SystickLock = 1;

        SysTick->VAL = 0;
        SysTick->LOAD = (72 / 8) * xus;
        SysTick->CTRL |= (1 << 0); //������ʱ
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //��ֹ��ʱ

        SystickValueSave = 0;

        SystickLock = 0;

        return;
    }
}

/**
  * @}
  */
