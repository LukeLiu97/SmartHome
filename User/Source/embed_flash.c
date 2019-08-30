/**
******************************************************************************
  * @file       embed_flash.c
  * @brief      
  * @version    1.0
  * @date       Aug-27-2019 Tue
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "embed_flash.h"

/** @addtogroup User Flash
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

u8 EmbedFlash_CheckSafe(u32 Addr,size_t DataSize)
{
	if(Addr >= USER_FLASH_START_ADDR && (Addr + DataSize - 1) <= USER_FLASH_END_ADDR)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

u8 EmbedFlash_Write(u32 Addr,const u32 *Data,size_t DataSize)
{
	u32 FLASH_Status = FLASH_COMPLETE;
	
	if(EmbedFlash_CheckSafe(Addr,DataSize) != 0)
	{
		return 1;
	}
	else
	{
		__set_PRIMASK(1);
		FLASH_Unlock();
		
		/* Clear All pending flags */
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		
		FLASH_Status = FLASH_ErasePage(Addr - (Addr - 0x80000000) % FLASH_PAGE_SIZE);
		if(FLASH_Status != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return 1;
		}
		else
		{}
		
//		while(DataSize != 0)
//		{
			if((FLASH_Status = FLASH_ProgramWord(Addr,*Data)) != FLASH_COMPLETE)
			{
				FLASH_Lock();
				return 1;
			}
			else
			{
				DataSize--;
			}
//		}
		FLASH_Lock();
		__set_PRIMASK(0);
	}
	
	
	return 0;
}

//void UserData_Update(void)
//{
//	static u8 RebootFlag = 0;
//	if(RebootFlag == 0)
//	{
//		RebootFlag = 1;
//		
//		UserData.Data = *(u32 *)(USER_FLASH_START_ADDR);
//		
//		Curtain.TargetPlace = UserData.DataArray[0];
//		RGB.Blue = UserData.DataArray[1];
//		RGB.Green = UserData.DataArray[2];
//		RGB.Red = UserData.DataArray[3];
//	}
//	else
//	{
//		UserData.DataArray[0] = Curtain.TargetPlace;
//		UserData.DataArray[1] = RGB.Blue;
//		UserData.DataArray[2] = RGB.Green;
//		UserData.DataArray[3] = RGB.Red;
//	
//		if(*(u32 *)(USER_FLASH_START_ADDR) != UserData.Data)
//		{
//			EmbedFlash_Write(USER_FLASH_START_ADDR,&(UserData.Data),sizeof(UserData));
//		}
//		else
//		{
//		}
//	}
//}

/**
  * @}
  */
