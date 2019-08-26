/**********************************************************************************************
Copyright 2008 - 2016 ��������ӯ��������޹�˾. All rights reserved.
����   :     
����   :       л����
�汾   :       V1.0
�޸�   :   
������ڣ�     2017��12��12��
��ӯ�������http://www.edu118.com/
��ӯ����У��http://www.edu118.cn/
��ӯ�＼����̳��http://bbs.edu118.com/
Notice    :������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;����Ȩ���У�����ؾ�
***********************************************************************************************/

/**********************************************************************************************
* INCLUDES
*/
#include "rfid.h"
#include "oled.h"
#include "time.h"
#include "string.h"
/**********************************************************************************************
* CONSTANTS
*/

/**********************************************************************************************
* TYPEDEFS
*/

/**********************************************************************************************
* LOCAL VARIABLES
*/

/**********************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/

/**********************************************************************************************
* LOCAL FUNCTIONS  
*/

u8 spi_read_write(u8 SendData)
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

/**********************************************************************************************
* PUBLIC FUNCTIONS
*/
/*******************************************************************
* ������	:		RFID_Init()
* ��������:		��ʼ��RFID��ʹ�ܽţ�����Ļ����SPI��������Ҫ����OLED_INIT��
* ����:			л���� 
* ����˵��:		��
* ����ֵ˵��:	��
* �޸ļ�¼:		��ֲ��׼��
* �޸���	:		LukeLiu
* ������

*******************************************************************/
/*************************************
*Ӳ������˵����
*1--SDA (CS)<----->PA6
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14
*5--IRQ<------>�ж�
*6--GND <----->GND
*7--RST <----->PA7
*8--3.3V<----->3.3V
**************************************/
void RFID_Init(void)
{
    // ����RC522�ڽϸ�����ͨ�Ų��ȶ�������SPIʱ��Ӧ����һЩ

    /* GPIO ConfigureRFID_CS PA6  RFID_RST PA7 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOA Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PA6 and PA7 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RFID_CS_H();
    RFID_RST_H();

    Reset_RC522(); //��������
    M500PcdConfigISOType('A');
}

/*******************************************************************
* ��������     Reset_RC522()
* �������� ��   ��λRC522
* ���ߣ�         л���� 
* ����˵����     ��
* ����ֵ˵����   ��
* �޸ļ�¼��
* ������
*******************************************************************/
void Reset_RC522(void)
{
    PcdReset();      //��    �ܣ���λRC522
    PcdAntennaOff(); //�ر�����
    PcdAntennaOn();  //��������
}

/*******************************************************************
* ��������     PcdRequest()
* �������� ��   	Ѱ��
* ���ߣ�         л���� 
*����˵��: req_code[IN]:Ѱ����ʽ
                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
                0x26 = Ѱδ��������״̬�Ŀ�
           pTagType[OUT]����Ƭ���ʹ���
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
* ����ֵ˵�����ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdRequest(u8 req_code, u8 *pTagType)
{
    char status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);
    WriteRawRC(BitFramingReg, 0x07);
    SetBitMask(TxControlReg, 0x03);

    ucComMF522Buf[0] = req_code;

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

    if ((status == MI_OK) && (unLen == 0x10))
    {
        *pTagType = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* ��������     PcdAnticoll()
* �������� ��   ����ײ
* ���ߣ�         л���� 
* ����˵����     pSnr[OUT]:��Ƭ���кţ�4�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdAnticoll(u8 *pSnr)
{
    char status;
    u8 i, snr_check = 0;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);
    WriteRawRC(BitFramingReg, 0x00);
    ClearBitMask(CollReg, 0x80);

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

    if (status == MI_OK)
    {
        for (i = 0; i < 4; i++)
        {
            *(pSnr + i) = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i])
        {
            status = MI_ERR;
        }
    }

    SetBitMask(CollReg, 0x80);
    return status;
}

/*******************************************************************
* ��������     PcdSelect()
* �������� ��   ѡ����Ƭ
* ���ߣ�         л���� 
* ����˵����     pSnr[IN]:��Ƭ���кţ�4�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdSelect(u8 *pSnr)
{
    char status;
    u8 i;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i = 0; i < 4; i++)
    {
        ucComMF522Buf[i + 2] = *(pSnr + i);
        ucComMF522Buf[6] ^= *(pSnr + i);
    }
    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);

    ClearBitMask(Status2Reg, 0x08);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);

    if ((status == MI_OK) && (unLen == 0x18))
    {
        status = MI_OK;
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* ��������     PcdAuthState()
* �������� ��   ��֤��Ƭ����
* ���ߣ�         л���� 
*����˵��: auth_mode[IN]: ������֤ģʽ
                 0x60 = ��֤A��Կ
                 0x61 = ��֤B��Կ 
								addr[IN]�����ַ
								pKey[IN]������
								pSnr[IN]����Ƭ���кţ�4�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdAuthState(u8 auth_mode, u8 addr, u8 *pKey, u8 *pSnr)
{
    char status;
    u8 unLen;
    //     u8   i;
    u8 ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    //    for (i=0; i<6; i++)
    //    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    //    for (i=0; i<6; i++)
    //    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    memcpy(&ucComMF522Buf[2], pKey, 6);
    memcpy(&ucComMF522Buf[8], pSnr, 4);

    status = PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* ��������     PcdRead()
* �������� ��   ��ȡM1��һ������
* ���ߣ�         л���� 
*����˵��: addr[IN]�����ַ
           p [OUT]�����������ݣ�16�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdRead(u8 addr, u8 *p)
{
    char status;
    u8 unLen;
    u8 i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    //   {   memcpy(p , ucComMF522Buf, 16);   }
    {
        for (i = 0; i < 16; i++)
        {
            *(p + i) = ucComMF522Buf[i];
        }
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* ��������     PcdWrite()
* �������� ��   д���ݵ�M1��һ��
* ���ߣ�         л���� 
*����˵��: addr[IN]�����ַ
           p [OUT]�����������ݣ�16�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdWrite(u8 addr, u8 *p)
{
    char status;
    u8 unLen;
    u8 i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, p , 16);
        for (i = 0; i < 16; i++)
        {
            ucComMF522Buf[i] = *(p + i);
        }
        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }

    return status;
}

/*******************************************************************
* ��������     PcdHalt()
* �������� ��   ���Ƭ��������״̬
* ���ߣ�         л���� 
*����˵��: 
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdHalt(void)
{
    //    u8   status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    //    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    return MI_OK;
}

/*******************************************************************
* ��������     CalulateCRC()
* �������� ��  ��MF522����CRC16����
* ���ߣ�         л���� 
*����˵��: 
* ����ֵ˵���� 
* �޸ļ�¼��
* ������
*******************************************************************/
void CalulateCRC(u8 *pIn, u8 len, u8 *pOut)
{
    u8 i, n;
    ClearBitMask(DivIrqReg, 0x04);
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80);
    for (i = 0; i < len; i++)
    {
        WriteRawRC(FIFODataReg, *(pIn + i));
    }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x04));
    pOut[0] = ReadRawRC(CRCResultRegL);
    pOut[1] = ReadRawRC(CRCResultRegM);
}

/*******************************************************************
* ��������     PcdReset()
* �������� ��  ��λRC522
* ���ߣ�         л���� 
* ����˵��: 
* ����ֵ˵���� �ɹ�����MI_OK
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdReset(void)
{
    //PORTD|=(1<<RC522RST);
    RFID_RSTH;
    delay_us(2);

    //PORTD&=~(1<<RC522RST);
    RFID_RSTL;
    delay_us(2);

    //PORTD|=(1<<RC522RST);
    RFID_RSTH;
    delay_us(2);

    WriteRawRC(CommandReg, PCD_RESETPHASE);
    WriteRawRC(CommandReg, PCD_RESETPHASE);
    delay_us(2);

    WriteRawRC(ModeReg, 0x3D); //��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL, 30);
    WriteRawRC(TReloadRegH, 0);
    WriteRawRC(TModeReg, 0x8D);
    WriteRawRC(TPrescalerReg, 0x3E);

    WriteRawRC(TxAutoReg, 0x40); //����Ҫ

    return MI_OK;
}

/*******************************************************************
* ��������     M500PcdConfigISOType()
* �������� ��  ����RC632�Ĺ�����ʽ 
* ���ߣ�         л���� 
* ����˵��: 
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
char M500PcdConfigISOType(u8 type)
{
    if (type == 'A') //ISO14443_A
    {
        ClearBitMask(Status2Reg, 0x08);
        WriteRawRC(ModeReg, 0x3D);   //3F
        WriteRawRC(RxSelReg, 0x86);  //84
        WriteRawRC(RFCfgReg, 0x7F);  //4F
        WriteRawRC(TReloadRegL, 30); //tmoLength);// TReloadVal = 'h6a =tmoLength(dec)
        WriteRawRC(TReloadRegH, 0);
        WriteRawRC(TModeReg, 0x8D);
        WriteRawRC(TPrescalerReg, 0x3E);
        delay_us(2);
        PcdAntennaOn();
    }
    else
    {
        return 1;
    }

    return MI_OK;
}

/*******************************************************************
* ��������     ReadRawRC()
* �������� ��  ��RC632�Ĵ���
* ���ߣ�         л���� 
* ����˵��:    Address[IN]:�Ĵ�����ַ
* ����ֵ˵���� ������ֵ
* �޸ļ�¼��
* ������
*******************************************************************/
u8 ReadRawRC(u8 Address)
{
    u8 ucAddr;
    u8 ucResult = 0;
    RFID_CSL;
    ucAddr = ((Address << 1) & 0x7E) | 0x80;

    spi_read_write(ucAddr);
    ucResult = spi_read_write(0);
    RFID_CSH;
    return ucResult;
}

/*******************************************************************
* ��������     WriteRawRC()
* �������� ��  дRC632�Ĵ���
* ���ߣ�         л���� 
* ����˵��:    Address[IN]:�Ĵ�����ַ
							 value[IN]:д���ֵ
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
void WriteRawRC(u8 Address, u8 value)
{
    u8 ucAddr;
    //	u8 tmp;

    RFID_CSL;
    ucAddr = ((Address << 1) & 0x7E);

    spi_read_write(ucAddr);
    spi_read_write(value);
    RFID_CSH;

    // 		tmp=ReadRawRC(Address);

    // 		if(value!=tmp)
    // 		printf("wrong\n");
}

/*******************************************************************
* ��������     SetBitMask()
* �������� ��  ��RC522�Ĵ���λ
* ���ߣ�         л���� 
* ����˵��:    reg[IN]:�Ĵ�����ַ
							 mask[IN]:��λֵ
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
void SetBitMask(u8 reg, u8 mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp | mask); // set bit mask
}

/*******************************************************************
* ��������     ClearBitMask()
* �������� ��  ��RC522�Ĵ���λ
* ���ߣ�         л���� 
* ����˵��:    reg[IN]:�Ĵ�����ַ
							 mask[IN]:��λֵ
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
void ClearBitMask(u8 reg, u8 mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask); // clear bit mask
}

/*******************************************************************
* ��������     PcdComMF522()
* �������� ��  ͨ��RC522��ISO14443��ͨѶ
* ���ߣ�         л���� 
* ����˵��:   Command[IN]:RC522������
							pIn [IN]:ͨ��RC522���͵���Ƭ������
							InLenByte[IN]:�������ݵ��ֽڳ���
							pOut [OUT]:���յ��Ŀ�Ƭ��������
							*pOutLenBit[OUT]:�������ݵ�λ����
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
char PcdComMF522(u8 Command,
                 u8 *pIn,
                 u8 InLenByte,
                 u8 *pOut,
                 u8 *pOutLenBit)
{
    char status = MI_ERR;
    u8 irqEn = 0x00;
    u8 waitFor = 0x00;
    u8 lastBits;
    u8 n;
    u16 i;
    switch (Command)
    {
    case PCD_AUTHENT:
        irqEn = 0x12;
        waitFor = 0x10;
        break;
    case PCD_TRANSCEIVE:
        irqEn = 0x77;
        waitFor = 0x30;
        break;
    default:
        break;
    }

    WriteRawRC(ComIEnReg, irqEn | 0x80);
    ClearBitMask(ComIrqReg, 0x80); //�������ж�λ
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80); //��FIFO����

    for (i = 0; i < InLenByte; i++)
    {
        WriteRawRC(FIFODataReg, pIn[i]);
    }
    WriteRawRC(CommandReg, Command);
    //   	 n = ReadRawRC(CommandReg);

    if (Command == PCD_TRANSCEIVE)
    {
        SetBitMask(BitFramingReg, 0x80);
    } //��ʼ����

    i = 600; //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
             // 	  i = 100000;
    do
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));
    ClearBitMask(BitFramingReg, 0x80);

    if (i != 0)
    {
        if (!(ReadRawRC(ErrorReg) & 0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
                status = MI_NOTAGERR;
            }
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);
                lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                }
                else
                {
                    *pOutLenBit = n * 8;
                }
                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAXRLEN)
                {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++)
                {
                    pOut[i] = ReadRawRC(FIFODataReg);
                }
            }
        }
        else
        {
            status = MI_ERR;
        }
    }

    SetBitMask(ControlReg, 0x80); // stop timer now
    WriteRawRC(CommandReg, PCD_IDLE);
    return status;
}

/*******************************************************************
* ��������     PcdAntennaOn()
* �������� ��  �������� 
* ���ߣ�         л���� 
* ����˵��:    
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
void PcdAntennaOn(void)
{
    u8 i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/*******************************************************************
* ��������     PcdAntennaOff()
* �������� ��  �ر����� 
* ���ߣ�         л���� 
* ����˵��:    
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

char Read_IC_Card_ID(u8 *id)
{
    unsigned char CT[2];                      //������
    if (PcdRequest(PICC_REQALL, CT) == MI_OK) //Ѱ���ɹ�
    {

        if (PcdAnticoll(id) == MI_OK) /*����ײ�ɹ�*/
        {

            if (PcdSelect(id) == MI_OK) //ѡ���ɹ�
            {
                return MI_OK;
            }
        }
    }
    return MI_ERR;
}
/***********************************************************************************************
***********************************************************************************************/
