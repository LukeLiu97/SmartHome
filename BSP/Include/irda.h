/**
******************************************************************************
  * @file       irda.h
  * @brief      
  * @version    1.0
  * @date       Aug-27-2019 Tue
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IRDA_H
#define __IRDA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "global.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define RemoteData_Status() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

/* Exported functions ------------------------------------------------------- */
void IR_Cell_Init(void);
u8 IR_Cell_ReadData(void);

#ifdef __cplusplus
}
#endif

#endif
