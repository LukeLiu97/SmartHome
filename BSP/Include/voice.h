/**
******************************************************************************
  * @file       voice.h
  * @brief      ����ģ�����ͷ�ļ�
  * @version    1.0
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VOICE_H
#define __VOICE_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "global.h"

#include "time.h"

    /* Global varaiables ---------------------------------------------------------*/
    extern u8 MuteMode;
    extern SmartLockStu SmartLock;

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    typedef enum
    {
        VoiceCmd_CHANGE_ADMIN_PASSWORD = 1,     //�޸Ĺ���Ա����
        VoiceCmd_SET_DOOR_PASSWORD = 2,         //���ÿ�������
        VoiceCmd_REGISTER_FINGER = 3,           //�Ǽ�ָ��
        VoiceCmd_REGISTER_CARD = 4,             //�Ǽǿ�Ƭ
        VoiceCmd_CHANGE_TIME = 5,               //�޸�ʱ��
        VoiceCmd_DELETE_ALLFINGER = 6,          //ɾ������ָ��
        VoiceCmd_DELETE_ASSIGN_FINGER = 7,      //ɾ��ָ��ָ��
        VoiceCmd_RECOVER_ALL = 8,               //�ָ���������
        VoiceCmd_CHANGE_VOLUME = 9,             //��������
        VoiceCmd_DELETE_ALL_CARD = 10,          //ɾ�������ſ�
        VoiceCmd_DELETE_ASSIGN_CARD = 11,       //ɾ��ָ���ſ�
        VoiceCmd_INPUT_ADMIN_PASSWORD = 12,     //���������Ա����
        VoiceCmd_INPUT_NEW_PASSWORD = 13,       //������������
        VoiceCmd_INPUT_NEW_PASSWORD_AGAIN = 14, //���ٴ�����������
        VoiceCmd_PASSWORD_ERROR = 15,           //��֤ʧ��
        VoiceCmd_PASSWORD_INCONFORMITY = 16,    //���벻һ��
        VoiceCmd_SETTING_SUCCESS = 17,          //�����ɹ�
        VoiceCmd_PASSWORD_REPETITION = 18,      //�����ظ�
        VoiceCmd_PUTCARD = 19,                  //����ÿ�Ƭ
        VoiceCmd_CARD_REGISTERED = 20,          //���ظ�
        VoiceCmd_DELETE_ALL_FIGNER = 21,        //�Ƿ�ɾ��
        VoiceCmd_DELETE_ASSIGNFIGNER = 22,      //�����ָ
        VoiceCmd_FINGER_REPETITION = 23,        //ָ���ظ�
        VoiceCmd_DOOROPEN_SUCCESS = 24,         //��ӭ�ؼ�
        VoiceCmd_DOOROPEN_FAIL = 25,            //����ʧ��
        VoiceCmd_CONNECT_SUCCESS = 26,          //�������ӳɹ�
        VoiceCmd_TIPS = 27,                     //��#��ȷ�ϣ���*���˳�
        VoiceCmd_REPETE = 28,                   //����������
        VoiceCmd_Di = 29,                       //�
        VoiceCmd_MenLing = 30,                  //���� ����
        VoiceCmd_Warm = 31                      //������
    } VoiceCmd_TypeDef;

    /* Exported defines ----------------------------------------------------------*/
    //#define CHANGE_ADMIN_PASSWORD		1 //�޸Ĺ���Ա����
    //#define SET_DOOR_PASSWORD			2 //���ÿ�������
    //#define REGISTER_FINGER				3 //�Ǽ�ָ��
    //#define REGISTER_CARD				4 //�Ǽǿ�Ƭ
    //#define CHANGE_TIME					5 //�޸�ʱ��
    //#define DELETE_ALLFINGER			6 //ɾ������ָ��
    //#define DELETE_ASSIGN_FINGER		7 //ɾ��ָ��ָ��
    //#define RECOVER_ALL					8 //�ָ���������
    //#define CHANGE_VOLUME				9 //��������
    //#define DELETE_ALL_CARD				10 //ɾ�������ſ�
    //#define DELETE_ASSIGN_CARD			11//ɾ��ָ���ſ�
    //#define INPUT_ADMIN_PASSWORD		12//���������Ա����
    //#define INPUT_NEW_PASSWORD			13//������������
    //#define INPUT_NEW_PASSWORD_AGAIN	14//���ٴ�����������
    //#define PASSWORD_ERROR				15//��֤ʧ��
    //#define PASSWORD_INCONFORMITY		16//���벻һ��
    //#define SETTING_SUCCESS				17//�����ɹ�
    //#define PASSWORD_REPETITION			18//�����ظ�
    //#define PUTCARD						19//����ÿ�Ƭ
    //#define CARD_REGISTERED				20//���ظ�
    //#define DELETE_ALL_FIGNER			21//�Ƿ�ɾ��
    //#define DELETE_ASSIGNFIGNER			22//�����ָ
    //#define FINGER_REPETITION			23//ָ���ظ�
    //#define DOOROPEN_SUCCESS			24//��ӭ�ؼ�
    //#define DOOROPEN_FAIL				25//����ʧ��
    //#define CONNECT_SUCCESS				26//�������ӳɹ�
    //#define TIPS						27//��#��ȷ�ϣ���*���˳�
    //#define REPETE						28//����������
    //#define Di							29//�
    //#define MenLing						30//���� ����
    //#define Warm						31//������

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    void VoiceModule_Init(void);
    void Voice_Play(VoiceCmd_TypeDef VoiceCmd);

#ifdef __cplusplus
}
#endif

#endif
