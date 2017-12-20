#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#include "w5100.h"
#include "socket.h"
#include "AllHand.h"
#include "ptl104.h"


#define ETHERNET_INIT           0       //��ʼ̬
#define ETHERNET_CFG            1       //�������״̬
#define ETHERNET_CONNECTING     2       //������״̬
#define ETHERNET_CONNECT_OK     3       //���ӳɹ�
#define ETHERNET_DISCONNECT     4       //���ӶϿ�
#define ETHERNET_FAILURE        5       //������ӳ��Ժ� ����ʧ�� �����Զ����

#define IMR_VALUE     0xcf
#define RTR_VALUE     0x07d0
#define RCR_VALUE     0x08

#define MAX_SIZE        2048
#define RX_Size         1024
#define TX_Size         1024

//PTL104_State
#define PTL104_LOGIN            0x80
#define PTL104_ACTV             0x40
#define PTL104_SETTIME          0x20
#define PTL104_JF_STRGY         0x10
#define PTL104_LOGIN_SENT       0x08
#define PTL104_RECV             0x04

//
//uint8_t			time_flg; //bit7 secflgÿ���ʶ ˢ��LCD
#define     SECFLG                  0x80    //time_flg �ϵ���Ӧ��λ
#define     PTL104_TIME_TO_HEART    0x20    //time_flg �ϵ���Ӧ��λ
#define     PTL104_TIME_TO_EVENT    0x10    //time_flg �ϵ���Ӧ��λ
#define     PTL104_HEART_WAIT_ACK   0x02    //time_flg �ϵ���Ӧ��λ
#define     PTL104_EVENT_WAIT_ACK   0x01    //time_flg �ϵ���Ӧ��λ
#define     PTL104_EVENT_CHANGE_UP   0x04    //time_flg �ϵ���Ӧ��λ

#define     SECONDS_PER_HEART_BEAT      30      //����ʱ��
#define     SECONDS_PER_EVENT_UPDATE    120     //�¼��ϱ�֡ʱ��

#define     TIMEOUT_HEART_W_A           20
#define     TIMEOUT_EVENT_W_A           20
#define     TIMEOUT_LOGIN_W_A           10

extern uint8_t         DisplayFlag;             //LCD��Ļ��ʾ��ʶλ
extern uint8_t         Charge_Flag;             //���׼����ʶ ǩȨ�ɹ���־bit7 ǹ���ӱ�ʶbit5
extern uint8_t         Ethernet_State;          //����״̬


extern uint8_t      time_flg;                       //ʱ����ر�ʶ  bit7 secflgÿ���ʶ ˢ��LCD
extern uint32_t     Time_Seconds;                   //ÿ���1 ϵͳ����ʱ��
extern uint32_t    DisplayWaitTimeSeconds;
extern uint32_t     Wait_Ack_Seconds;
extern uint32_t     Wait_Gun_Insert_Seconds;        //�ȴ����ǹ���� ��λs
extern uint32_t     Wait_Heart_Ack_Seconds;
extern uint32_t     Wait_Event_Ack_Seconds;

extern uint8_t RX_Buffer[RX_Size];
extern uint8_t TX_Buffer[RX_Size];
extern uint8_t Ethernet_config(void);
extern void Ethernet_Init(void);
extern void Keep_Connect(void);
extern void Check_IntR(void);

extern void Ethernet_Work(uint8_t Value);
extern void Ethrenet_Connecting(uint8_t Value);
#endif
