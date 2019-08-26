/**
******************************************************************************
  * @file       at24c04.c
  * @brief      AT24C04 drive source file
  * @version    1.1
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at24c04.h"

/** @addtogroup AT24C04 Drive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void AT24C04_Init(void)
{
    /* �����ݲ�ʹ�ã��ú���ִ��ǰ���ȳ�ʼ��KEY */

    return;
}

u8 AT24C04_ReadByte(const u8 ByteAddress, u8 *ReadData)
{
    IIC_Start();
    IIC_SendByte(AT24C04_WRITE_ADDR);
    if (IIC_CheckACK() != 1)
    {
        return 1;
    }
    else
    {
    }
    IIC_SendByte(ByteAddress);
    if (IIC_CheckACK() != 1)
    {
        return 1;
    }
    else
    {
    }

    IIC_Start();
    IIC_SendByte(AT24C04_READ_ADDR);
    if (IIC_CheckACK() != 1)
    {
        return 1;
    }
    else
    {
    }
    *ReadData = IIC_RecvByte(0);
    IIC_CheckACK();

    IIC_Stop();

    return 0;
}

// ����ִ�к���ʱһ��ʱ��
u8 AT24C04_WriteByte(const u8 ByteAddress, const u8 WriteData)
{
    IIC_Start();
    IIC_SendByte(AT24C04_WRITE_ADDR);
    if (IIC_CheckACK() != 1)
    {
        return 1;
    }
    else
    {
    }
    IIC_SendByte(ByteAddress);
    if (IIC_CheckACK() != 1)
    {
        return 1;
    }
    else
    {
    }
    IIC_SendByte(WriteData);
    if (IIC_CheckACK() != 1)
    {
        return 1;
    }
    else
    {
    }
    IIC_Stop();

    /* ����ȴ�һ��ʱ�� */
    Delay(10);

    return 0;
}

u8 AT24C04_WriteBuff(u8 Addr, const u8 *Buff, u8 Size)
{
    for (u32 i = 0; i < Size; i++)
    {
        if (AT24C04_WriteByte(Addr++, *(Buff + i)) != 0)
        {
            return 1;
        }
        else
        {
        }
    }

    return 0;
}

u8 AT24C04_ReadBuff(u8 Addr, u8 *Buff, u8 Size)
{
    for (u32 i = 0; i < Size; i++)
    {
        if (AT24C04_ReadByte(Addr++, Buff + i) != 0)
        {
            return 1;
        }
        else
        {
        }
    }

    return 0;
}

// ����д��0 �ظ�д��1 ��ȡʧ��2 д��ʧ��3 д�����ݴ���4
u8 AT24C04_Save16Byte(const u8 Addr, const u8 Data[16])
{
    u8 ReadTemp[16] = {0};
    if (AT24C04_ReadBuff(Addr, ReadTemp, 16) != 0)
    {
        return 2;
    }
    else
    {
        if (HashCompare(Data, ReadTemp, 16) == 0)
        {
            // ����һ�²����ظ�д��
            return 1;
        }
        else
        {
            if (AT24C04_WriteBuff(Addr, Data, 16) != 0)
            {
                return 3;
            }
            else
            {
                if (AT24C04_ReadBuff(Addr, ReadTemp, 16) != 0)
                {
                    return 2;
                }
                else
                {
                    /* д�����ݲ�һ�� */
                    if (HashCompare(Data, ReadTemp, 16) != 0)
                    {
                        return 4;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }
}

void AT24C04_SaveSmartLockData(void)
{
    u8 WriteOK;

    /* ����ȫ���ж� */
    __set_PRIMASK(1);

    if (SmartLock.MemoryFlag == SET)
    {
        u8 Data[16] = {0};
        *(Data) = EEPROM_ENABLE_FLAG;
        *(Data + EEPROM_MuteMode_Addr) = SmartLock.MuteMode;
        memcpy(Data + EEPROM_UserPassword_Addr, SmartLock.UserPassword, sizeof(SmartLock.UserPassword[0]) * 6);
        memcpy(Data + EEPROM_OFFSET, Data, sizeof(u8) * EEPROM_OFFSET);
        WriteOK = AT24C04_Save16Byte(EEPROM_ENABLE_BYTE, Data);

        if (WriteOK != 0 && WriteOK != 1)
        {
            SmartLock.MemoryFlag = RESET;
        }
        else
        {
        }
    }
    else
    {
    }

    /* ����ȫ���ж� */
    __set_PRIMASK(0);

    return;
}
#ifdef DEBUG
void AT24C04_CheckWrite0(void)
{
    u8 ReadTemp;
    printf("AT24C04_CheckWrite0\r\n");
    for (u32 i = 0; i <= 255; i++)
    {
        printf("|%4d W0 %d ", i, AT24C04_WriteByte(i, 0x00));
        printf("R0 %d %#0.2x|", AT24C04_ReadByte(i, &ReadTemp), ReadTemp);
        printf("\r\n");
    }

    return;
}
void AT24C04_CheckWrite1(void)
{
    u8 ReadTemp;
    printf("AT24C04_CheckWrite1\r\n");
    for (u32 i = 0; i <= 255; i++)
    {
        printf("|%4d W1 %d ", i, AT24C04_WriteByte(i, 0xFF));
        printf("R1 %d %#0.2x|", AT24C04_ReadByte(i, &ReadTemp), ReadTemp);
        printf("\r\n");
    }

    return;
}
#endif

/**
  * @}
  */
