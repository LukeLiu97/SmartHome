/**
******************************************************************************
  * @file       finger.c
  * @brief      
  * @version    1.1
  * @date       Aug-13-2019 Tue
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "finger.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void Fingerprint_Init(void)
{
    // TODO GPIO PB9 MG200_PWR PC13 MG200_DETECT

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PB9 in PP mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC13 in IF mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // TODO UART2

    MG200_PWR_ON();
}

void Fingerprint_SendPack(u8 Command, u8 Parameter)
{
    /* ���Ͱ�7Byte */
    UART2_SendByte(FingerPack_Start);                                                                      // ��ʼ��
    UART2_SendByte(FingerPack_MasterAddr);                                                                 // MCU��ַ
    UART2_SendByte(FingerPack_SlaveAddr);                                                                  // ģ���ַ
    UART2_SendByte(Command);                                                                               // ����
    UART2_SendByte(Parameter);                                                                             // ����
    UART2_SendByte(FingerPack_UnUse);                                                                      // ����λ
    UART2_SendByte(FingerPack_MasterAddr + FingerPack_SlaveAddr + Command + Parameter + FingerPack_UnUse); // ��Уλ

    return;
}

u8 Fingerprint_RecvPack(u8 Command, u8 *Result, u8 *Parameter)
{
    /* ���ܰ�8Byte */
    u8 CheckSum = 0;

    while (FingerPack.Over != 1)
    {
    }
    FingerPack.Over = 0;

    for (u8 i = 1; i < 7; i++)
    {
        CheckSum += FingerPack.Data[i];
    }
    if (FingerPack.Data[7] != CheckSum)
    {
        return 1;
    }

    if (FingerPack.Data[0] != FingerPack_Start)
    {
        return 1;
    }
    else
    {
    }

    if (FingerPack.Data[1] != FingerPack_SlaveAddr)
    {
        return 1;
    }
    else
    {
    }

    if (FingerPack.Data[2] != FingerPack_MasterAddr)
    {
        return 1;
    }
    else
    {
    }

    if (FingerPack.Data[3] != Command)
    {
        return 1;
    }
    else
    {
    }

    *Result = FingerPack.Data[4];
    *Parameter = FingerPack.Data[5];

    return 0;
}

u8 Finger_CaptureAndExtract(u8 time)
{
    u8 ResultTemp = 0;
    u8 ParamTemp = 0;
    u8 i = 0x00; // ��¼����ȡָ�ƵĶ�Ӧ����

    while (time--)
    {
        while (MG200_DETECT_Status() != SET) // �ȴ���ָ����
        {
        }
        Fingerprint_SendPack(FingerPack_CaptureAndExtract, i);

        //		while(MG200_DETECT_Status() != RESET) // �ȴ���ָ�ƿ�
        //		{
        //		}

        Fingerprint_RecvPack(FingerPack_CaptureAndExtract, &ResultTemp, &ParamTemp);

        if (ResultTemp != i)
        {
            return 1;
        }
        else
        {
        }

        if (i >= 0x04)
        {
            i = 0x00;
        }
    }

    return 0;
}

u8 Finger_EnrollNewUser(u8 ID)
{
    u8 Result, Param;

    if (Finger_CaptureAndExtract(3) != 0)
    {
        return 1; // �ɼ�����ʧ��
    }
    else
    {
        Fingerprint_SendPack(FingerPack_Enroll, ID);
        if (Fingerprint_RecvPack(FingerPack_Enroll, &Result, &Param) != 0) // ID�ᱻ����0x00
        {
            return 1;
        }
        else
        {

#ifdef DEBUG
            printf("Enroll new user result = %d\r\n", Result);
#endif
            if (Result != 0x00)
            {
                return 1;
            }
            else
            {
            }
        }
    }

    return 0;
}

u8 Finger_Compare(u8 *ID)
{
    u8 Result;

    if (Finger_CaptureAndExtract(3) != 0)
    {
        return 1; // �ɼ�����ʧ��
    }
    else
    {
        Fingerprint_SendPack(FingerPack_Match1N, 0x00);
        if (Fingerprint_RecvPack(FingerPack_Match1N, &Result, ID) != 0)
        {
            return 1;
        }
        else
        {

#ifdef DEBUG
            printf("Match 1 vs N result = %d\r\n", Result);
#endif
            if (Result != 0x00)
            {
                return 1;
            }
            else
            {
            }
        }
    }

    return 0;
}

// Result 0x00 ɾ���ɹ� 0x90 ָ�ƿ�� other ɾ��ʧ��
u8 Finger_EraseAllUser(u8 *Result)
{
    u8 Param;

    Fingerprint_SendPack(FingerPack_EraseAll, 0x00);
    if (Fingerprint_RecvPack(FingerPack_EraseAll, Result, &Param) != 0)
    {
        return 1;
    }
    else
    {

#ifdef DEBUG
        printf("Erase All User Result = %d\r\n", *Result);
#endif
        if (*Result != 0x00 && *Result != 0x90)
        {
            return 1;
        }
        else
        {
        }
    }

    return 0;
}

/**
  * @}
  */
