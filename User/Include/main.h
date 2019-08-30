/**
******************************************************************************
  * @file       main.h
  * @brief      主程序头文件
  * @note       
  * @version    1.1
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include <stdio.h>
#include "global.h"
#include "led.h"
#include "uart.h"
#include "time.h"
#include "spi.h"
#include "oled.h"
#include "font.h"
#include "motor.h"
#include "irda.h"
#include "embed_flash.h"
#include "dht11.h"
#include "esp8266.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif
