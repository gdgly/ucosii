/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           charge_station_deal.c
** Last modified Date:  2017-11-16
** Last Version:        V0.01
** Descriptions:        it_isr  
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-11-16
** Version:             V0.01
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:				TP-EVWA-220-1C
** Descriptions:       
**
** Rechecked by:  		
*********************************************************************************************************/
#include "charge_station_deal.h"
#include "lcd.h"

#define     TIMEOUT_Gun_Insert           30

//��ʾ��ǹ �����ǹ���� ��⳵�������� ��ʼ��� ����ADת������cp��ѹֵ
//������ǹ�복��δ������ʾ������ǹ���ȴ���ʱ��ֹͣ�˴γ�粢������Ϣ֪֡ͨ��������
//�����������ʼ���cp���ѹ����ʼ��� cp���PWM


//�ȴ�ɨ���� ��ʾ��ά��
void Station_Idle_deal(void)
{
    //��Ȩͨ�� �ҳ��ǹ���ӳ���
    if((Charge_Flag & (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT)) == (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT) ) 
    {
        Station_State = STATION_Charge;
        return;
    }
    else if(Charge_Flag & CHANGE_AUTHTIC_BIT)
    {
        Station_State = STATION_Authtic;
        return;
    }
    else if(Charge_Flag & CHANGE_GUN_CONN_BIT)
    {
        Station_State = STATION_Connect;
        return;
    }
}    


//���ǹ���ӵȴ�ǩȨ
void Station_Connect_deal(void)
{
    if((Charge_Flag & (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT)) == (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT) )
    {
        Station_State = STATION_Charge;
        return;
    }
}

//���ǹǩȨ�ȴ�����
void Station_Authtic_deal(void)
{
   
    if(Time_Seconds - Wait_Gun_Insert_Seconds > TIMEOUT_Gun_Insert) //��ǹ��ʱ
    {
        Station_State = STATION_SETUP;
        
        //��ʾ����ǹ��ʱ
    }
    
    if((Charge_Flag & (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT)) == (CHANGE_AUTHTIC_BIT|CHANGE_GUN_CONN_BIT) )
    {
        Station_State = STATION_Charge;
        return;
    }    
}

//����� ��ʾ��������Ϣ
void Station_Charge_deal(void)
{    
    //�������Գ�� �л�CP�������PWM
    if(Charge_Flag == (CHANGE_AUTHTIC_BIT|CHANGE_CAR_READY_BIT|CHANGE_GUN_CONN_BIT)) 
    {
        Charge_Flag |= CHANGE_ING_BIT;
        P1P16ON();//�򿪳��̵�������  //220�̵���������� 
        ChargeBeginTime = Time_Seconds;
        SET_CHG_KSSJ();
        return;
    } 
    else if(Charge_Flag == (CHANGE_AUTHTIC_BIT|CHANGE_CAR_READY_BIT|CHANGE_GUN_CONN_BIT|CHANGE_ING_BIT)) //�����
    {
        if(PWRPINSTS )//��״̬ �������� �ߵ�ƽ�̵����Ͽ�
        {
            if(!(DisplayFlag&DisplayErrButtonOff))
            {
                DisplayFlag |= DisplayErrButtonOff;
                DisplayOpenButton();
            }
        }
        else
            DisplayFlag |= DisplayChargeInfoFlag;
    }
}


//������ ��ʾ�ۿ��綩��������Ϣ
void Station_ChargeEND_deal(void)
{
//    Station_State = STATION_SETUP;
}

void Station_ChargeError_deal(void)
{
    
}

/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/


