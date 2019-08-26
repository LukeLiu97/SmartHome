/**
******************************************************************************
  * @file       i2c.c
  * @brief      
  * @version    1.1
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iic.h"

/** @addtogroup I2C function
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  ���IIC��ʼ������
  * @param  NULL
  * @return NULL
  */
void IIC_Software_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ���ų�ͻ ��Ҫ��ӳ��

    /* Configure PB4 and PB5 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ����״̬��ʼ�� */
    SCK_H();
    SDA_H();

    return;
}

/**
  * @brief  ����SDA����Ϊ���븡��״̬
  * @param  NULL
  * @return NULL
  */
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    return;
}

/**
  * @brief  ����SDA����Ϊ����״̬
  * @param  NULL
  * @return NULL
  */
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    return;
}

/**
  * @brief  ����IIC��ʼ�ź�
  * @param  NULL
  * @return NULL
  */
void IIC_Start(void)
{
    SDA_OUT();

    SCK_H();
    SDA_H();

    delay_us(NOP_TIME);
    SDA_L();
    delay_us(NOP_TIME);

    SCK_L(); //Ϊ֮��ʱ��׼��

    return;
}

/**
  * @brief  ����IIC�����ź�
  * @param  NULL
  * @return NULL
  */
void IIC_Stop(void)
{
    SDA_OUT();

    SCK_L(); //Ԥ������ʱ��

    SDA_L(); //׼������

    delay_us(NOP_TIME);
    SCK_H(); //׼���������
    SDA_H();
    delay_us(NOP_TIME);

    return;
}

/**
  * @brief  IIC���ܷ�Ӧ���ź�
  * @param  NULL
  * @return NULL
  */
void IIC_SendACK(void)
{
    SDA_OUT(); //���ܷ�Ӧ��׼��

    SCK_L(); //׼��ʱ��

    SDA_L(); //Ӧ��

    //SCK_H();
    delay_us(NOP_TIME);
    SCK_H();
    delay_us(NOP_TIME);

    SCK_L();

    return;
}

/**
  * @brief  IIC���ܷ�����δӦ���ź�
  * @param  NULL
  * @return NULL
  */
void IIC_SendNoACK(void)
{
    SDA_OUT();

    SCK_L();

    SDA_H();
    //SCK_H();
    delay_us(NOP_TIME);
    SCK_H();
    delay_us(NOP_TIME);
    SCK_L();

    return;
}

/**
  * @brief  IIC���ͷ�ȷ��ACK״̬
  * @param  NULL
  * @return Ӧ��״̬
  *     value 0 Ӧ��ʱ
  *     value 1 Ӧ��
  */
u8 IIC_CheckACK(void)
{
    u8 Count = 0;

    SDA_IN();

    SCK_L();
    delay_us(NOP_TIME);
    SCK_H();
    delay_us(NOP_TIME);

    while (SDA_Status() == 1)
    {
        Count++;
        if (Count > 250)
        {
            IIC_Stop();
            return 0;
        }
    }

    SCK_L();
    return 1;
}

/**
  * @brief  
  * @param  NULL
  * @return NULL
  */
void IIC_SendByte(u8 data)
{
    SDA_OUT();

    for (u32 i = 0; i < 8; i++)
    {
        SCK_L();

        if ((data & (0x80)) == 0)
        {
            SDA_L();
        }
        else
        {
            SDA_H();
        }
        data <<= 1;

        delay_us(NOP_TIME);

        SCK_H();
        delay_us(NOP_TIME);

        SCK_L();
        delay_us(NOP_TIME);
    }

    return;
}

u8 IIC_RecvByte(u8 End)
{
    u8 data;

    SDA_IN();

    for (u8 i = 0; i < 8; i++)
    {
        SCK_L(); // ÿһλ��Ҫ��Ӧ��ʱ��
        delay_us(NOP_TIME);

        SCK_H();
        data <<= 1;

        if (SDA_Status() != 0)
        {
            data += 1;
        }

        delay_us(NOP_TIME);

        SCK_L();
    }

    if (!End)
    {
        IIC_SendACK();
    }
    else
    {
        IIC_SendNoACK();
    }

    return data;
}

/**
  * @}
  */
