/****************************************Copyright (c)****************************************************
**                            ShangHai YUNWANG *** Co.,LTD.
**
**                                 http://www.***.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL104.c
** Latest modified Date:       2014-1-6
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 AndyLau
** Created date:               2014-1-6
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2014-1-6
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include 	"ptl104.h"

#define SHEAD 	0  			//	I֡�ֶ�ƫ����
#define SFLEN 	1
#define SSDSN 	2
#define SRXSN 	4
#define STYPE 	6
#define SLIMT 	7
#define SRSON 	8
#define SAPDU 	10
#define SDOBJ 	12

#define TMLEN 	8 			//	�ն��豸���볤��


/************************************************************************
**
**	m1�����ݽṹ��
**
************************************************************************/
#define UIDLEN  4
#define MNELEN	4
#define PSWLEN	6

typedef struct
{
	uint32_t		No ;
	uint8_t		Moeny[MNYLEN] ;
	uint8_t		Password[PWDLEN] ;
	uint8_t		UserID[UIDLEN] ;
	BOOLEAN 	Chgstate ; 
	uint8_t	 	Vaild ;
	uint8_t 		Work; 
}CHGCARD_PARA;


sFor_Ack_Idx		ForAckIdx[FIFODEEP];
sUart_DlyTim		UartDlyTim;
sPtl_104_i 			sPTL_104_I_SND;
sPtl_104_i 			sPTL_104_I_RCV;
sPtl_104_i_100		sPTL_104_I_100;
sPtl_104_i_100		sPTL_104_I_100_BAK;
sPtl_104_i_tim		sPTL_104_I_TIM;
CHECKUP_PARA 		sPTL_104_I_UP_JQSJ;
CHGONLINE_PARA		sPTL_104_I_UP_ZXSJ;     //��������
CHGOFFLINE_PARA		sPTL_104_I_UP_LXSJ;

CHECKDOWN_PARA		sPTL_104_I_DW_JQSJ;     //����������·���Ȩ����

BALANCEDOWN_PARA	sPTL_104_I_DW_KKXX;
BLACKLIST_PARA 		sPTL_104_I_DW_HMD;
RATE_PARA			sPTL_104_I_DW_BZFL;
uint8_t				sPTL_104_COMOUT_COUNT;		//104ͨѶ��ʱ����				

uint8_t isOnlinePayBack;                        //���߿ۿ��Ƿ񷵻أ����׮���ϴ������߳���¼֮�󣬻Ὣ�˱����1,����յ��������·��Ŀۿ��,�ͽ���������0


//uint8_t         MD5[16];                //�ƷѲ��Ե�MD5��
//uint8_t         Strategy[24*4];         //����0-23
//uint8_t         RecodeNum;             //�ƷѲ��Եļ�¼����

//uint8_t	MD5_BAK[16];
//uint8_t RecodeNum_BAK;
//uint8_t Strategy_BAK[24*4];
//uint8_t StrategyRev = 0;



CHASTRATEGY_UP_PARA ChargeStrategyUP;

extern const uint8_t softVer[4];									//����汾��
extern uint8_t ReadFirstFlag;



void uart3SendStr(uint8_t *sBuf,uint8_t len)
{
    
}

//UART_RECV_FIFO		UartRecvFifo[FIFODEEP];
//UART_SEND_FIFO		UartSendFifo[FIFODEEP]; 

//�����ȷ������֡
uint8_t F104_ACK_STAT( void )
{
	uint8_t i;	
 	for(i=0;i<FIFODEEP;i++)
	{
		if(ForAckIdx[i].flag)
		{
			return 1;
		}
	}
	return 0;
}

//�ж��Ƿ��д�ȷ������֡
uint8_t Is_F104_UnACK(void)
{
	uint8_t i;	
 	for(i=0;i<FIFODEEP;i++){
		if(ForAckIdx[i].flag==1){
			break;
		}
	}
	
	//������еĶ����ж�û����Ҫȷ�ϵ�֡���ͷ���0
	if(i==FIFODEEP)
		return 0;
	else
		return 1;
}

//ȷ�ϴ�ȷ������֡
void F104_ACK_SIDX(uint16_t sidx)
{
	uint8_t i;	
 	for(i=0;i<FIFODEEP;i++){
		if(ForAckIdx[i].flag==1 && ForAckIdx[i].Sidx+2 == sidx){
			memset((uint8_t*)&ForAckIdx[i],0,sizeof(sFor_Ack_Idx));	
		}
	}
}
//�����ȷ������֡
void F104_ACK_SAVE(uint8_t *pbuf,uint8_t len)
{
	uint8_t i;	
 	for(i=0;i<FIFODEEP;i++){
		if(ForAckIdx[i].flag==0){
			ForAckIdx[i].flag = 1;
			ForAckIdx[i].Sidx = *(pbuf+2)+*(pbuf+3)*256;
			memcpy((uint8_t*)&ForAckIdx[i].Fdat,pbuf,len);	
			break;
		}
	}
}

//��Ϣ֡(I֡)���
uint8_t F104_INF_PACK(uint8_t *sBuf,sPtl_104_i * pPtl104)
{
	uint8_t	len;
	uint8_t	*psBuf;

	psBuf = sBuf;
	*psBuf++ = 0x68;
	*psBuf++ = pPtl104->Flen;
	*psBuf++ = pPtl104->Sdsn%256;
	*psBuf++ = pPtl104->Sdsn/256;
	*psBuf++ = pPtl104->Rxsn%256;
	*psBuf++ = pPtl104->Rxsn/256;	
	*psBuf++ = pPtl104->Type;
	*psBuf++ = pPtl104->Lmtd;
	*psBuf++ = pPtl104->Rson%256;
	*psBuf++ = pPtl104->Rson/256;
	*psBuf++ = pPtl104->Apdu%256;
	*psBuf++ = pPtl104->Apdu/256;
	if(pPtl104->Flen>10)
	{
		memcpy(psBuf,&pPtl104->Dobj[0],pPtl104->Flen-10);
		psBuf+=pPtl104->Flen-10;
	}
	len = pPtl104->Flen + 2;

	return len;
}

//ң��ȷ��֡���
uint8_t F104_RC_PACK(uint8_t *rcBuf,sPtl_104_i * rcPtl104)
{
	uint8_t	len;
	uint8_t	*psrcBuf;

	psrcBuf = rcBuf;
	*psrcBuf++ = 0x68;
	*psrcBuf++ = rcPtl104->Flen;
	*psrcBuf++ = rcPtl104->Sdsn%256;
	*psrcBuf++ = rcPtl104->Sdsn/256;
	*psrcBuf++ = rcPtl104->Rxsn%256;
	*psrcBuf++ = rcPtl104->Rxsn/256;	
	*psrcBuf++ = rcPtl104->Type;
	*psrcBuf++ = rcPtl104->Lmtd;
	*psrcBuf++ = rcPtl104->Rson%256;
	*psrcBuf++ = rcPtl104->Rson/256;
	*psrcBuf++ = rcPtl104->Apdu%256;
	*psrcBuf++ = rcPtl104->Apdu/256;
	memcpy(psrcBuf,&rcPtl104->Dobj[0],rcPtl104->Flen-10);
	psrcBuf+=rcPtl104->Flen-11;
	*psrcBuf++ = 0x01;
	len = rcPtl104->Flen + 2;

	return len;
}

uint16_t	dbcd2hex16(uint16_t dd )
{
	uint8_t b1,b2,b3,b4;
	b1 = dd &0x0f;
	b2 = (dd >> 4)&0x0f;
	b3 = (dd >> 8)&0x0f;
	b4 = (dd >> 12)&0x0f;	
	return b4*1000+b3*100+b2*10+b1;
}

uint32_t	dbcd2hex32(uint32_t dd )
{
	uint8_t 	b1,b2,b3,b4,b5,b6,b7,b8;
	uint32_t tmp;

	b1 = dd &0x0f;
	b2 = (dd >> 4) &0x0f;
	b3 = (dd >> 8) &0x0f;
	b4 = (dd >> 12)&0x0f;
	b5 = (dd >> 16)&0x0f;
	b6 = (dd >> 20)&0x0f;
	b7 = (dd >> 24)&0x0f;
	b8 = (dd >> 28)&0x0f;
	tmp = 	b4*1000+b3*100+b2*10+b1;
	tmp += 	b8*10000000+b7*1000000+b6*100000+b5*10000;
	return tmp;
}

//���ٻ����ݴ��
uint8_t F104_ZZHSJ_PACK(sPtl_104_i * pPtl104)
{
    uint8_t	i=0;
    uint32_t	tmp;

    pPtl104->Dobj[i++] = 0x01;	//��¼����

    tmp = dbcd2hex16(sPTL_104_I_100.CDSCDY);  //��ѹ��Ϣ �ϴ�
    pPtl104->Dobj[i++] = tmp%256;
    pPtl104->Dobj[i++] = tmp/256;
    tmp = dbcd2hex16(sPTL_104_I_100.CDSCDL);    //������Ϣ �ϴ�
    pPtl104->Dobj[i++] = tmp%256;
    pPtl104->Dobj[i++] = tmp/256;
    pPtl104->Dobj[i++] = sPTL_104_I_100.SCJDQZT;

    //����ȷ�Ͽ���
    pPtl104->Dobj[i++] = sPTL_104_I_100.LJQRKGZT;

    memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_100.YGZDD,4);   

    i+=4;
    memcpy(&pPtl104->Dobj[i],sPTL_104_I_100.CDZBH,8);
    i+=8;
    pPtl104->Dobj[i++] = sPTL_104_I_100.SFLJDC;
    pPtl104->Dobj[i++] = sPTL_104_I_100.GZZT%256;
    pPtl104->Dobj[i++] = sPTL_104_I_100.GZZT/256;
	memcpy(&pPtl104->Dobj[i],&sPTL_104_I_DW_JQSJ.OrderNo,16);
//    memset(&pPtl104->Dobj[i],0,16);
    i+=16;
    return i;		
}
//�������ݴ��
uint8_t F104_LXSJSJ_PACK(sPtl_104_i *pPtl104)
{
	uint8_t	i=0;
	
	pPtl104->Dobj[i++] = 0x05;	//��¼����   0x03

	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_LXSJ.ZDJQBM,8);
	i+=8;
	
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JYLSH,16);
	i+=16;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.YHBH,8);
	i+=8;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.LJKH,8);
	i+=8;
	
	////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_LXSJ.LOGIC_NUM,16);
	i+=16;
	////////////////////////////////////////////////////////////
	
	pPtl104->Dobj[i++] = sPTL_104_I_UP_LXSJ.LXJYLX;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.KSSJ,7);
	i+=7;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JSSJ,7);
	i+=7;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.FQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.FZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.PQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.PZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.GQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.GZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JLLX,2);
	i+=2;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.BCJLSS,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.SCJLSS,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JDJ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.JJE,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.FDJ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.FDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.FJE,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.PDJ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.PDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.PJE,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.GDJ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.GDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.GJE,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.ZDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.YWLX,2);
	i+=2;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.XFSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.XFDJ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.XFJE,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_LXSJ.DDQCWYBS,32);
	i+=32;
	
	return i;		
}
////�������ݴ��
//uint8_t F104_ZXSJSJ_PACK(sPtl_104_i *pPtl104)
//{
//	uint8_t	i=0;
//	if(Charge_Flag & CHANGE_END_Full_BIT)
//        pPtl104->Dobj[i++] = 0x02;	//��¼����
//    else if(Charge_Flag & CHANGE_END_Pull_BIT)
//        pPtl104->Dobj[i++] = 0x01;	//��¼����
//    else if(Charge_Flag & CHANGE_END_Ctrl_BIT)
//        pPtl104->Dobj[i++] = 0x03;	//��¼����
//    else
//        pPtl104->Dobj[i++] = 0x04;	//��¼����
//	//�����ͨ��΢�š�APPɨ����ģ���֮ǰ�������·��Ķ����żӵ�����¼����
////	if(SysCtlWrod.chargeByCardOrApp == 2)	
////		memcpy(&pPtl104->Dobj[i],(uint8_t *)&SysCtlWrod.OrderNo,16);
////	else
////		memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JYLSH,16);
//	
//    memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_DW_JQSJ.OrderNo,16);
//    
//	i+=16;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.YWLX,2);
//	i+=2;
//	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.ZDJQBM,8);
//	i+=8;
//	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.YHBH,8);
//	i+=8;
//	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.LJKH,8);
//	i+=8;
//	
//	////////////////////////////////////////////////////////
//	///////////////////////////////////////////////////////////////
//	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.LOGIC_NUM,16);
//	i+=16;
//	////////////////////////////////////////////////////////////
//	
//	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.KSSJ,7);
//	i+=7;
//	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.JSSJ,7);
//	i+=7;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JQSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JZSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.FQSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.FZSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.PQSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.PZSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.GQSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.GZSZ,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JDL,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.FDL,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.PDL,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.GDL,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.ZDL,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JLSSLX,2);
//	i+=2;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.BCJLSS,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.SCJLSS,4);
//	i+=4;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.SCCHDZBH,5);
//	i+=5;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.SCYWLX,2);
//	i+=2;
//	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.DDQCWYBS,32);
//	i+=32;
//	return i;		
//}

//�������ݴ��
uint8_t F104_ZXSJSJ_PACK(sPtl_104_i *pPtl104)
{
	uint8_t	i=0;
	if(Charge_Flag & CHANGE_END_Full_BIT)
        pPtl104->Dobj[i++] = 0x02;	//��¼����
    else if(Charge_Flag & CHANGE_END_Pull_BIT)
        pPtl104->Dobj[i++] = 0x01;	//��¼����
    else if(Charge_Flag & CHANGE_END_Ctrl_BIT)
        pPtl104->Dobj[i++] = 0x03;	//��¼����
    else
        pPtl104->Dobj[i++] = 0x04;	//��¼����
	//�����ͨ��΢�š�APPɨ����ģ���֮ǰ�������·��Ķ����żӵ�����¼����
//	if(SysCtlWrod.chargeByCardOrApp == 2)	
//		memcpy(&pPtl104->Dobj[i],(uint8_t *)&SysCtlWrod.OrderNo,16);
//	else
//		memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JYLSH,16);
	
    memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_DW_JQSJ.OrderNo,16);
    
	i+=16;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.YWLX,2);
	i+=2;
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.ZDJQBM,8);
	i+=8;
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.YHBH,8);
	i+=8;
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_DW_JQSJ.LJKH,8);
	i+=8;
	
	////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.LOGIC_NUM,16);
	i+=16;
	////////////////////////////////////////////////////////////
	
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.KSSJ,7);
	i+=7;
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_ZXSJ.JSSJ,7);
	i+=7;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.FQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.FZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.PQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.PZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.GQSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.GZSZ,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.FDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.PDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.GDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.ZDL,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JLSSLX,2);
	i+=2;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.BCJLSS,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.SCJLSS,4);
	i+=4;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.SCCHDZBH,5);
	i+=5;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.SCYWLX,2);
	i+=2;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.DDQCWYBS,32);
	i+=32;
	return i;		
}

//�ƷѲ������ݴ��
uint8_t F104_JFCLCDQK_UP_PACK(sPtl_104_i *pPtl104)
{
	uint8_t	i=0;
	
	pPtl104->Dobj[i++] = 0x01;	//��¼����

	//�����ͨ��΢�š�APPɨ����ģ���֮ǰ�������·��Ķ����żӵ�����¼����
//	if(SysCtlWrod.chargeByCardOrApp == 2)	
//    memcpy(&pPtl104->Dobj[i],(uint8_t *)&SysCtlWrod.OrderNo,16);
//	else
//		memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_UP_ZXSJ.JYLSH,16);
	
    memcpy(&pPtl104->Dobj[i],(uint8_t *)&sPTL_104_I_DW_JQSJ.OrderNo,16);
    
	i+=16;
	memcpy(&pPtl104->Dobj[i],(uint8_t *)&ChargeStrategyUP.MD5,16);
	i+=16;
	pPtl104->Dobj[i++] = ChargeStrategyUP.recode_num;	//��¼����
	
	memcpy(&pPtl104->Dobj[i],ChargeStrategyUP.strategy_kwh,24*6);
	i+=144;

	return i;		
}

//��Ȩ֡���ݴ��
uint8_t F104_JQZSJ_PACK(sPtl_104_i *pPtl104)
{
	uint8_t	i=0;
	pPtl104->Dobj[i++] = 0x01;	//��¼����
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_JQSJ.ZDJQBM,8);
	i+=8;
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_JQSJ.LJKH,8);
	i+=8;
	
	
	///////////////////////////////////////////////////////////////////
	//Ӧ������̨Ҫ�󣬼�Ȩ֡����16���ֽڵ��߼����ţ�ĿǰȫΪ0
	memset(&pPtl104->Dobj[i],0,16);		//���ӵ��߼����ţ�16��0
	i+=16;
	//////////////////////////////////////////////////////////////////
	
	
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_JQSJ.KMM,16);
	i+=16;
	memcpy(&pPtl104->Dobj[i],sPTL_104_I_UP_JQSJ.SRMM,16);
	i+=16;
	memset(&pPtl104->Dobj[i],0,4);		//������д
	i+=4;
	memset(&pPtl104->Dobj[i],0,2);		//״̬������д
	i+=2;
	memset(&pPtl104->Dobj[i],0,32);		//������д
	i+=32;
	memset(&pPtl104->Dobj[i],0,8);		//������д
	i+=8;
	return i;		
}

////���ü�Ȩ����
//void SET_JQSJ(CHGCARD_PARA * pChgcard)
//{
//	memcpy(sPTL_104_I_UP_JQSJ.ZDJQBM,Term_Para1.ChargeNo,8);

//	memset(sPTL_104_I_UP_JQSJ.LJKH,0,sizeof(sPTL_104_I_UP_JQSJ.LJKH));
//	bcdone2bcdtwolo(SysCtlWrod.card1no,sPTL_104_I_UP_JQSJ.LJKH,CDNLEN);

//	memset(sPTL_104_I_UP_JQSJ.KMM,0,sizeof(sPTL_104_I_UP_JQSJ.KMM));
//	memset(sPTL_104_I_UP_JQSJ.SRMM,0,sizeof(sPTL_104_I_UP_JQSJ.SRMM));
//	strchghead(sPTL_104_I_UP_JQSJ.SRMM,SysCtlWrod.card1psw,PWDLEN);
//	bcd2asc(sPTL_104_I_UP_JQSJ.SRMM,sPTL_104_I_UP_JQSJ.SRMM,PWDLEN);
//	memcpy(sPTL_104_I_UP_JQSJ.KMM,sPTL_104_I_UP_JQSJ.SRMM,PWDLEN);

//	sPTL_104_I_UP_JQSJ.KYE = 0;
//	sPTL_104_I_UP_JQSJ.KZT = 0;
//	memset(sPTL_104_I_UP_JQSJ.DDQCWYBS,0,sizeof(sPTL_104_I_UP_JQSJ.DDQCWYBS));
//	memset(sPTL_104_I_UP_JQSJ.JFMXBM,0,sizeof(sPTL_104_I_UP_JQSJ.JFMXBM));
//}

//��翪ʼʱ��
void SET_CHG_KSSJ(void )
{
	uint8_t tmp;

  tmp = (RTCtimedate.second * 1000) % 256;
	sPTL_104_I_UP_ZXSJ.KSSJ[0] = tmp;
	tmp = (RTCtimedate.second * 1000) / 256;
	sPTL_104_I_UP_ZXSJ.KSSJ[1] = tmp;
	sPTL_104_I_UP_ZXSJ.KSSJ[2] = RTCtimedate.minute;
	sPTL_104_I_UP_ZXSJ.KSSJ[3] = RTCtimedate.hour;
	sPTL_104_I_UP_ZXSJ.KSSJ[4] = RTCtimedate.day + (RTCtimedate.week<<5);
	sPTL_104_I_UP_ZXSJ.KSSJ[5] = RTCtimedate.month;
	sPTL_104_I_UP_ZXSJ.KSSJ[6] = RTCtimedate.year%2000;

	tmp = (RTCtimedate.second * 1000) % 256;
	sPTL_104_I_UP_LXSJ.KSSJ[0] = tmp;
	tmp = (RTCtimedate.second * 1000) / 256;
	sPTL_104_I_UP_LXSJ.KSSJ[1] = tmp;
	sPTL_104_I_UP_LXSJ.KSSJ[2] = RTCtimedate.minute;
	sPTL_104_I_UP_LXSJ.KSSJ[3] = RTCtimedate.hour;
	sPTL_104_I_UP_LXSJ.KSSJ[4] = RTCtimedate.day + (RTCtimedate.week<<5);
	sPTL_104_I_UP_LXSJ.KSSJ[5] = RTCtimedate.month;
	sPTL_104_I_UP_LXSJ.KSSJ[6] = RTCtimedate.year%2000;

//	memcpy(SysCtlWrod.chargebgntime,sPTL_104_I_UP_ZXSJ.KSSJ,7);
	
}
//������ʱ��
void SET_CHG_JSSJ(void )
{
	uint8_t tmp;

	tmp = (RTCtimedate.second * 1000) % 256;
	sPTL_104_I_UP_ZXSJ.JSSJ[0] = tmp;
	tmp = (RTCtimedate.second * 1000) / 256;
	sPTL_104_I_UP_ZXSJ.JSSJ[1] = tmp;
	sPTL_104_I_UP_ZXSJ.JSSJ[2] = RTCtimedate.minute;
	sPTL_104_I_UP_ZXSJ.JSSJ[3] = RTCtimedate.hour;
	sPTL_104_I_UP_ZXSJ.JSSJ[4] = RTCtimedate.day + (RTCtimedate.week<<5);
	sPTL_104_I_UP_ZXSJ.JSSJ[5] = RTCtimedate.month;
	sPTL_104_I_UP_ZXSJ.JSSJ[6] = RTCtimedate.year%2000;

	tmp = (RTCtimedate.second * 1000) % 256;
	sPTL_104_I_UP_LXSJ.JSSJ[0] = tmp;
	tmp = (RTCtimedate.second * 1000) / 256;
	sPTL_104_I_UP_LXSJ.JSSJ[1] = tmp;
	sPTL_104_I_UP_LXSJ.JSSJ[2] = RTCtimedate.minute;
	sPTL_104_I_UP_LXSJ.JSSJ[3] = RTCtimedate.hour;
	sPTL_104_I_UP_LXSJ.JSSJ[4] = RTCtimedate.day + (RTCtimedate.week<<5);
	sPTL_104_I_UP_LXSJ.JSSJ[5] = RTCtimedate.month;
	sPTL_104_I_UP_LXSJ.JSSJ[6] = RTCtimedate.year%2000;
}

//�������߽�����ˮ��
void SaveZXJYLSH(void)
{
	OS_ENTER_CRITICAL();
	FRAMWrite((uint8_t *)&JYSN,ZXJYLSH_ADD,sizeof(JYSN));
	OS_EXIT_CRITICAL();
}

extern uint8_t hex2bcd(uint8_t velue);
//��������ֹͣ����
void SET_ZXTZSJ(void)
{
	uint32_t tmp=0;
	
	memset(&sPTL_104_I_UP_ZXSJ.JYLSH,0,sizeof(sPTL_104_I_UP_ZXSJ.JYLSH));
	sPTL_104_I_UP_ZXSJ.YWLX = 1;		
	memcpy(sPTL_104_I_UP_ZXSJ.ZDJQBM,Term_Para1.ChargeNo,8);	//�ն˻������루���׮��ţ�

	
	//ˢ�����ļ�Ȩ
    if(JYSN++>=9999)
        JYSN=0;

    SaveZXJYLSH();
//    u32tobcdnomstr(JYSN,(uint8_t *)&sPTL_104_I_UP_ZXSJ.JYLSH[0],4);
    tmp+=4;

    sPTL_104_I_UP_ZXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.year%2000);
    sPTL_104_I_UP_ZXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.month);
    sPTL_104_I_UP_ZXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.day);
    sPTL_104_I_UP_ZXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.hour);

    memcpy(&sPTL_104_I_UP_ZXSJ.JYLSH[tmp],sPTL_104_I_UP_ZXSJ.ZDJQBM,sizeof(sPTL_104_I_UP_ZXSJ.ZDJQBM));

//	bcdone2bcdtwolo(SysCtlWrod.card1no,sPTL_104_I_UP_ZXSJ.LJKH,CDNLEN);

	sPTL_104_I_UP_ZXSJ.JLSSLX = 2;
}
//��������ֹͣ����
void SET_LXTZSJ(CHGCARD_PARA * pChgcard)
{
//	uint32_t tmp=0;

//	memcpy(sPTL_104_I_UP_LXSJ.ZDJQBM,Term_Para1.ChargeNo,8);	//�ն˻������루���׮��ţ�
//	memset(&sPTL_104_I_UP_LXSJ.JYLSH,0,sizeof(sPTL_104_I_UP_LXSJ.JYLSH));
//	
//	memcpy(sPTL_104_I_UP_LXSJ.YHBH,Term_Para1.ChargeNo,8); 		//�û���ţ�ͬ���׮��ţ�
//	bcdone2bcdtwolo(SysCtlWrod.card1no,sPTL_104_I_UP_LXSJ.LJKH,CDNLEN);

//		//ˢ�����ļ�Ȩ
//	if(SysCtlWrod.chargeByCardOrApp == 1)
//	{
////		if(JYSN++>=9999)
////			JYSN=0;
//		
//		if(isOnlinePayBack){
//			isOnlinePayBack = 0;
//			if(JYSN>=9999)
//				JYSN=0;
//		}else{
//		if(JYSN++>=9999)
//			JYSN=0;
//		}
//		
//		u32tobcdnomstr(JYSN,(uint8_t *)&sPTL_104_I_UP_LXSJ.JYLSH[0],4);
//		tmp+=4;

//		sPTL_104_I_UP_LXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.year%2000);
//		sPTL_104_I_UP_LXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.month);
//		sPTL_104_I_UP_LXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.day);
//		sPTL_104_I_UP_LXSJ.JYLSH[tmp++]=hex2bcd(RTCtimedate.hour);

//		memcpy(&sPTL_104_I_UP_LXSJ.JYLSH[tmp],sPTL_104_I_UP_LXSJ.ZDJQBM,sizeof(sPTL_104_I_UP_LXSJ.ZDJQBM));
//		}
//	else if(SysCtlWrod.chargeByCardOrApp == 2){//΢�š�APPɨ���磬
//		memcpy(&sPTL_104_I_UP_LXSJ.JYLSH,&SysCtlWrod.OrderNo,16);
//	}
//	
//	bcdone2bcdtwolo(SysCtlWrod.card1no,sPTL_104_I_UP_LXSJ.LJKH,CDNLEN);

//	sPTL_104_I_UP_LXSJ.JLLX = 2;  	//�������� 2�������
//	sPTL_104_I_UP_LXSJ.YWLX = 1;	//ҵ������ 1�����
//	sPTL_104_I_UP_LXSJ.XFSZ = sPTL_104_I_UP_LXSJ.ZDL; 	//�������� 2�������
}
//��Ȩ֡�������
void F104_JQZ_PACK(sPtl_104_i * pPtl104){
//	uint8_t len;
//	uint8_t sBuf[BUFSIZE];
//	pPtl104->Type = 0x82;
//	pPtl104->Lmtd = 0x01;
//	pPtl104->Rson = 0x03;
//	pPtl104->Apdu = 0x00;
//	SET_JQSJ(&Chgcard[0]);
//	len = F104_JQZSJ_PACK(pPtl104);
//	pPtl104->Flen = 10+len;
//	len = F104_INF_PACK(sBuf,pPtl104);
//	uart3SendStr(sBuf,len);
//	pPtl104->Sdsn += 2;
}
//��������֡�������
void F104_ZXSJ_PACK_UP(sPtl_104_i * pPtl104)
{
//	uint8_t len;
//	uint8_t sBuf[BUFSIZE];
//	pPtl104->Type = 0x82;
//	pPtl104->Lmtd = 0x01;
//	pPtl104->Rson = 0x03;
//	pPtl104->Apdu = 0x00;	
//	SET_ZXTZSJ();
//	len = F104_ZXSJSJ_PACK(pPtl104);
//	pPtl104->Flen = 10+len;
//	len = F104_INF_PACK(sBuf,pPtl104);
//	uart3SendStr(sBuf,len);
//	pPtl104->Sdsn += 2;	
}
//��������֡�������
void F104_ZXSJ_PACK(sPtl_104_i * pPtl104)
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
	uart3SendStr(sBuf,len);
	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;	
	isOnlinePayBack = 1;
}
//��������֡�������
void F104_LXSJ_PACK(sPtl_104_i * pPtl104){
	uint8_t len;
	uint8_t sBuf[BUFSIZE];
	pPtl104->Type = 0x82;
	pPtl104->Lmtd = 0x01;
	pPtl104->Rson = 0x03;
	pPtl104->Apdu = 0x00;	
	len = F104_LXSJSJ_PACK(pPtl104);
	pPtl104->Flen = 10+len;
	len = F104_INF_PACK(sBuf,pPtl104);
	uart3SendStr(sBuf,len);
	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;
}
//��½֡���
uint8_t F104_LOGIN_PACK(uint8_t *sBuf,uint8_t *pNo,uint16_t add)
{
	uint8_t	len;
	uint8_t	*psBuf;
	psBuf = sBuf;
	*psBuf++ = 0x68;
	*psBuf++ = 0x01;
	*psBuf++ = 0x02;	
	memcpy(psBuf,pNo,8);
	psBuf+=8;
	memset(psBuf,0,8);
	psBuf+=8;
	*psBuf++ = add%256;
	*psBuf++ = add/256;
	
	///////////////////////////////////////////////////////////
	//����汾�ţ�Ӧ����Ҫ�󣬵�¼֡�������4���ֽڵ�����汾�ţ�ʱ���ʽ��
	memcpy(psBuf,softVer,4);
	psBuf += 4;
	///////////////////////////////////////////////////////////
	
	len = psBuf - sBuf;
	return len;
}


extern uint8_t     strategy_kwh[24*2];     //�ƷѲ����еĵ���

//���üƷѲ����ϴ�����
void SET_JFCLSJ(CHASTRATEGY_UP_PARA * strategy_up)
{
	uint8_t authet_back_type,i;
	uint8_t charge_strategy[96] = {0};
	
	FRAMRead((uint8_t *)&authet_back_type, ORDER_ID_FLAG_ADD , sizeof(authet_back_type));
	if (authet_back_type)
	{
		FRAMRead(strategy_up->order_num, ORDER_ID_ADD , 16);
	}
	else
	{
		//������ˮ��
		memcpy(strategy_up->order_num,&sPTL_104_I_DW_JQSJ.OrderNo, 16);				
	}	

	strategy_up->recode_num = RecodeNum;
	memcpy(strategy_up->MD5, MD5, 16);
	memcpy(charge_strategy, Strategy, 96);
	//������ʱ�εĵ���
	for(i = 0; i < 24; i++)
	{
		memcpy(&strategy_up->strategy_kwh[i*6], &charge_strategy[i*4], 4);  //�Ʒ�ʱ�� 4 bytes
		strategy_up->strategy_kwh[i*6+4] = strategy_kwh[i*2+1];
		strategy_up->strategy_kwh[i*6+5] = strategy_kwh[i*2];
	}
}

//����֡���
uint8_t F104_ACTIVE_PACK(uint8_t *sBuf)
{
	uint8_t	len;
	uint8_t	*psBuf;
	psBuf = sBuf;
	*psBuf++ = 0x68;
	*psBuf++ = 0x04;
	*psBuf++ = 0x0B;	
	*psBuf++ = 0x00;
	*psBuf++ = 0x00;
	*psBuf++ = 0x00;
	len = psBuf - sBuf;
	return len;
}
//����֡���
uint8_t F104_TEST_PACK(uint8_t *sBuf)
{
	uint8_t	len;
	uint8_t	*psBuf;
	psBuf = sBuf;
	*psBuf++ = 0x68;
	*psBuf++ = 0x04;
	*psBuf++ = 0x43;	
	*psBuf++ = 0x00;
	*psBuf++ = 0x00;
	*psBuf++ = 0x00;
	len = psBuf - sBuf;
	return len;
}
//ֹͣ֡���
uint8_t F104_STOP_PACK(uint8_t *sBuf)
{
	uint8_t	len;
	uint8_t	*psBuf;
	psBuf = sBuf;
	*psBuf++ = 0x68;
	*psBuf++ = 0x04;
	*psBuf++ = 0x13;	
	*psBuf++ = 0x00;
	*psBuf++ = 0x00;
	*psBuf++ = 0x00;
	len = psBuf - sBuf;
	return len;
}
//ȷ��֡���
uint8_t F104_ACK_PACK(uint8_t *sBuf,uint16_t rSn)
{
	uint8_t	len;
	uint8_t	*psBuf;
	psBuf = sBuf;
	*psBuf++ = 0x68;
	*psBuf++ = 0x04;
	*psBuf++ = 0x01;	
	*psBuf++ = 0x00;
	*psBuf++ = (rSn)%256;
	*psBuf++ = (rSn)/256;
	len = psBuf - sBuf;
	return len;
}

//�ƷѲ��Գ������ϴ��������
void F104_JFCLCDQKSC_PACK(sPtl_104_i * pPtl104){
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
	uart3SendStr(sBuf,len);
//	F104_ACK_SAVE(sBuf,len);
	pPtl104->Sdsn += 2;	
}

//��Ȩ�������ݽ���
void PTL104_JQXX_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;	
	sPTL_104_I_DW_JQSJ.JQCGBZ = 5;
//	SysCtlWrod.chargeByCardOrApp = 1;								//ˢ�����
//	memset(&SysCtlWrod.OrderNo,0,16);
	
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.ZDJQBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.LJKH,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.DDQCWYBS,&pPtl104->Dobj[i],32);
	i+=32;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.JFMXBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.KKHKYE,&pPtl104->Dobj[i],4);
	i+=4;
	sPTL_104_I_DW_JQSJ.JQCGBZ = pPtl104->Dobj[i++];
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.JQSBYY,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYCS,&pPtl104->Dobj[i],4);
	i+=4;
}

//��Ȩ�������ݽ���
//�ֻ�APP,΢��ɨ��󣬷�������Ȩ����
//Ӧ�����º�̨Ҫ��APP��΢��ɨ���Ȩ��������Ҫ��16λ�Ľ�����ˮ�ţ�������ˮ����Ҫ���ϴ�����¼��ʱ�����ȥ
void PTL104_JQXXWithOrder_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;	
	sPTL_104_I_DW_JQSJ.JQCGBZ = 5;
//	SysCtlWrod.chargeByCardOrApp = 2;	
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.ZDJQBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.LJKH,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.DDQCWYBS,&pPtl104->Dobj[i],32);
	i+=32;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.JFMXBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.KKHKYE,&pPtl104->Dobj[i],4);
	i+=4;
	sPTL_104_I_DW_JQSJ.JQCGBZ = pPtl104->Dobj[i++];
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.JQSBYY,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYCS,&pPtl104->Dobj[i],4);
	i+=4;
	
	//APPɨ��ʱ�������µĶ�����ţ��˶��������Ҫ�����������Է��ϵ�����ϵ磬
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.OrderNo,&pPtl104->Dobj[i],16);
	i+=16;
}
//��Ȩ�������ݽ���
//�ֻ�APP,΢��ɨ��󣬷�������Ȩ����
//Ӧ�����º�̨Ҫ��APP��΢��ɨ���Ȩ��������Ҫ��16λ�Ľ�����ˮ�ţ�������ˮ����Ҫ���ϴ�����¼��ʱ�����ȥ
void PTL104_JQXXWithOrderAndLogicNumber_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;	
	sPTL_104_I_DW_JQSJ.JQCGBZ = 5;
//	SysCtlWrod.chargeByCardOrApp = 2;	
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.ZDJQBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.LJKH,&pPtl104->Dobj[i],8);
	i+=8;
	
	///////////////////////////////////////////////////////////////////////
	///Ӧ�������󣬼�Ȩ���е������������16���ֽڵ��߼�����
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.LOGIC_NUM,&pPtl104->Dobj[i],16);
	memcpy((uint8_t *)&sPTL_104_I_UP_ZXSJ.LOGIC_NUM,&pPtl104->Dobj[i],16);
	i+=16;
	/////////////////////////////////////////////////////////////////////////
	
	
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.DDQCWYBS,&pPtl104->Dobj[i],32);
	i+=32;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.JFMXBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.KKHKYE,&pPtl104->Dobj[i],4);
	i+=4;
	sPTL_104_I_DW_JQSJ.JQCGBZ = pPtl104->Dobj[i++];
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.JQSBYY,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.SYCS,&pPtl104->Dobj[i],4);
	i+=4;
	
	//APPɨ��ʱ�������µĶ�����ţ��˶��������Ҫ�����������Է��ϵ�����ϵ磬
	memcpy((uint8_t *)&sPTL_104_I_DW_JQSJ.OrderNo,&pPtl104->Dobj[i],16);
//	memcpy((uint8_t *)&SysCtlWrod.OrderNo,&sPTL_104_I_DW_JQSJ.OrderNo,16);
	i+=16;
}


//�ۿ��������ݽ���
void PTL104_KKXX_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;	
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.ZDJQBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.LJKH,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KKJE,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.ZHYE,&pPtl104->Dobj[i],4);
	i+=4;
	sPTL_104_I_DW_KKXX.KKCGBZ= pPtl104->Dobj[i++];
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KKSBYY,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KCLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.SYLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KCDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.SYDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KCCS,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.SYCS,&pPtl104->Dobj[i],4);
	i+=4;
}

//���߼����ŵĿۿ���������
void PTL104_KKXX_With_LogicNumber_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;	
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.ZDJQBM,&pPtl104->Dobj[i],8);
	i+=8;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.LJKH,&pPtl104->Dobj[i],8);
	i+=8;
	
	///////////////////////////////////////////////////////////////////////
	//Ӧ������̨Ҫ�󣬿ۿ��������ݼ���16���ֽ��߼�����
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.LOGIC_NUM,&pPtl104->Dobj[i],16);
	i+=16;
	
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KKJE,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.ZHYE,&pPtl104->Dobj[i],4);
	i+=4;
	sPTL_104_I_DW_KKXX.KKCGBZ= pPtl104->Dobj[i++];
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KKSBYY,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KCLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.SYLC,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KCDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.SYDL,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.KCCS,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_KKXX.SYCS,&pPtl104->Dobj[i],4);
	i+=4;
}

//���������ݽ���
void PTL104_HMD_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_HMD.LJKH,&pPtl104->Dobj[i],8);
	i+=8;
	sPTL_104_I_DW_HMD.ZT = pPtl104->Dobj[i++];
	memcpy((uint8_t *)&sPTL_104_I_DW_HMD.ZHGXSJ,&pPtl104->Dobj[i],7);
	i+=7;
}	
//��׼�������ݽ���
void PTL104_BZFL_unpack(sPtl_104_i * pPtl104)
{
	uint8_t i=0;	
	i++;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.QYSJ,&pPtl104->Dobj[i],7);
	i+=7;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.SXSJ,&pPtl104->Dobj[i],7);
	i+=7;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.ZXZT,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.JLLX,&pPtl104->Dobj[i],2);
	i+=2;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.JDJ,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.FDJ,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.PDJ,&pPtl104->Dobj[i],4);
	i+=4;
	memcpy((uint8_t *)&sPTL_104_I_DW_BZFL.GDJ,&pPtl104->Dobj[i],4);
	i+=4;
}

//����ϵͳʱ��
void SET_SYS_TIME(void )
{  	
//	BakTimeDate.second = (sPTL_104_I_RCV.Dobj[1]+sPTL_104_I_RCV.Dobj[2]*256)/1000;
//    BakTimeDate.minute 	= sPTL_104_I_RCV.Dobj[3];
//	BakTimeDate.hour	= sPTL_104_I_RCV.Dobj[4];
//	BakTimeDate.day		= sPTL_104_I_RCV.Dobj[5]&0x1f;
//	BakTimeDate.week	= sPTL_104_I_RCV.Dobj[5]>>5;
//	BakTimeDate.month	= sPTL_104_I_RCV.Dobj[6]&0x0f;
//	BakTimeDate.year	= sPTL_104_I_RCV.Dobj[7]+2000;

//	rtcInit(&BakTimeDate);		//�����ڲ�RTC
//	ExRTCSet(&BakTimeDate);		//�����ⲿRTC
}

/*********************************************************************************************************
  END FILE
**********************************************************************************************************/
