/**
******************************************************************************
  * @file       global.c
  * @brief      全局变量定义
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global.h"

/** @addtogroup Global variables
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/
u8 ReversalFlag = 0;

// CurtainStu Curtain ={10,0,0};

RemoteStu Remote = {0};

union UserData_ UserData;

u8 RecvBuff[1024] = {0};
u32 RecvCount = 0;
u8 RecvEnd = 0;

SysInfoStu SysInfo;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */
