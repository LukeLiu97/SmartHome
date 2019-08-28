/**
******************************************************************************
  * @file       irda.c
  * @brief      
  * @version    1.0
  * @date       Aug-27-2019 Tue
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "irda.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
    KEY_NO    = 0x00,
    KEY_0     = 0x19,
    KEY_1     = 0x45,
    KEY_2     = 0x46,
    KEY_3     = 0x47,
    KEY_4     = 0x44,
    KEY_5     = 0x40,
    KEY_6     = 0x43,
    KEY_7     = 0x07,
    KEY_8     = 0x15,
    KEY_9     = 0x09,
    KEY_UP    = 0x18,
    KEY_DOWN  = 0x52,
    KEY_LEFT  = 0x08,
    KEY_RIGHT = 0x5A,
    KEY_OK    = 0x1C
}RemoteData_Type;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void IR_Cell_Init(void)
{
	/* Remote_Data PA1 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PA1 in  mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

u8 IR_Cell_ReadData(void)
{
	u8 Retval = 0;
	if(Remote.End != 0)
	{
		Remote.End = 0;
		switch((Remote.RemoteCode>>16) & 0xFF)
		{
			case KEY_0:
				Retval = '0';
				break;
			case KEY_1:
				Retval = '1';
				break;
			case KEY_2:
				Retval = '2';
				break;
			case KEY_3:
				Retval = '3';
				break;
			case KEY_4:
				Retval = '4';
				break;
			case KEY_5:
				Retval = '5';
				break;
			case KEY_6:
				Retval = '6';
				break;
			case KEY_7:
				Retval = '7';
				break;
			case KEY_8:
				Retval = '8';
				break;
			case KEY_9:
				Retval = '9';
				break;
			case KEY_UP:
				Retval = 24;// ^
				break;
			case KEY_DOWN:
				Retval = 25;// v
				break;
			case KEY_LEFT:
				Retval = 27;// <-
				break;
			case KEY_RIGHT:
				Retval = 26;// ->
				break;
			case KEY_OK:
				Retval = '\n';
				break;
			default:
				Retval = 0;
		}
	}
	else
	{
		Retval = 0;
	}
	
	return Retval;
}

/**
  * @}
  */
