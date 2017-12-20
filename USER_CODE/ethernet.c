/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           ethernet.c
** Last modified Date:  2017-10-17
** Last Version:        V0.01
** Descriptions:        it_isr  
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-10-17
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
#include "ethernet.h"

//uint8_t Src_IP_Add[4] =         {192,168,1,10};
//uint8_t Subnet_Mask_Add[4] =    {255,255,255,0};
//uint8_t Gateway_Add[4] =        {192,168,1,1};//·����ip211.157.135.58

//uint8_t Src_MAC_Add[6] =        {0,0,0,5,122,0};
//uint8_t Sn_DIP_Add[4] =         {192,168,1,14};
//uint8_t Sn_Port[2] =            {0x09,0x66};//2406

//���Ӳ��Է�����
uint8_t Src_IP_Add[4] =         {192,168,1,140};
uint8_t Subnet_Mask_Add[4] =    {255,255,255,0};
uint8_t Gateway_Add[4] =        {192,168,1,1};

uint8_t Src_MAC_Add[6] =        {0,0,0,5,122,0};
uint8_t Sn_DIP_Add[4] =         {47,93,185,40};
uint8_t Sn_Port[2] =            {0x09,0x66};//2406

uint8_t RX_Buffer[RX_Size];
uint8_t TX_Buffer[TX_Size];        

uint8_t PTL104_State;           //bit7 login bit6 activation bit5 time bit4 JF_strategy 
                                //bit3 Tx  bit2 Rx 

uint8_t Log_In_Frame[25] = {0x68,0x01,0x02,0x84,0x01,0x01,0x10,0x11,0x11,0x22,
                            0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0xae,0x21,0x02,0x17,};                       
  
                            
/*****************************************************************
������: W5100_Initialization
����: ��
���: ��
����: ��
˵�����ȶ�W5100��ʼ����Ȼ�������أ����ֱ��ʼ��4���˿�
*****************************************************************/
void W5100_Initialization(void)
{
//	S0_State = 0 ; 
//	W5100_Load_Parameters() ; 	// ������̫������
//	W5100_Init();				// ��ʼ����̫������
//	OSTimeDly(10) ; 
//	Detect_Gateway();			// ������ط����� 
//	OSTimeDly(10) ; 
//	Socket_Init(0);				// �˿�0 
//	OSTimeDly(10) ; 
}
                            
void Ethernet_Init(void)
{
	if(Term_Para1.CHGMode == MODE_GPRS)
    {
//		GPRSPWReset();
    }
	if(Term_Para1.CHGMode == MODE_WIFI)
    {
//		wifienterATmode();
    }
	if(Term_Para1.CHGMode == MODE_4G)
    {
//		Module4GInit();
    }
	if(Term_Para1.CHGMode == MODE_RJ45)   
    {   
        PTL104_State = 0;
        iinchip_init();
    }
}    
  
uint8_t Ethernet_config(void)
{
    uint8_t read_buffer[8];
    OSTimeDly(OS_TICKS_PER_SEC/10);
    setGAR(Term_Para1.GateWay); 
    getGAR(read_buffer);
    
    setSUBR(Term_Para1.SelfMASK);
    setSHAR(Term_Para1.SelfMAC);
    setSIPR(Term_Para1.SelfIP);
    getSIPR(read_buffer);
    
    sysinit(0x55,0x55);
    
    setRTR(0xd007);
    setRCR(0x03);
    setIMR(0xcf);
    
    OSTimeDly(OS_TICKS_PER_SEC/20);;
    
    if(!socket(0,Sn_MR_TCP,Term_Para1.SelfPort,0x20))//(5369  0x14f9 ) (4658,0x1232)
    {
//        GPIO_OutputValue(LED_PORT,LED_PIN,0); 
    }
    setSn_DIPR(0,Term_Para1.ServerIP);
}


uint8_t			Ethernet_ReCnc_Ctr= 0;      //������������

void Ethrenet_Connecting(uint8_t Value)
{
    uint8_t SN_SR;
    SN_SR = getSn_SR(0);
    if(Value&Sn_IR_CON)												/* ��TCPģʽ��,Socket0�ɹ����� */
    {
        if(SN_SR == SOCK_ESTABLISHED){                
            Ethernet_State = ETHERNET_CONNECT_OK; 
            Ethernet_ReCnc_Ctr = 0;
            PTL104_State = 0;
            time_flg &= 0x80;
            putISR(0,Value&(~Sn_IR_CON));
        }
        else
            Ethernet_State = ETHERNET_CONNECTING;
        return;
    }
    if(Value&Sn_IR_TIMEOUT)
    {
        Ethernet_ReCnc_Ctr++;
        if(Ethernet_ReCnc_Ctr == 2){
             Ethernet_State = ETHERNET_FAILURE;
        } 
        putISR(0,Value&(~Sn_IR_TIMEOUT));
    }
    if(Value&Sn_IR_DISCON)
    {
        putISR(0,Value&(~Sn_IR_DISCON));
    }
    switch(SN_SR)
    {
        case SOCK_CLOSED:
            connect(0,Term_Para1.ServerIP,Term_Para1.ServerPort);
            break;
        case SOCK_CLOSE_WAIT:
            close(0);
            break;        
    }
    return;
}

//�����ȷ��֡  ���׮���͵� ��Ҫ������ȷ�ϵ� ֡һ��ʱ��δ�յ�ȷ�� �����·��͸�֡
uint8_t Save_NoAck_Frame(uint8_t const *sbuf,uint8_t len)
{
    
}

//ɾ����ȷ��֡   ȷ���Ժ�ɾ������ĸ�֡
void Del_Ack_Frame(uint16_t Txsn)
{
    
}


//���͵�¼֡
uint8_t Sent_Login_Frame(uint8_t *pNo)
{
    uint8_t len,sBuf[30];
    len = F104_LOGIN_PACK(sBuf,pNo,0);
//    Save_NoAck_Frame(sBuf,len);
    return send(0,sBuf,len);
}

//��������֡
void Sent_Heart_Frame(void)
{
    uint8_t FRM_HEART[]   = {0x68,0x04,0x43,0x00,0x00,0x00};
//    Save_NoAck_Frame(FRM_HEART,6);
    send(0,FRM_HEART,6);
}

//����Sȷ��֡
void Sent_S_Ack(uint16_t Txsn)
{
    uint8_t FRM_S_Ack[]   = {0x68,0x04,0x01,0x00,0x00,0x00};
    FRM_S_Ack[4] = Txsn%256;
    FRM_S_Ack[5] = Txsn/256;
    send(0,FRM_S_Ack,6);
}


//uint8_t FRM_ACTIVE[]  = {0x68,0x04,0x07,0x00,0x00,0x00};
//���ͼ���ȷ��֡   FRM_RACTIVE
void Sent_RACTIVE(void)
{
    uint8_t FRM_RACTIVE[] = {0x68,0x04,0x0B,0x00,0x00,0x00};
    send(0,FRM_RACTIVE,6);
}

//����ʱ��ͬ��ȷ��
void Sent_Rsettime(unsigned short Rxsn,unsigned short Sdsn)
{
    uint8_t	  len ,dbuf[32];
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson = 0x07;
    sPTL_104_I_SND.Sdsn = Rxsn;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);//��������ȷ��֡	
}

//�����¼�״̬�ϱ�֡
void Sent_Event_Frame(void)
{
    
}

//�������ٻ�ȷ��
void R_Call_Ack(unsigned short Rxsn,unsigned short Sdsn)
{
    uint8_t	  len ,dbuf[32];
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson = 0x07;
    sPTL_104_I_SND.Sdsn = Rxsn;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);//��������ȷ��֡											
//  F104_ACK_SAVE(dbuf,len);    
}

void Charge_State_Update(void)
{
    memcpy(sPTL_104_I_100.CDZBH,Term_Para1.ChargeNo,8);						//�ն˻������루�����׮��ţ�

    switch(Station_State)
    {
        case STATION_SETUP:                     
        case STATION_Idle:                  //�ȴ�ɨ���� ��ʾ��ά��      
        case STATION_Connect:               //�����ӳ��ǹ�ȴ�����Ȩ  
        case STATION_Authtic:               //��ǩȨ�ȴ����ǹ����
            sPTL_104_I_100.GZZT = 0x02;
            break;             
        case STATION_Charge:                //����� ��ʾ��������Ϣ
            sPTL_104_I_100.GZZT = 0x03;
            break;        
        case STATION_ChargeEND:             //������ ��ʾ�ۿ��綩��������Ϣ            
            sPTL_104_I_100.GZZT = 0x04;
            break;            
        default : 
            sPTL_104_I_100.GZZT = 0x01;
            break;
    }

    switch(Charge_Flag&0x60)
    {
        case 0x0:
            sPTL_104_I_100.LJQRKGZT = 0x00;
            break;
        case 0x40:
            break;
        case 0x20:
            sPTL_104_I_100.LJQRKGZT = 0x01;
            break;
        case 0x60:
            sPTL_104_I_100.LJQRKGZT = 0x02;
            break;
        default:
            break;
    }   
    if(PWRPINSTS )//��״̬ �������� �ߵ�ƽ�̵����Ͽ�
        sPTL_104_I_100.SCJDQZT = 0x00;
    else
        sPTL_104_I_100.SCJDQZT = 0x01;
}

//���ͳ��׮״̬
void Charge_State_Ack(unsigned short const Rxsn,unsigned short const Sdsn)
{
    uint8_t	  len ,dbuf[50];    
    sPTL_104_I_SND.Type = 0x86;
    sPTL_104_I_SND.Rson = 0x00;

    Charge_State_Update();//���׮��Ϣ����
    len = F104_ZZHSJ_PACK(&sPTL_104_I_SND);
    sPTL_104_I_SND.Flen = 10+len;
    sPTL_104_I_SND.Sdsn = Rxsn+2;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);							
    send(0,dbuf,len);	   												//������������֡
//  F104_ACK_SAVE(dbuf,len);
}

//�������ٻ�����
void R_Call_Over(unsigned short const Rxsn,unsigned short const Sdsn)
{
    uint8_t	  len ,dbuf[32];      
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson = 0x0a;
    sPTL_104_I_SND.Sdsn = Rxsn+4;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);													//�������ٽ���֡
//  F104_ACK_SAVE(dbuf,len);	
//  sPTL_104_I_SND.Sdsn += 2;
}

//�ظ����������ٻ� �˴����淢�ͽ������кţ��ȴ���Ӧ��sȷ��֡ ȷ�ϣ�����ȴ���ʱ����Ӧ��֡�ط�
void Answer_R_Call(unsigned short const Rxsn,unsigned short const Sdsn)
{
    R_Call_Ack(Rxsn,Sdsn);//�������ٻ�ȷ��֡
    Charge_State_Ack(Rxsn,Sdsn);//���ͳ��׮״̬
    R_Call_Over(Rxsn,Sdsn);//�������ٻ�����      
}

//����ϵͳʱ��
void Set_Sys_Time(void )
{  	
	RTCtimedate.second = (sPTL_104_I_RCV.Dobj[1]+sPTL_104_I_RCV.Dobj[2]*256)/1000;
    RTCtimedate.minute 	= sPTL_104_I_RCV.Dobj[3];
	RTCtimedate.hour	= sPTL_104_I_RCV.Dobj[4];
	RTCtimedate.day		= sPTL_104_I_RCV.Dobj[5]&0x1f;
	RTCtimedate.week	= sPTL_104_I_RCV.Dobj[5]>>5;
	RTCtimedate.month	= sPTL_104_I_RCV.Dobj[6]&0x0f;
	RTCtimedate.year	= sPTL_104_I_RCV.Dobj[7]+2000;

	rtcInit(&RTCtimedate);		//�����ڲ�RTC
	ExRTCSet(&RTCtimedate);		//�����ⲿRTC
}


//MD5_ADD                
//MD5_BAK_ADD            
//CHARGE_STRATEGY_ADD    
//CHARGE_STRATEGY_BAK_ADD
//����ƷѲ���
void Save_Charge_Strategy(uint16_t len)
{ 
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3],MD5_ADD,16);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+16],CHARGE_STRATEGY_RECODE_NUM_ADD,1);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+17],CHARGE_STRATEGY_ADD,96);   

//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3],MD5_BAK_ADD,16);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+16],CHARGE_STRATEGY_RECODE_NUM_BAK_ADD,1);
//    FRAMWrite(&sPTL_104_I_RCV.Dobj[3+17],CHARGE_STRATEGY_BAK_ADD,96);     
}    



uint8_t Authtic_Success[120];
//��Ȩ֡����  ���潻����ˮ�� �����Ȩ��Ϣ ��ȫ�ּ�Ȩ������
void Authtic_Deal_With(uint16_t len)
{
    if(len == 0x66)
        PTL104_JQXXWithOrder_unpack(&sPTL_104_I_RCV);		//��Ȩ��Ӧ����	����Ϊ0x66        
    else if(len == 0x76)
        PTL104_JQXXWithOrderAndLogicNumber_unpack(&sPTL_104_I_RCV);   //��Ȩ��Ӧ���� ����Ϊ0x76
    if(DspJQFHerr(sPTL_104_I_DW_JQSJ.JQSBYY)==0)        //��Ȩ�ɹ� 
    {
        Wait_Gun_Insert_Seconds = Time_Seconds;
        Charge_Flag |= CHANGE_AUTHTIC_BIT;

        if(GUNPINSTS)       //ǹ������Ϊ 0 ǹ���������
        {
            P3P0OFF();          //��ǹ��
            P3P1ON();
            OSTimeDly(OS_TICKS_PER_SEC);
            P3P1OFF();   
        }        
        
        memcpy(Authtic_Success,&sPTL_104_I_RCV.Dobj[0],len-12);
    }
}

//����ң��ȷ��֡  unsigned short Rxsn,unsigned short Sdsn
void SendRemoteCtrlAsk(unsigned short const Rxsn,unsigned short const Sdsn)
{
    uint8_t	  len ,dbuf[32];      
    memcpy(&sPTL_104_I_SND,&sPTL_104_I_RCV,sizeof(sPTL_104_I_RCV));
    sPTL_104_I_SND.Rson =   0x07;
    sPTL_104_I_SND.Sdsn = Rxsn;
    sPTL_104_I_SND.Rxsn = Sdsn+2;
    sPTL_104_I_SND.Apdu += 1;
    sPTL_104_I_SND.Dobj[6] = 0x01;
    len = F104_INF_PACK(dbuf,&sPTL_104_I_SND);
    send(0,dbuf,len);   
}


//ң��ֹͣ֡����
void RemoteCtrlFrameDeal(unsigned short const Rxsn,unsigned short const Sdsn)
{
    //ֹͣ��� ���� ���㶩��  �ϴ��ƷѲ��Գ����� 
    SendRemoteCtrlAsk(Rxsn,Sdsn);    //����ң��ȷ��֡
    
    pwmDb1Init(HIVCNT);
    P1P16OFF();         //220�̵����������  �رճ��ǹ220v���
    SET_CHG_JSSJ();     //���ó�����ʱ��
    Charge_Flag |= CHANGE_END_Ctrl_BIT;         //APP������             
    Charge_Flag &= ~CHANGE_AUTHTIC_BIT;         //ǩȨλ��0
    Station_State = STATION_ChargeEND;          //���ĳ��׮״̬Ϊ������״̬
    DisplayWaitTimeSeconds = Time_Seconds;

}

void PayForDile(uint8_t len)
{
    PTL104_KKXX_With_LogicNumber_unpack(&sPTL_104_I_RCV);
}

//�������ݽ��� �������յ�����
void PTL104DataAnalysis(uint8_t const * rx_data,uint16_t len)
{
	uint8_t data = 0,frame_len = 0,minus_len  = 0;
    
    data = *rx_data;
    if(data == 0x68)	//�ж��Ƿ�Ϊ֡ͷ
    {
        uint16_t sent_temp,rece_temp;
        sent_temp = *(rx_data+2)+*(rx_data+3)*256;
        rece_temp = *(rx_data+4)+*(rx_data+5)*256;
        
        memcpy(&sPTL_104_I_RCV,rx_data,len);//memcpy(&sPTL_104_I_RCV,rx_data,sizeof(sPTL_104_I_RCV));
        frame_len = *(rx_data+1);
        minus_len = frame_len+2;    //��ǰ֡����
        switch(frame_len){
            case 0x01://Ϊ��¼����֡
                PTL104_State |= PTL104_LOGIN;
                PTL104_State &= ~PTL104_LOGIN_SENT;
                minus_len = 25;
                break;
            case 0x04://����֡,����ȷ��֡��Sȷ��֡ 
                data = *(rx_data+2);
                if(data == 0x07){//����֡ �ظ�����ȷ��
                    Sent_RACTIVE();
                    PTL104_State |= PTL104_ACTV;
                }
                else if(data == 0x83){//����ȷ��֡ 
                    time_flg &= ~PTL104_HEART_WAIT_ACK;
                }
                else if(data == 0x01){//Sȷ��֡ 
//                    Del_Ack_Frame(*(rx_data+2)+*(rx_data+3)*256-2);
                }
                break;
            case 0x0e://Ϊ���ٻ� �� ң��֡              
                data = *(rx_data+6);
                if(data == 0x64){//Ϊ���ٻ� 
                    Answer_R_Call(sent_temp,rece_temp);
                }
                else if(data == 0x45){//Ϊң��֡ ����  
                    RemoteCtrlFrameDeal(sent_temp,rece_temp);
                }
                break; 
            case 0x12://Ϊʱ��ͬ�� 
                data = *(rx_data+6);
                if(data == 0x67)                //Ϊʱ��ͬ�� 
                {
                    Set_Sys_Time();//ͬ��ʱ�� ����ʱ��
                    PTL104_State |= PTL104_SETTIME;
                    Sent_Rsettime(sent_temp,rece_temp);     //����ʱ��ͬ��ȷ��֡ 
                }
                break;
            case 0x4e://Ϊ�ۿ�            
                PayForDile(frame_len);//��ʾ�ۿ���Ϣ
                break;            
            case 0x72://��ά���·�                               
                break;            
            case 0x7e://Ϊ�ƷѲ���   �ظ�Sȷ��֡    
                Save_Charge_Strategy(len);//����ƷѲ���
                PTL104_State |= PTL104_JF_STRGY;
                Sent_S_Ack(sent_temp+2);
                break;
            case 0x56:
            case 0x66:
            case 0x76://Ϊ��Ȩ����  �ظ�Sȷ��֡     0x66
                Authtic_Deal_With(frame_len);                        //���ݼ�Ȩ��Ϣ�򿪳��׮��� ���� ��׼����
                Sent_S_Ack(*(rx_data+2)+*(rx_data+3)*256+2);
                break;               
            default: 
                break;	
        }
        if(len > minus_len)
        {
            PTL104DataAnalysis(rx_data+minus_len,len - minus_len);
        }
    }
}

extern CHASTRATEGY_UP_PARA ChargeStrategyUP;
void Sent_StrateChargeInfo_Frame(sPtl_104_i * pPtl104)
{
	uint8_t len;
	uint8_t sBuf[BUFSIZE];
	pPtl104->Type = 0x79;
	pPtl104->Lmtd = 0x01;
	pPtl104->Rson = 0x02;
	pPtl104->Apdu = 0x00;
	SET_JFCLSJ(&ChargeStrategyUP);
	len = F104_JFCLCDQK_UP_PACK(pPtl104);
	pPtl104->Flen = 10+len;
	len = F104_INF_PACK(sBuf,pPtl104);
	send(0,sBuf,len);
//	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;	    
}

void Sent_OnlineChargeInfo_Frame(sPtl_104_i * pPtl104)
{
	uint8_t len;
	uint8_t sBuf[BUFSIZE];
	pPtl104->Type = 0x82;
	pPtl104->Lmtd = 0x01;
	pPtl104->Rson = 0x03;
	pPtl104->Apdu = 0x00;
	SET_ZXTZSJ();
	len = F104_ZXSJSJ_PACK(pPtl104);
	pPtl104->Flen = 10+len;
	len = F104_INF_PACK(sBuf,pPtl104);
	send(0,sBuf,len);
//	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;	
//	isOnlinePayBack = 1;
}


//����������ӽ����Ժ�ҵ��ִ��
void Ethernet_Work(uint8_t Value)
{
    uint8_t SN_SR;
    SN_SR = getSn_SR(0); 
    if(Value & Sn_IR_DISCON){                             //����Ͽ�
        Ethernet_State = ETHERNET_CONNECTING;
        return;
    }    
    if(SN_SR == SOCK_ESTABLISHED){                          //����״̬
        if(Value & Sn_IR_RECV){                             //���յ�����
            uint16_t len = getSn_RX_RSR(0);
            if(len < RX_Size){
                recv(0,RX_Buffer,len);
                PTL104DataAnalysis(RX_Buffer,len);          //�������ݽ��� ����������յ�����
            }
            putISR(0,Value&(~Sn_IR_RECV));
        }
        if(!(PTL104_State&PTL104_LOGIN)){               //�豸δ��¼�����͵�¼֡ �ȴ���¼ȷ��
            if(!(PTL104_State&PTL104_LOGIN_SENT) || \
                ((PTL104_State&PTL104_LOGIN_SENT)&&  \
            (Time_Seconds-Wait_Ack_Seconds>TIMEOUT_LOGIN_W_A))){
                
                if(Sent_Login_Frame(Term_Para1.ChargeNo) == 0)
                {
                    Ethernet_State = ETHERNET_CONNECTING;   //����ʧ�ܶ˿����ӹر���Ҫ��������
                }
                PTL104_State |= PTL104_LOGIN_SENT;
                Wait_Ack_Seconds = Time_Seconds;
            }
        }
        
        if(PTL104_State>=0xe0)//��¼�ɹ� ���׮���� ʱ��ͬ�� ���
        {
            if((Time_Seconds != Wait_Heart_Ack_Seconds) && \
                (!(Time_Seconds%SECONDS_PER_HEART_BEAT) || \
                (time_flg&PTL104_TIME_TO_HEART))){   //��������ʱ�䵽
                Sent_Heart_Frame();
                time_flg &= ~PTL104_TIME_TO_HEART;
                time_flg |= PTL104_HEART_WAIT_ACK;
                Wait_Heart_Ack_Seconds = Time_Seconds;
            }
            
            if(time_flg&PTL104_HEART_WAIT_ACK){
                if(Time_Seconds-Wait_Heart_Ack_Seconds >= TIMEOUT_HEART_W_A){
                    time_flg |= PTL104_TIME_TO_HEART;
                    //��ʱ��ʶ��1
                }
            }
            
            if((Time_Seconds != Wait_Event_Ack_Seconds) && \
                (!(Time_Seconds%SECONDS_PER_EVENT_UPDATE) \
                || (time_flg&PTL104_TIME_TO_EVENT))){//�����¼�״̬ʱ�䵽
//                Sent_Event_Frame();
                time_flg &= ~PTL104_TIME_TO_EVENT;
                time_flg |= PTL104_EVENT_WAIT_ACK; 
                Wait_Event_Ack_Seconds = Time_Seconds;
                
            }
            
            if(time_flg&PTL104_EVENT_WAIT_ACK){
                if(Time_Seconds-Wait_Event_Ack_Seconds >= TIMEOUT_EVENT_W_A){
                    time_flg |= PTL104_TIME_TO_EVENT;//�ط��¼��ϱ�֡
                    //��ʱ��ʶ��1
                }
            }   
            //���ͼƷѲ��Գ����� �Լ����߳���¼�ϴ�
            if((Charge_Flag&CHANGE_END_Ctrl_BIT)||(Charge_Flag&CHANGE_END_Full_BIT)||(Charge_Flag&CHANGE_END_Pull_BIT))    
            {
                Charge_Flag &= ~(CHANGE_END_Ctrl_BIT|CHANGE_END_Full_BIT|CHANGE_END_Pull_BIT);
                Charge_Flag |= CHANGE_END_PAY;
                Sent_StrateChargeInfo_Frame(&sPTL_104_I_SND);       //�ƷѲ��Գ�����
                Sent_OnlineChargeInfo_Frame(&sPTL_104_I_SND);       //���߳�����    
            }
            //���ͼ�Ȩ֡ ˢ���¼�����ʱ����
        }
        
    }
    else
        Ethernet_State = ETHERNET_CONNECTING;
}
/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/




                            