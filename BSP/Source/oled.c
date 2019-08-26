/**
******************************************************************************
  * @file       oled.c
  * @brief      OLED Driver
  * @version    1.1
  * @date       Aug-09-2019 Fri
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "oled.h"

/** @addtogroup SPI peri
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define OLED_DC_H() (GPIO_SetBits(GPIOB, GPIO_Pin_12))
#define OLED_DC_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_12))
#define OLED_RES_H() (GPIO_SetBits(GPIOB, GPIO_Pin_6))
#define OLED_RES_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_6))
#define OLED_CS_H() (GPIO_SetBits(GPIOB, GPIO_Pin_7))
#define OLED_CS_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_7))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void OLED_Init(void)
{
    /* SPI2 Init */
    // ȱʡ

    /* OLED_CS PB7 OLED_DC PB12 OLED_RES PB6 */

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PB6 and PB7 , PB12 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIO׼�� */
    OLED_CS_H();
    OLED_RES_H();

    return;
}

u8 SPI_Write_Read(u8 SendData)
{
    u8 RecvData;

    /* �ȴ����ͼĴ���Ϊ�� */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
    }
    /* �������� */
    SPI_I2S_SendData(SPI2, SendData);

    /* �ȴ����ܼĴ����ǿ� */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
    }
    /* �������� */
    RecvData = SPI_I2S_ReceiveData(SPI2);

    return RecvData;
}

void OLED_SendCmd(u8 Cmd)
{
    /* ѡ��OLED Ϊ���豸 */
    OLED_CS_L();

    /* ָʾOLED��ȡ���� */
    OLED_DC_L();

    SPI_Write_Read(Cmd);

    /* �������߿���Ȩ */
    OLED_CS_H();

    return;
}

void OLED_SendData(u8 Data)
{
    /* ѡ��OLED Ϊ���豸 */
    OLED_CS_L();

    /* ָʾOLED��ȡ���� */
    OLED_DC_H();

    SPI_Write_Read(Data);

    /* �������߿���Ȩ */
    OLED_CS_H();

    return;
}

void OLED_Config(void)
{
    OLED_RES_L();
    delay_ms(100);
    OLED_RES_H();
    delay_ms(100); //��λ

    OLED_SendCmd(0xAE); //�ر���ʾ
    OLED_SendCmd(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_SendCmd(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
    OLED_SendCmd(0xA8); //��������·��
    OLED_SendCmd(0X3F); //Ĭ��0X3F(1/64)
    OLED_SendCmd(0xD3); //������ʾƫ��
    OLED_SendCmd(0X00); //Ĭ��Ϊ0

    OLED_SendCmd(0x40); //������ʾ��ʼ�� [5:0],����.

    OLED_SendCmd(0x8D); //��ɱ�����
    OLED_SendCmd(0x14); //bit2������/�ر�
    OLED_SendCmd(0x20); //�����ڴ��ַģʽ
    OLED_SendCmd(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_SendCmd(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
    OLED_SendCmd(0xC8); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
    OLED_SendCmd(0xDA); //����COMӲ����������
    OLED_SendCmd(0x12); //[5:4]����

    OLED_SendCmd(0x81); //�Աȶ�����
    OLED_SendCmd(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
    OLED_SendCmd(0xD9); //����Ԥ�������
    OLED_SendCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_SendCmd(0xDB); //����VCOMH ��ѹ����
    OLED_SendCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_SendCmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_SendCmd(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    OLED_SendCmd(0xAF); //������ʾ

    return;
}

void OLED_SetLocation(u8 x, u8 y) //������ʾλ�� x ʱ�У�0-127�У�  y���У�0-7��һ��8����
{
    OLED_SendCmd(0XB0 + y); //������Y  0-7 ��

    OLED_SendCmd(0X00 + (x & 0X0F));        //��4λ
    OLED_SendCmd(0X10 + ((x & 0XF0) >> 4)); //��4λ
}

//OLED����
void OLED_Draw_Point(u8 x, u8 y) //x : 0-127  y : 0-63
{
    OLED_SendCmd(0x00 + x);                  //�����������Y������
    OLED_SendData(0x10 + ((x & 0x0F) >> 4)); //���������4λ
    OLED_SendData(0x00 + (x & 0x0F));        //���������4λ

    OLED_SendCmd(0xB0 + y);                  //�����������Y������
    OLED_SendData(0x10 + ((y & 0x0F) >> 4)); //���������4λ
    OLED_SendData(0x00 + (y & 0x0F));        //���������4λ

    return;
}

//����
void OLED_Clear(void)
{
    u8 i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetLocation(0, j);

        for (i = 0; i < 128; i++)
        {
            OLED_SendData(0x00);
        }
    }

    return;
}

void OLED_ClearPart(u8 StartRowNum, u8 Height, u8 StartColumn, u8 Width)
{
    u8 Blank;

    if (ReversalFlag == 0)
    {
        Blank = 0x00;
    }
    else
    {
        Blank = 0xFF;
    }

    for (u8 i = StartRowNum; i < StartRowNum + Height; i++)
    {
        OLED_SetLocation(StartColumn, i);

        for (u8 j = StartColumn; j < StartColumn + Width; j++)
        {
            OLED_SendData(Blank);
        }
    }

    return;
}

// ÿ��ռ8�����أ���Ϊ��Ļ�� Row 0~7
u8 OLED_Show_XxN8_Character(u8 Row, u8 Column, u8 RowHeight, u8 Width, const u8 *FontArray)
{
    u8 i;
    u8 CurrentRow = 0;

    while (CurrentRow < RowHeight)
    {
        OLED_SetLocation(Column, Row + CurrentRow);
        for (i = 0; i < Width; i++)
        {
            if (ReversalFlag == 0)
            {
                OLED_SendData(((u8(*)[Width])FontArray)[CurrentRow][i]);
            }
            else
            {
                OLED_SendData(~((u8(*)[Width])FontArray)[CurrentRow][i]);
            }
        }
        CurrentRow++;
    }

    return 0;
}

void OLED_ShowString(u8 RowNumber, u8 RowHeight, u8 Column, const u8 *StringFont, u8 StringLength)
{
    for (u32 j = 0; j < StringLength; j += 1)
    {
        OLED_Show_XxN8_Character(RowNumber, (j * 16 + Column), RowHeight, 16, StringFont + (32 * j));
    }
    return;
}

void OLED_ShowPicture(u8 x, u8 y, u8 px, u8 py, const u8 *Picture)
{
    u8 i;
    u8 CurrentRow = 0;

    while (CurrentRow < (py / 8))
    {
        OLED_SetLocation(x, (y / 8) + CurrentRow);
        for (i = 0; i < px; i++)
        {
            if (ReversalFlag == 0)
            {
                //				OLED_SendData(((u8 (*)[px])Picture)[CurrentRow][i]);// ǿת�������Ͳ���������
                OLED_SendData(*(Picture + CurrentRow * px + i));
            }
            else
            {
                //				OLED_SendData(~((u8 (*)[px])Picture)[CurrentRow][i]);// ǿת�������Ͳ���������
                OLED_SendData(*(Picture + CurrentRow * px + i));
            }
        }
        CurrentRow++;
    }

    return;
}

/**
  * @}
  */
