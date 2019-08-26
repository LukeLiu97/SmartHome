/**
******************************************************************************
  * @file       rtc.c
  * @brief      Real time drive
  * @version    1.1
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/** @addtogroup Real Time Clock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const u32 CommonYearSec = 365 * 24 * 60 * 60;
const u32 LeapYearSec = 365 * 24 * 60 * 60;
const u8 Months[12] =
    {
        31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31};
const u32 DaySec = 24 * 60 * 60;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// NVIC RTC���ж�����

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval 
  */
u8 RTC_Config(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // ��������洢RTC��־

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    return 0;
}

#define RTC_FLAG 0x9797 //�Զ���

void RTC_Init(void)
{
    if (BKP_ReadBackupRegister(BKP_DR1) != RTC_FLAG)
    {
        /* Backup data register value is not correct or not yet programmed (when
		the first time the program is executed) */

        /* RTC �趨 */
        if (RTC_Config() != 0)
        {
            // ����ʧ��
        }
        else
        {
            // User adjust time task ȱʡ

            BKP_WriteBackupRegister(BKP_DR1, RTC_FLAG);
        }
    }
    else
    {
        /* Check if the Power On Reset flag is set */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
        }
        /* Check if the Pin Reset flag is set */
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
        }
        else
        {
        }

        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchro();

        /* Enable the RTC Second */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}

//void RTC_TimeCalulate(TimeStu *Time)
//{
////	TimeStu TimeTemp;
//	u32 RTC_Value;
//	RTC_Value = RTC_GetCounter();
//
//	Time->Hour = RTC_Value / 3600;
//	Time->Minute = (RTC_Value % 3600) / 60;
//	Time->Second = (RTC_Value % 3600) % 60;

//	return ;
//}

//u8 RTC_SetCountValue(TimeStu *Time)
//{
//	u32 ValueTemp = 0;
//
//	u32 i;
//
//	if(Time->Year < 1970 || Time->Year > 2099)
//	{
//		return 1;
//	}
//	else
//	{
//		for(i = 1970; i < Time->Year ; i++)
//		{
//			if(i / 400 == 0)
//			{
//				ValueTemp += LeapYearSec;
//			}
//			else if(i / 4 == 0 && i/ 100 != 0)
//			{
//				ValueTemp += LeapYearSec;
//			}
//			else
//			{
//				ValueTemp += CommonYearSec;
//			}
//		}
//
//		for(i = 1;i < Time->Month;i++)
//		{
//			ValueTemp += Months[i-1] * DaySec;
//
//			if(i == 2 && Time->Year / 4 == 0 && Time->Year / 100 != 0)
//			{
//				ValueTemp += DaySec;
//			}
//			else
//			{
//			}
//		}
//
//
//		ValueTemp += (Time->Date - 1) * DaySec;
//		ValueTemp += Time->Hour * 3600;
//		ValueTemp += Time->Minute * 60;
//		ValueTemp += Time->Second;
//	}
//
//	/* Enable PWR and BKP clocks */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//	/* Allow access to BKP Domain */
//	PWR_BackupAccessCmd(ENABLE);
//
//	RTC_SetCounter(ValueTemp);
//	RTC_WaitForLastTask();
//
//	return 0;
//}

// �����޸�������Դ��

const unsigned char DateMaxTab[13] = {
    0,
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31,
};

/****************************************************************************
* ��    �ƣ�u8 Is_Leap_Year(u16 year)
* ��    �ܣ��ж��Ƿ�������
* ��ڲ�����year
* ���ڲ�����1:�����ꣻ0:������
* ˵    ����
* ���÷������� 
****************************************************************************/
u8 Is_LeapYear(u16 year)
{
    if (year % 4 == 0) //�����ܱ�4����
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
                return 1; //�����00��β,��Ҫ�ܱ�400����
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}

/****************************************************************************
* ��    �ƣ�u8 Get_Week(u16 Year,u16 Month,u16 Date)
* ��    �ܣ��ò��գ�Zeller����ʽ�������ڼ�	

	w=y+[y/4]+[c/4]-2c+[26��m+1��/10]+d-1

	Ȼ��w��7ȡ�ࡣ
	��ʽ�еķ��ź������£�w�����ڣ�c�����ͣ�y���꣨��λ������m���£�m���ڵ���3��С
	�ڵ���14�����ڲ��չ�ʽ�У�ĳ���1��2��Ҫ������һ���13��14�������㣬����2003��1��1
	��Ҫ����2002���13��1�������㣩��d���գ�[ ]����ȡ������ֻҪ�������֡�

* ��ڲ�����Year:��; Month:��; Date:�ա�
* ���ڲ��������ڼ� 1~6��������һ~��������0����������
* ˵    ����
* ���÷�������
****************************************************************************/
u8 Get_Week(u16 Year, u16 Month, u16 Date)
{
    int W, Y, C, M, D;
    u8 day;

    C = Year / 100;
    Y = Year % 100;
    M = Month;
    if (M < 3)
    {
        M += 12;
        Y--;
    }
    D = Date;
    W = Y + Y / 4 + C / 4 - 2 * C + 26 * (M + 1) / 10 + D - 1;
    while (W < 0)
        W += 7;
    day = W % 7;
    return day;
}

u8 Get_DateMax(u16 Year, u16 Month)
{
    u8 DateMax;

    DateMax = DateMaxTab[Month];
    if (Month == 2)
    {
        if (Is_LeapYear(Year) == 1)
            DateMax++;
    }

    return DateMax;
}

/****************************************************************************
* ��    �ƣ�u8 Set_RTCTime(struct T_STRUCT *Time)
* ��    �ܣ�����ʱ��:��1970-01-01 0:0:0Ϊ��׼,�������ʱ��ת��Ϊ����,
* 					��д��RTC��������1970~2099��Ϊ�Ϸ���ݡ�
* ��ڲ�����T_STRUCT* Time
* ���ڲ�����0:�ɹ�;
						1:������룬����Чʱ��(1970~2099)
* ˵    ����
* ���÷�����Set_RTCTime(&Real_Time);
****************************************************************************/
u8 Set_RTCTime(TimeStu *Time)
{
    u32 Count = 0;
    u16 i;
    if (Time->Year < 1970 || Time->Year > 2099)
        return 1;
    for (i = 1970; i < Time->Year; i++)
    {
        if (Is_LeapYear(i))
            Count += 31622400; //���������
        else
            Count += 31536000; //ƽ�������
    }
    for (i = 1; i < Time->Month; i++)
    {
        Count += (u32)DateMaxTab[i] * 86400;
        if (Is_LeapYear(Time->Year) && i == 2)
            Count += 86400;
    }
    Count += (u32)(Time->Date - 1) * 86400;
    Count += (u32)(Time->Hour) * 3600;
    Count += (u32)(Time->Minute) * 60;
    Count += (u32)(Time->Second);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RTC_SetCounter(Count);
    RTC_WaitForLastTask();

    return 0;
}

/****************************************************************************
* ��    �ƣ�void Cal_Time(u32 Count,struct T_STRUCT *Time)
* ��    �ܣ�����32λCount����ֵ�������ں�ʱ��
* ��ڲ�����Count
* ���ڲ�����T_STRUCT* Time
* ˵    ����
* ���÷�����Cal_Time(Count,&Real_Time);
****************************************************************************/
void Cal_Time(u32 Count, TimeStu *Time)
{
    u32 SecTemp = 0, DateTemp = 0, Temp = 0;

    DateTemp = Count / 86400; //����
    if (DateTemp != 0)        //����1��
    {
        //ȷ����
        Temp = 1970;
        while (DateTemp >= 365)
        {
            if (Is_LeapYear(Temp)) //����?
            {
                if (DateTemp >= 366)
                    DateTemp -= 366;
                else
                {
                    break;
                }
            }
            else
                DateTemp -= 365;
            Temp++;
        }
        Time->Year = Temp;

        //ȷ����
        Temp = 1;
        while (DateTemp >= 28)
        {
            if (Is_LeapYear(Time->Year) && Temp == 2)
            {
                if (DateTemp >= 29)
                    DateTemp -= 29; //����Ķ�������
                else
                    break;
            }
            else
            {
                if (DateTemp >= DateMaxTab[Temp])
                    DateTemp -= DateMaxTab[Temp];
                else
                    break;
            }
            Temp++;
        }
        Time->Month = Temp;
        Time->Date = DateTemp + 1;
    }
    else
    {
        Time->Year = 1970;
        Time->Month = 1;
        Time->Date = 1;
    }
    SecTemp = Count % 86400;
    Time->Hour = SecTemp / 3600;
    Time->Minute = SecTemp % 3600 / 60;
    Time->Second = SecTemp % 60;
    Time->Day = Get_Week(Time->Year, Time->Month, Time->Date);
}

/****************************************************************************
* ��    �ƣ�void Read_RTCTime(struct T_STRUCT *Time)
* ��    �ܣ���RTC��������ȡʱ��		
* ��ڲ�����T_STRUCT* Time
* ���ڲ�������
* ˵    ����
* ���÷�����Read_RTCTime(&Real_Time);
****************************************************************************/
void Read_RTCTime(TimeStu *Time)
{
    u32 Count = 0;
    Count = RTC_GetCounter();
    Cal_Time(Count, Time);
}

/**
  * @}
  */
