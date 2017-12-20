/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-08-08
** Last Version:        V0.01
** Descriptions:        The main() function 
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-09-30
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
#include "main.h"

/*********************************************************************************************************
  �궨��
*********************************************************************************************************/

//�����������ȼ�����
#define     MainTask_Prio           10
#define     UartCfgTas_Prio         15
#define     NetTask_Prio	        18
#define     RFCardTask_Prio	        13
#define     Uart485EleMeTas_Prio     19
#define     DisplayTas_Prio         30

/*********************************************************************************************************
  ����ȫ�ֱ�������
*********************************************************************************************************/
static OS_STK stkMainTask[1024];			//������
static OS_STK stkNetTask[1024];			    //��������
static OS_STK stkRFCardTask[1024];			//ˢ������
static OS_STK stkUartCfgTask[1024];			//�������ò�������
static OS_STK stkUart485Task[1024];		    //485��������
static OS_STK stkDisplayTask[1024];         //��ʾ����

uint8_t			time_flg;                   //bit7 secflgÿ���ʶ ˢ��LCD
uint8_t         Charge_Flag = 0;                //���׼����ʶ ǩȨ�ɹ���־bit7 ǹ���ӱ�ʶbit5
uint8_t         DisplayFlag = 0;            //LCD��Ļ��ʾ��ʶλ
//#define         CHANGE_READY_OK         0xA0
//#define         CHANGE_AUTHTIC_BIT      0x80
//#define         CHANGE_GUN_CONN_BIT     0x20

uint8_t         Ethernet_State = 0;         //����״̬
uint8_t         Station_State  = 0;         //���׮״̬

uint32_t    Time_Seconds;                   //��ʼ��ʱʱ�� ��λs ϵͳ�ϵ�����ʱ��
uint32_t    Wait_Ack_Seconds;               //�ȴ�Ӧ���ʱ ��λs
uint32_t    Wait_Heart_Ack_Seconds;         //�����ȴ���ʱ ��λs
uint32_t    Wait_Event_Ack_Seconds;         //�¼��ȴ���ʱ ��λs

uint32_t    DisplayWaitTimeSeconds;         //��ʾ�ȴ�ʱ��
uint32_t    Wait_Gun_Insert_Seconds;        //�ȴ����ǹ���� ��λs
uint32_t    ChargeBeginTime;                //��翪ʼʱ��

const uint8_t softVer[4] = {9,01,17,20};

/*********************************************************************************************************
** Function name:       RFCardTask
** Descriptions:        RFˢ������
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void RFCardTask(void *pvData)
{
	pvData = pvData;
        
	while (1) 
	{
        OSTimeDly(OS_TICKS_PER_SEC/4);
	}
}

/*********************************************************************************************************
** Function name:       NetTask
** Descriptions:        ������������
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void NetTask(void *pvData)
{
	pvData = pvData;
    
	while (1) 
	{ 
        uint8_t Sn_IR_Value;
        if(W5100_INT_STS()){
            ISR_Process();
        }
        Sn_IR_Value = getISR(0);
        switch(Ethernet_State){
            case ETHERNET_INIT:
                Ethernet_Init();
                Ethernet_State = ETHERNET_CFG;
                break;
            case ETHERNET_CFG:
                Ethernet_config();
                if(connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort)){ 
                    Ethernet_State = ETHERNET_CONNECTING;
                }
                break;
            case ETHERNET_CONNECTING://2
                Ethrenet_Connecting(Sn_IR_Value);
                break;
            case ETHERNET_CONNECT_OK:
                Ethernet_Work(Sn_IR_Value);
                break; 
            case ETHERNET_DISCONNECT:
                if(connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort)){ 
                    Ethernet_State = ETHERNET_CONNECTING;
                }
                break; 
            case ETHERNET_FAILURE:
                if(connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort)){ 
                    Ethernet_State = ETHERNET_CONNECTING;
                }
                break; 
            default:
                Ethernet_State = ETHERNET_CFG;
                break;             
        }
        if(Ethernet_State != ETHERNET_CONNECT_OK)
        {
            if((Charge_Flag&CHANGE_END_Ctrl_BIT)||(Charge_Flag&CHANGE_END_Full_BIT)||(Charge_Flag&CHANGE_END_Pull_BIT))    
            {
                Charge_Flag &= 0xf0;
                Charge_Flag |= CHANGE_END_PAY;
//                Save_StrateChargeInfo_Frame(&sPTL_104_I_SND);       //����ƷѲ��Գ�����
//                Save_OfflineChargeInfo_Frame(&sPTL_104_I_SND);       //�������߳�����    
            }            
        }
        OSTimeDly(OS_TICKS_PER_SEC/10);
	}
}

/*********************************************************************************************************
** Function name:       Uart485_E_M_Task
** Descriptions:        485��������
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart485_E_M_Task(void *pvData)
{
	pvData = pvData;
    
    main_rs232();

}

/*********************************************************************************************************
** Function name:       UartCfgTask
** Descriptions:        ������������
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UartCfgTask(void *pvData)
{
	pvData = pvData;
    
	main_uart1();
}

/*********************************************************************************************************
** Function name:       DisplayTask
** Descriptions:        ��ʾ����
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void DisplayTask(void *pvData)
{
    pvData = pvData;
    
    while(1)
    {
    	if(time_flg & SECFLG)
        {
    		time_flg &= ~SECFLG;
    		dspTimeAndIcon(Term_Para1.CHGMode);
       	} 
        switch(Station_State)
        {
            case STATION_SETUP: 
                sbhmaskflag=0;  
                dspstandbypage();               //LCD��ʾ��ʼ����                     
                Charge_Flag = 0x0;
                DisplayFlag = 0;
                memset(&sPTL_104_I_DW_JQSJ,0,sizeof(sPTL_104_I_DW_JQSJ));
                Station_State = STATION_Idle;            
                break;
            case STATION_Idle:                  //�ȴ�ɨ���� ��ʾ��ά��                 
                break;      
            case STATION_Connect:               //�ȴ�����Ȩ
                
                break;   
            case STATION_Authtic:               //�ȴ����ǹ����
                Display_GUN_Insert();           //��ʾ�뽫���ǹ����綯����������
                break;             
            case STATION_Charge:                //����� ��ʾ��������Ϣ
                if(DisplayFlag&DisplayChargeInfoFlag)
                    DisplayChargeInfo();
                break;        
            case STATION_ChargeEND:             //������ ��ʾ�ۿ��綩��������Ϣ
                DisplayChargeEndInfo();
                if(Time_Seconds - DisplayWaitTimeSeconds > TIMEOUT_DISPLAYWAIT)
                {                  
                    Station_State = STATION_SETUP;
                }

                
//                OSTimeDly(OS_TICKS_PER_SEC*8);  //��������Ϣ��ʾ��ʱ  �����������ʱ��Ӱ������
                break;            
            default : 
                DisplayChargeError();
                break;
        }   
        OSTimeDly(OS_TICKS_PER_SEC/5);        
    }
}

////ֱ�ӱ��������
//void SaveCurrent(void)
//{
//	OS_ENTER_CRITICAL();
//	SysCtlWrod.cs	= CheckSum1((uint8_t *)&SysCtlWrod, (uint8_t *)&SysCtlWrod.cs);
//	memcpy((uint8_t *)&SysCtlWrodBak,(uint8_t *)&SysCtlWrod,sizeof(SysCtlWrodBak));
//	FRAMWrite((uint8_t *)&SysCtlWrodBak,CHGPARA_ADD,sizeof(SysCtlWrodBak));
//	FRAMWrite((uint8_t *)&SysCtlWrodBak,CHGPARA_ADD_BAK,sizeof(SysCtlWrodBak));
//	OS_EXIT_CRITICAL();
//}
////������״̬�³�����
//void SavePara2Fram(void )
//{	
//	if(SysCtlWrod.card1pwdok==1){
//		if(memcmp((uint8_t *)&SysCtlWrodBak,(uint8_t *)&SysCtlWrod,sizeof(SysCtlWrodBak))!=0)
//			SaveCurrent();
//	}					
//}

/*********************************************************************************************************
** Function name:       mainTask
** Descriptions:        ������ ��ʾ��ʾ��Ϣ
** input parameters:    pvData: û��ʹ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void mainTask(void *pvData)
{
    char QrCode[60] = { 0x68,0x74,0x74,0x70,0x3A,0x2F,0x2F,0x77,0x78,0x2E,
                        0x74,0x65,0x6C,0x6C,0x75,0x73,0x70,0x6F,0x77,0x65,
                        0x72,0x2E,0x63,0x6E,0x3F,0x63,0x6F,0x64,0x65,0x3D,
                        0x31,0x39,0x2C,0x31,0x31,0x30,0x38,0x2C,};
	pvData = pvData;
       
	GolbalVarInit();										//������ʼ��
	SystemParaInit();										//������ʼ��
          
    //���ɶ�ά�� ������ m_byModuleData  �汾��Ϣ m_nVersion
    //��ά���ַ�������//"http://wx.telluspower.cn?code=19,1108,1322111110001676"
   
//    FillQrCode(QrCode);
//    EncodeData(QrCode);
                        
    EncodeData("http://wx.telluspower.cn?code=19,1108,1322111110001676");
    
    T6963_Init();								   			//LCD��ʾ��ʼ��
    dspstandbypage();                                       //LCD��ʾ��ʼ����   
    ADCInit(ADC_CLK);                                       //ADC��ʼ��
    
    OSTaskCreate(UartCfgTask, (void *)0, &stkUartCfgTask[sizeof(stkUartCfgTask) / 4 - 1], UartCfgTas_Prio);
    OSTaskCreate(NetTask, (void *)0, &stkNetTask[sizeof(stkNetTask) / 4 - 1], NetTask_Prio);
    OSTaskCreate(RFCardTask, (void *)0, &stkRFCardTask[sizeof(stkRFCardTask) / 4 - 1], RFCardTask_Prio);
    OSTaskCreate(Uart485_E_M_Task, (void *)0, &stkUart485Task[sizeof(stkUart485Task) / 4 - 1], Uart485EleMeTas_Prio);
    OSTaskCreate(DisplayTask, (void *)0, &stkDisplayTask[sizeof(stkDisplayTask) / 4 - 1], DisplayTas_Prio);
    
    Station_State = STATION_SETUP;
    pwmDb1Init(HIVCNT);
    PWMCON();                       //cp �������Ӽ̵���     ���� P1.5
    OSTimeDly(OS_TICKS_PER_SEC/4);  //��΢��ʱ  �����ѹ���
    
//    P3P0OFF();          //��ǹ��
//    P3P1ON();
//    OSTimeDly(OS_TICKS_PER_SEC);
//    P3P1OFF();    
//    if(GUNPINSTS)       //ǹ������Ϊ 0
//    {
//        P3P1OFF(); 
//    }

//    P3P0ON();           //����ǹ��
//    P3P1OFF();
//    OSTimeDly(OS_TICKS_PER_SEC);
//    P3P0OFF();
//    if(GUNPINSTS)       //ǹ������Ϊ 0
//    {
//        P3P0OFF();
//    }    
    
	while (1) 
	{
//        if(!GUNPINSTS)       //ǹ������Ϊ0 
//        {
//            P3P0ON();           //����ǹ��
//            P3P1OFF();
//            OSTimeDly(OS_TICKS_PER_SEC);
//            P3P0OFF();  
//        }
        CP_Pin_Dector();
        switch(Station_State)
        {
            case STATION_SETUP: //������������0 
                break;
            case STATION_Idle:                  //�ȴ�ɨ���� 
                Station_Idle_deal();
                break;      
            case STATION_Connect:               //�ȴ�����Ȩ
                Station_Connect_deal();
                break;   
            case STATION_Authtic:               //�ȴ����ǹ����
                Station_Authtic_deal();
                break;             
            case STATION_Charge:                //����� 
                Station_Charge_deal();
                break;        
            case STATION_ChargeEND:             //������ 
                Station_ChargeEND_deal();
                break;            
            default : 
                Station_ChargeError_deal();
                break;
        }	
        if(Station_State==STATION_Charge)
            OSTimeDly(OS_TICKS_PER_SEC/25);
        else
            OSTimeDly(OS_TICKS_PER_SEC/5);
	}
}
/*********************************************************************************************************
** Function name:       main
** Descriptions:        �û�������ں���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main(void)
{
    
    SSP0_Init();													// rf_FM1702 & FM25Cl64 SPI�ӿ�
    SSP1_Init();													// W5100 SPI�ӿ�	

    i2c1Init(100000);                                               // ��ʼ��I2C1 ʵʱʱ��PCF8563
    
    All_Relay_Init();
    
    main_rtc();
    
    
    OSInit();                                                                                                
    OSTaskCreate(mainTask, (void *)0, &stkMainTask[sizeof(stkMainTask) / 4 - 1], MainTask_Prio);
    OSStart();
}
/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

