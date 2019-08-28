/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Extern variables ----------------------------------------------------------*/
//extern u16 gTouchStatus;
//extern u8 CurrentWindowMode;
//extern u32 UnBusy_Count;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SHOW_TOUCHSTATUS

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	u16 HighVoltage_Time = 0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		/* Interrupt task */
		// 当边沿触发一一直未产生，计数上溢,通讯空闲
		if(Remote.Start != 0)
		{
			Remote.Start = 0;
			Remote.End = 1;
		}
		
		/* Clears the TIM2 interrupt update pending bit. */
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
	else
	{
	}
	
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET)
	{
		/* Interrupt task */
		
		// 恢复为高电平
		if(RemoteData_Status() == SET)
		{
			TIM_SetCounter(TIM2,0);
			TIM2->CCER |= (0x01 << 5);
			TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_Low);
		}
		// 恢复为低电平
		else
		{
			HighVoltage_Time = TIM_GetCapture2(TIM2);
			TIM_OC2PolarityConfig(TIM2,TIM_OCPolarity_High);
			TIM_SetCounter(TIM2,0);
			
			if(Remote.Start == 0)
			{
				// TODO Start 4500us
				if(HighVoltage_Time > 4300 && HighVoltage_Time < 4700)
				{
					Remote.Start = 1;
					TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
				}
				else
				{
				}
			}
			else
			{
				if (HighVoltage_Time > 460 && HighVoltage_Time < 660)// TODO Bit 0 560us
				{
					Remote.RemoteCode >>= 1;
					Remote.RemoteCode &= ~(0x80000000);
				}
				else if(HighVoltage_Time > 1580 && HighVoltage_Time < 1780)// TODO Bit 1 1680us
				{
					Remote.RemoteCode >>= 1;
					Remote.RemoteCode |= (0x80000000);
				}
				else
				{
				}
			}
			
		}
		/* Clear TIM2 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
		
	}
	else
	{
	}
}

/**
  * @brief  This function handles TIM4 interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
	static u32 Laps = 0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
	{
		/* Interrupt task */
		if(Curtain.TargetPlace > Curtain.CurrentPlace)
		{
			Curtain.MoveDirection = 1;
		}
		else if(Curtain.TargetPlace < Curtain.CurrentPlace)
		{
			Curtain.MoveDirection = -1;
		}
		else
		{
			Curtain.MoveDirection = 0;
		}
		
		Step_Motor_Roll();
		
		if(Laps > 80)
		{
			Laps = 0;
			Curtain.CurrentPlace += Curtain.MoveDirection;
		}
		else
		{
			Laps++;
		}
		
		
		/* Clears the TIM4 interrupt update pending bit. */
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
