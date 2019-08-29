/**
******************************************************************************
  * @file       dht11.h
  * @brief      DH11 Temper Sensor Driver Header File
  * @version    1.0
  * @date       Aug-28-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "time.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DHT11_Init(void);
u8 DHT11_TemperAndHumidity(float *Temperature,float *Humidity);

#ifdef __cplusplus
}
#endif

#endif
