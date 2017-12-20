#ifndef __CHARGE_STATION_DEAL_H__
#define __CHARGE_STATION_DEAL_H__
#include "AllHand.h"

extern uint8_t         Station_State;         //���׮״̬
extern uint8_t         Charge_Flag;                //���׼����ʶ ǩȨ�ɹ���־bit7 ǹ���ӱ�ʶbit5

extern uint32_t    ChargeBeginTime;                //��翪ʼʱ��

//�ȴ�ɨ���� ��ʾ��ά��
void Station_Idle_deal(void);

//�ȴ����ǹ����
void Station_Connect_deal(void);

//����� ��ʾ��������Ϣ
void Station_Charge_deal(void);

//������ ��ʾ�ۿ��綩��������Ϣ
void Station_ChargeEND_deal(void);

void Station_ChargeError_deal(void);

//���ǹǩȨ�ȴ�����
void Station_Authtic_deal(void);



#endif