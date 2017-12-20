#ifndef _ALLHAND_H_
#define _ALLHAND_H_

#include <LPC177x_8x.H>
#include "stdint.h"
#include "ucos_ii.h"

//#include "PTL104.h"
//#include "lcd.h"
//#include "main.h"

#include "PCF8563.h"
#include "rtc.h"
#include "t6963.h"
#include "datastructure.h"
#include "global.h"
#include "lcd.h"
#include "ethernet.h"
#include "socket.h"
#include "w5100.h"
#include "lpc_adc.h"
#include "relay.h"
#include "charge_station_deal.h"
#include "rs232.h"
#include "dl645a.h"
#include "QR_Encode.h"


#define LOCAL_DEBUG 1


//���׮����״̬����
#define STATION_SETUP       (0)
#define STATION_Idle        (1)
#define STATION_Charge      (2)
#define STATION_ChargeEND   (3)
#define STATION_Connect     (4)
#define STATION_Authtic     (5)

//���׮���׼��λ
#define         CHANGE_READY_OK             0xA0

#define         CHANGE_AUTHTIC_BIT          0x80        //��Ȩ�ɹ�λ 1 ��Ȩ�ɹ�
#define         CHANGE_CAR_READY_BIT        0x40        //�������Գ��  1׼��OK
#define         CHANGE_GUN_CONN_BIT         0x20        //���ǹ���ӳ����ɹ�λ  1���ӳɹ�
#define         CHANGE_ING_BIT              0x10
#define         CHANGE_END_PAY              0x08
#define         CHANGE_END_Full_BIT         0x04        //��س�������
#define         CHANGE_END_Ctrl_BIT         0x02        //APP�������
#define         CHANGE_END_Pull_BIT         0x01        //�γ��ǹ�������

//��ʾ��ʶλ
#define         ChargeInfoTitle             0x80
//#define         ChargeEndInfoTitle          0x40 
//#define         ChargeInfoUpdata            0x20        //�����Ϣ����
#define         ChargeGunInsert             0x10
#define         DisplayChargeInfoFlag       0x08        //�����Ϣ��ʾ
#define         DisplayChargeEndInfoFlag    0x04        //������Ϣ��ʾ
#define         DisplayStartFlag            0x02        //��ʼ������Ϣ��ʾ

#define         DisplayErrButtonOff         0x01        //������ͣ��ť

#endif

