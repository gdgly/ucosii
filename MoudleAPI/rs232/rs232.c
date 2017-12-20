/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2009-05-12
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chengmingji
** Created date:        2009-07-24
** Version:             V1.00
** Descriptions:        ����û�Ӧ�ó���
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         Li Baihua
** Modified date:       2009-07-27
** Version:				V1.01
** Descriptions:        ��дUART2����
**
** Rechecked by:        Zhang bin1
*********************************************************************************************************/
#include "rs232.h"

/*********************************************************************************************************
  ������궨��
*********************************************************************************************************/
#define UART2_BPS  	2400 												/* ����ͨ�Ų�����               */
/*********************************************************************************************************/
#define FEILV			1			//����
#define READCTW			0x11
#define METERNOLEN	    6			//����ų���
#define METDATLEN		12		    //������ݳ���
#define	METERNO			2 		    //������
#define	METERIDNO		3			//�������������

/*********************************************************************************************************
  
*********************************************************************************************************/
uint8_t               GucRcvBuf[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               GucSndBuf[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint32_t              GulNum;                                                     /*  ���ڽ������ݵĸ���          */
volatile            uint32_t              RcvLen;                                 /*  ���ڽ������ݵĸ���          */
volatile            uint32_t              SndLen;                                 /*  ���ڽ������ݵĸ���          */
volatile            uint8_t               GucRcvNew;                              /* ���ڽ��������ݵı�־         */

//extern uint8_t bgntimesave;
/*********************************************************************************************************
  ����������ݽṹ���� 
*********************************************************************************************************/
typedef struct {	
	uint32_t	ID;						//���ݱ�ʶ
	uint8_t	Slen;						//�������ݳ���	
	uint8_t	Rlen;						//�������ݳ���
	uint8_t	Data[METDATLEN];			//��������
}sData;

typedef struct {
	uint8_t 	meterflag;				//������ñ�־
	uint8_t 	meterno[METERNOLEN];	//�����
	sData IDdata[METERIDNO];		    //���г������ݱ�ʶ 
}sMeter;

uint8_t ReadFirstFlag;
sMeter Meter[METERNO];				    //���

uint8_t         MD5[16];                //�ƷѲ��Ե�MD5��
uint8_t         Strategy[24*4];         //����0-23
uint8_t         RecodeNum;             //�ƷѲ��Եļ�¼����

uint8_t	MD5_BAK[16];
uint8_t RecodeNum_BAK;
uint8_t Strategy_BAK[24*4];
uint8_t StrategyRev = 0;

//const uint8_t  MeterNoDB[METERNO][METERNOLEN]  = { 0x03,0x50,0x17,0x19,0x11,0x13,
//											  0x03,0x50,0x17,0x19,0x11,0x13,
//											};
//const uint8_t  MeterNoDB[METERNO][METERNOLEN]  = { 0x32,0x00,0x00,0x80,0x13,0x20,
//											  0x32,0x00,0x00,0x80,0x13,0x20,
//											};
const uint8_t  MeterNoDB[METERNO][METERNOLEN]  = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
											  0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
											};
											//��ѹ ���� ����
const uint32_t MeterNoIDDB[METERNO][METERIDNO] = { 0x0201ff00,0x0202ff00,0x00010000,	  	
											  0x0201ff00,0x0202ff00,0x00010000,	
											};
uint32_t RxMeterDtLen(uint32_t id )
{
	uint32_t Rlen=0;

  	switch(id)
	{
		case 0x00010000:	//����		
			Rlen =4;		
			break;
		case 0x02010100:	//��ѹ		
			Rlen =2;		
			break;
		case 0x02020100:	//����		
			Rlen =3;		
			break;
		case 0x0201ff00:	//��ѹ��		
			Rlen =2*3;		
			break;
		case 0x0202ff00:	//������		
			Rlen =3*3;		
			break;

		default:
		break;	
	
	}

	return Rlen;
}

void MeterParaInit(void )
{
	uint32_t i,j;
	for(i=0;i<METERNO;i++)
	{
		if(i==0)
			Meter[i].meterflag = 1;
		else
			Meter[i].meterflag = 0;	
		memcpy((uint8_t *)&Meter[i].meterno[0],(uint8_t *)&MeterNoDB[i][0],METERNOLEN);
		for(j=0;j<METERIDNO;j++)
		{
			Meter[i].IDdata[j].ID = MeterNoIDDB[i][j];
			Meter[i].IDdata[j].Slen = 4;
			Meter[i].IDdata[j].Rlen = 0;
			memset(Meter[i].IDdata[j].Data,0,METDATLEN);
		}	
	}	
}

/*********************************************************************************************************
** Function name:   	delayNS 
** Descriptions:	    ��ʱ����
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void delayNS (uint32_t ulDly)
{
    uint32_t i;
    
    for (; ulDly > 0; ulDly--) {
        for (i = 0; i < 50000; i++);
    }
}

uint8_t     strategy_kwh[24*2];     //�ƷѲ����еĵ���

/*********************************************************************************************************
** Function name:	    DLT645_2007CRC
** Descriptions:	    CRC ����
** input parameters:    pDLT645_2007CF ��ָ��Ҫ�����Э�黺����
** output parameters:   ��
** Returned value:      CRC
*********************************************************************************************************/
void ChargeStrategyMeterKwh(uint32_t meter_cur_kwh)
{
	static uint8_t recode_cnt = 0;
	static uint8_t cs_time[96] = {0};
	uint8_t md5[15] = {0};
	uint16_t cur_time, cs_start, cs_stop, i = 0,j = 0;
	static uint16_t cs_kwh[24];
	static uint8_t csp = 0;
	static uint32_t cskwbg = 0;	//ĳʱ��ο�ʼ����
	static uint16_t cskwlast = 0; // �ϴα���ĵ���
	uint8_t cspb;
    uint32_t kwh_buf;
	
	if (!ReadFirstFlag)
	{
		ReadFirstFlag = 1;
			
		recode_cnt = 0;
		memset(md5, 0, sizeof(md5));
		memset(cs_time, 0, sizeof(cs_time));

        OS_ENTER_CRITICAL();
        FRAMRead(md5, MD5_ADD ,16);
        FRAMRead(&recode_cnt, CHARGE_STRATEGY_RECODE_NUM_ADD ,1);
        FRAMRead(cs_time, CHARGE_STRATEGY_ADD , 24*4);
        OS_EXIT_CRITICAL();	

        if(recode_cnt == 0 || (cs_time[0] == 0 && cs_time[1] == 0 && cs_time[2] == 0 && cs_time[3] == 0))
        {
            OSTimeDly(OS_TICKS_PER_SEC/20);	
            OS_ENTER_CRITICAL();
            FRAMRead(md5, MD5_BAK_ADD ,16);
            FRAMRead(&recode_cnt, CHARGE_STRATEGY_RECODE_NUM_BAK_ADD ,1);
            FRAMRead(cs_time, CHARGE_STRATEGY_BAK_ADD , 24*4);  //�ӱ�������ȡ���µļƷѲ���
            OS_EXIT_CRITICAL();
					
            if(recode_cnt == 0 || (cs_time[0] == 0 && cs_time[1] == 0 && cs_time[2] == 0 && cs_time[3] == 0))
            {
                if(RecodeNum_BAK && StrategyRev && (Strategy_BAK[0] != 0 || Strategy_BAK[1] != 0 || Strategy_BAK[2] != 0 || Strategy_BAK[3] != 0))
                {
                    memcpy(md5,MD5_BAK,16);
                    recode_cnt = RecodeNum_BAK;
                    memcpy(cs_time,Strategy_BAK,96);
                }
                else
                {
                    recode_cnt = 1;
                    memset(cs_time,0,96);
                    cs_time[0] =  0;
                    cs_time[1] =  0;
                    cs_time[2] =  23;
                    cs_time[3] =  59;
                }
            }						
        }

        RecodeNum = recode_cnt;

        memcpy(MD5, md5, 16);
        memcpy(Strategy, cs_time, 96);

        csp = 0;
	}

	cur_time = ((uint16_t)RTCtimedate.hour << 8) | RTCtimedate.minute;
	
	if(recode_cnt)
	{
		for(i = 0; i < recode_cnt; i++)
		{
			cs_start = ((uint16_t)cs_time[i*4] << 8) | cs_time[i*4+1];
			cs_stop = ((uint16_t)cs_time[i*4+2] << 8) | cs_time[i*4+3];
			//ʱ��μ���
			if(cs_start <= cs_stop)
			{
				if(cur_time >= cs_start && cur_time <= cs_stop)
				{
					cspb = i+1;
					if(csp == cspb)
					{
						if(meter_cur_kwh >= cskwbg)
						{
							cs_kwh[i] = (uint16_t)(meter_cur_kwh - cskwbg) + cskwlast;
						}	
						else
						{
							cs_kwh[i] = cskwlast;
						}
					}	
					else
					{						
						cskwbg = meter_cur_kwh;
						kwh_buf = 0;
						for(j = 0;j < recode_cnt;j ++)
						{							
							if(j != i) kwh_buf += cs_kwh[j];							
						}
						if(meter_cur_kwh >= kwh_buf)							
						{
							cskwlast = (meter_cur_kwh - kwh_buf);
						}
						else
						{
							cskwlast = cs_kwh[i];	
						}						
						csp = cspb;
					}
					strategy_kwh[i*2] = cs_kwh[i] & 0xFF; 
					strategy_kwh[i*2+1] = (cs_kwh[i] >> 8) & 0xFF;
					break;
				}								
			}	
			else if(cs_start > cs_stop)
			{
				if(cur_time >= cs_start || cur_time <= cs_stop)
				{
					cspb = i+1;
					if(csp == cspb)
					{
						if(meter_cur_kwh >= cskwbg)
						{
							cs_kwh[i] = (uint16_t)(meter_cur_kwh - cskwbg) + cskwlast;
						}	
						else
						{
							cs_kwh[i] = cskwlast;
						}
					}	
					else
					{						
						cskwbg = meter_cur_kwh;
						kwh_buf = 0;
						for(j = 0;j < recode_cnt;j ++)
						{							
							if(j != i) kwh_buf += cs_kwh[j];							
						}
						if(meter_cur_kwh >= kwh_buf)							
						{
							cskwlast = (meter_cur_kwh - kwh_buf);
						}
						else
						{
							cskwlast = cs_kwh[i];	
						}	
						csp = cspb;
					}
					strategy_kwh[i*2] = cs_kwh[i] & 0xFF; 
					strategy_kwh[i*2+1] = (cs_kwh[i] >> 8) & 0xFF;
					break;
				}
			}
		}
	}
	else
	{
		strategy_kwh[0] = meter_cur_kwh & 0xFF;
		strategy_kwh[1] = (meter_cur_kwh >> 8) & 0xFF; 
	}
	
}

/*********************************************************************************************************
** Function name:	    uart2Init
** Descriptions:	    UART2��ʼ��������Ϊ8λ����λ��1λֹͣλ��żУ�飬������ΪUART2_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart2Init (void)
{
    uint32_t Fdiv;
	LPC_SC->PCONP |= 0x01000000;
	LPC_IOCON->P0_10 &= ~0x07;
	LPC_IOCON->P0_10 |= 1;                                      /* P0.10ΪU2_TXD                */
	LPC_IOCON->P0_11 &= ~0x07;
	LPC_IOCON->P0_11 |= 1;                                      /* P0.11ΪU2_RXD                */

	LPC_UART2->LCR = 0x9b;        
	Fdiv = ( PeripheralClock / 16 ) / (UART2_BPS*2) ;           /*�����ֵ                      */
	LPC_UART2->DLM = Fdiv / 256;
	LPC_UART2->DLL = Fdiv % 256;
	LPC_UART2->LCR = 0x1b;                                     	/* ������������                 */
	LPC_UART2->FCR = 0x07 | 0x80;                              	/* ʹ�ܲ���λFIFO               */

	NVIC_EnableIRQ(UART2_IRQn);

	LPC_UART2->IER = IER_RBR | IER_RLS;

	ENRD485();																									
}
/*********************************************************************************************************
** Function name:	    Uart2RxbufClear
** Descriptions:	    ������ջ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart2RxbufClear(void )
{
    GucRcvNew = 0;                                              		/* �����־*/
    RcvLen = 0;
    memset(GucRcvBuf,0,sizeof(GucRcvBuf));
}
/*********************************************************************************************************
** Function name:	    uart2SendByte
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart2SendByte (uint8_t ucDat)
{
    LPC_UART2->THR = ucDat;                                           /* д������                     */
    while ((LPC_UART2->LSR & 0x20) == 0);                             /* �ȴ����ݷ������             */
}
/*********************************************************************************************************
** Function name:	    uart2SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart2SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
	
    ENWR485();
	OSTimeDly(3);
	Uart2RxbufClear();	
    for (i = 0; i < ulNum; i++) {                                       /* ����ָ�����ֽ�����           */
        uart2SendByte (*pucStr++);
    }
	OSTimeDly(3);
	ENRD485();
}

/*********************************************************************************************************
* Function Name  : UART2_IRQHandler
* Description    : UART2 �жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART2_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;

  	uiIIRValue = LPC_UART2->IIR;

    uiIIRValue >>= 1;                                                     /* �жϱ�־�Ĵ���ֵ����1λ*/
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {                                        /* ������״̬�� */
        uiLSRValue = LPC_UART2->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
            uiDummy = LPC_UART2->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {                                     /* ����FIFO��Ϊ���ж� */
            for (GulNum = 0; GulNum < 8; GulNum++){                      /*  ��������8���ֽ�*/
                if ( GulNum+RcvLen >= BUFSIZE ) {
                    Uart2RxbufClear();
                }
                GucRcvBuf[GulNum+RcvLen] = LPC_UART2->RBR;
            }
            RcvLen+=GulNum;
            GucRcvNew = 1; 					
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {                                   /* �������ݿ����ж�*/
        for (GulNum = 0; GulNum < 8; GulNum++){                      /*  ��������8���ֽ� */
            if ( GulNum+RcvLen >= BUFSIZE ) {
                Uart2RxbufClear();
            }
            GucRcvBuf[GulNum+RcvLen] = LPC_UART2->RBR;
        }
        RcvLen+=GulNum;
        GucRcvNew = 1; 						
    }
    else if ( uiIIRValue == IIR_CTI ) {                 /* �ַ���ʱ�ж�               */
        while ((LPC_UART2->LSR & 0x01) == 0x01){  
            if(RcvLen >= BUFSIZE){
                Uart2RxbufClear();
            }                       					/*  �ж������Ƿ�������      */ 
            GucRcvBuf[RcvLen] = LPC_UART2->RBR;
            RcvLen++;
        }
        GucRcvNew = 1; 									/* 	���յ���������֡ 		*/
    }
    else if ( uiIIRValue == IIR_THRE ) {                /* ���ͱ��ּĴ������ж�       */
        uiLSRValue = LPC_UART2->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            //UART2TxEmpty = 1;
        } else {
            //UART2TxEmpty = 0;
        }
    }
}

uint8_t CheckSum0904(uint8_t *pdat,uint16_t dlen)
{	
	uint8_t  cs=0;
	uint16_t i,j,len;
		
	for(i=0;i<dlen-MINFLEN0904;i++)
	{
	 	if(	*(pdat+i)!=0x68 || *(pdat+i+7)!=0x68)
			continue;
		len = *(pdat+i+9) + *(pdat+i+10) * 256;
		if(len>MAXFLEN0904-13-i)
			break;
		for(j=0;j<len+11;j++)
		  	cs += *(pdat+i+j);		
		if(cs != *(pdat+i+j))
			break;
		if(0x16 != *(pdat+i+j+1))
			break;
		return 1;
	}
	return 0;
}

void ClearDataBuffer(void)
{
    sPTL_104_I_100.CDSCDY = 0;
    sPTL_104_I_100.CDSCDL = 0;
    sPTL_104_I_100.YGZDD  = 0;

    ReadFirstFlag = 0;
    if(sPTL_104_I_UP_ZXSJ.PQSZ != 0)
    {
        memset(&sPTL_104_I_UP_ZXSJ,0,sizeof(sPTL_104_I_UP_ZXSJ));
    }
    
//  bgntimesave = 0;
}

uint32_t ndbcdstr2u32(uint8_t const *pointer,uint16_t len)//�����λ�ں�
{
    uint8_t i,j,temp;
    uint32_t ret=0;
    
    for(i= 0;i<len;i++)
    {
        temp = (*(pointer+(len-1-i))>>4)*10+(*(pointer+(len-1-i))&0x0f);
        ret = ret*100+temp;
    }
    return ret;
}

/*********************************************************************************************************
** Function name:       main_rs232
** Descriptions:        UART2����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void main_rs232 (void)
{
    uint32_t 	i = 0,j = 0,k,tmp;
    uint32_t 	dval,pbgn,pend,mny;
    uint32_t 	timdiv,head;
    uint32_t 	rlen,cur;
    uint8_t 	*pdat;
    uint8_t 	tmpbuf[6],dtmpbuf[3];

    OSTimeDly(OS_TICKS_PER_SEC); 																			//��ʱ��������������

    uart2Init();                                   												            //���ڳ�ʼ��   	
    MeterParaInit();																						//��ʼ���������
    Uart2RxbufClear();	

    while(1){
        OSTimeDly(OS_TICKS_PER_SEC/2);
        if((STATION_Charge != Station_State) && (STATION_ChargeEND != Station_State))
        {
            k=0;
            ClearDataBuffer();
            continue;
        }
        if((Charge_Flag&CHANGE_ING_BIT)&&(Station_State == STATION_Charge))
        {                                               
            for(j = 0;j<3;j++)                      //���ĵ���������ѹ����BCD��
            {
                SndLen = FramePack_645_2007(Meter[i].meterno,READCTW,Meter[i].IDdata[j].ID,(uint8_t *)&Meter[i].IDdata[j].Data,4,GucSndBuf);
                uart2SendStr(GucSndBuf,SndLen);
                for(timdiv=0;timdiv<4;timdiv++){							   
                    OSTimeDly(OS_TICKS_PER_SEC/2);
                    if(RcvLen < MINFLEN0904)
                        continue;
                    head = FrameCheck_645_2007(GucRcvBuf,Meter[i].meterno,Meter[i].IDdata[j].ID);            

                    if(head < MAXFRMLEN-VALUEOFFSET){
                        k=0;						
                        rlen = GucRcvBuf[head+9]-4;																//���ݳ���
                        pdat = GucRcvBuf+VALUEOFFSET+head;							   					        //����ָ��
                        memcpy((uint8_t *)&Meter[i].IDdata[j].Data[0],pdat,rlen);
                        if(Meter[i].IDdata[j].ID==0x00010000){ 	                  //����										        //�����й��ܵ���			
        					dval = ndbcdstr2u32(GucRcvBuf+VALUEOFFSET+head,4);				                    //ת����ǰ����						
                            if(dval != 0)
                            {	
                                pbgn = sPTL_104_I_UP_ZXSJ.PQSZ;
                                if(pbgn==0)
                                {
                                    sPTL_104_I_UP_ZXSJ.PQSZ = dval;													//????????,????
                                    sPTL_104_I_UP_LXSJ.PQSZ = dval;													//????????,????
                                    sPTL_104_I_UP_ZXSJ.PZSZ = dval;							 						//ƽֹʾֵ����λС��
                                    sPTL_104_I_UP_LXSJ.PZSZ = dval;
                                }
                                else
                                {
                                    if(dval < pbgn)
                                    {
                                        sPTL_104_I_UP_ZXSJ.PQSZ = dval;													//��������ƽ��ʾֵ����λС��
                                        sPTL_104_I_UP_LXSJ.PQSZ = dval;													//��������ƽ��ʾֵ����λС��
                                        sPTL_104_I_UP_ZXSJ.PZSZ = dval;							 						//ƽֹʾֵ����λС��
                                        sPTL_104_I_UP_LXSJ.PZSZ = dval;							 						//ƽֹʾֵ����λС��
                                    }
                                    else
                                    {
                                        sPTL_104_I_UP_ZXSJ.PZSZ = dval;							 						//ƽֹʾֵ����λС��
                                        sPTL_104_I_UP_LXSJ.PZSZ = dval;							 						//ƽֹʾֵ����λС��
                                        pend = dval - sPTL_104_I_UP_ZXSJ.PQSZ;					                //ͨ������																	
                                        sPTL_104_I_UP_ZXSJ.ZDL = pend;													//�ܵ�������λС��
                                        sPTL_104_I_UP_LXSJ.ZDL = pend;							   					    //�ܵ�������λС��
                                        sPTL_104_I_UP_ZXSJ.PDL = pend;													//ƽ��������λС��
                                        sPTL_104_I_UP_LXSJ.PDL = pend;														//�ܵ�����һλС��																									
                                        sPTL_104_I_100.YGZDD = pend;
                                                                
                                        mny = pend*FEILV;		        //�ƷѲ����õ���
                                        ChargeStrategyMeterKwh(mny);
                                    }
                                }
                            }
                        }
                        else if(Meter[i].IDdata[j].ID==0x0201ff00){		//��ѹ	 	
                            tmp =  Meter[i].IDdata[j].Data[0];
                            tmp += (Meter[i].IDdata[j].Data[1]<<8);
                            sPTL_104_I_100.CDSCDY = tmp;            //����ʵʱ����ѹ
                            if(rlen>2)
                            {
                                if (Meter[i].IDdata[j].Data[2] != 0xFF && Meter[i].IDdata[j].Data[3] != 0xFF && Meter[i].IDdata[j].Data[4] != 0xFF &&
                                                Meter[i].IDdata[j].Data[5] != 0xFF)
                                {
                                }
                                else
                                {
                                }	
                            }
                        }
                        else if(Meter[i].IDdata[j].ID==0x0202ff00){				//����							
                            Meter[i].IDdata[j].Data[2] &= 0x7f;					//���η���λ
                            Meter[i].IDdata[j].Data[5] &= 0x7f;
                            Meter[i].IDdata[j].Data[8] &= 0x7f;
                            cur =  Meter[i].IDdata[j].Data[0];
                            cur += (Meter[i].IDdata[j].Data[1]<<8);
                            cur += (Meter[i].IDdata[j].Data[2]<<16);  
                            sPTL_104_I_100.CDSCDL = cur >> 8;
                            if(rlen>3)
                            {
                                if (Meter[i].IDdata[j].Data[3] != 0xFF && Meter[i].IDdata[j].Data[4] != 0xFF && Meter[i].IDdata[j].Data[5] != 0xFF &&
                                        Meter[i].IDdata[j].Data[6] != 0xFF && Meter[i].IDdata[j].Data[7] != 0xFF && Meter[i].IDdata[j].Data[8] != 0xFF)
                                {

                                }
                                else
                                {

                                }
                                cur =  dtmpbuf[2];
                                cur += (dtmpbuf[1]<<8);
                                cur += (dtmpbuf[0]<<16);                                
                            }						
                        }
                        break;
                    } 
                }
            }                
        }
    }
}


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
