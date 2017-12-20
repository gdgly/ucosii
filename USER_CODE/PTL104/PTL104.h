/****************************************Copyright (c)****************************************************
**                            ShangHai YUNWANG *** Co.,LTD.
**
**                                 http://www.***.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL104.h
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

#ifndef __PTL104_H_
#define __PTL104_H_

#include "AllHand.h"

#define 	FIFODEEP		6
#define 	BUFSIZE			255

//104��ԼI֡�ṹ��
typedef struct 
{
	unsigned char  Head;		//֡ͷ
	unsigned char  Flen;		//֡����=��֡��-2
	unsigned short Sdsn;		//�������
	unsigned short Rxsn;		//�������
	unsigned char  Type;		//���ͱ�ʶ
	unsigned char  Lmtd;		//�ɱ�ṹ�޶���
	unsigned short Rson;		//����ԭ��
	unsigned short Apdu;		//Ӧ�÷������ݵ�Ԫ
	unsigned char  Dobj[243];	//��Ϣ����1...N
}sPtl_104_i;

extern 	uint8_t F104_INF_PACK(uint8_t *sBuf,sPtl_104_i * pPtl104);
extern  uint8_t F104_RC_PACK(uint8_t *rcBuf,sPtl_104_i * rcPtl104);
extern  uint8_t F104_ZZHSJ_PACK(sPtl_104_i * pPtl104);


//��5-34������ʵʱ�������
typedef struct 
{
	unsigned short  CDSCDY;		//��������ѹ
	unsigned short 	CDSCDL;		//����������
	unsigned char   SCJDQZT;	//����̵���״̬
	unsigned char   LJQRKGZT;	//����ȷ�Ͽ���״̬
	unsigned int    YGZDD;		//�й��ܵ��
	unsigned char   CDZBH[8];	//���׮���
	unsigned short  SFLJDC;		//�Ƿ����ӵ��
	unsigned short  GZZT;		//����״̬
	
}sPtl_104_i_100;
//��5-34������ʵʱ�������
typedef struct 
{
	unsigned short  MS;		
	unsigned short 	MIN;		
	unsigned char   DAY;	
	unsigned char   WEEK;	
	unsigned char   MON;		
	unsigned char   YEAR;		
}sPtl_104_i_tim;
//ϵͳ�����ṹ��
typedef struct 
{
	unsigned char  Head;		//֡ͷ
	unsigned char  Flen;		//֡����=��֡��-2
	unsigned short Sdsn;		//�������
	unsigned short Rxsn;		//�������
	unsigned char  Type;		//���ͱ�ʶ
	unsigned char  Lmtd;		//�ɱ�ṹ�޶���
	unsigned short Rson;		//����ԭ��
	unsigned short Apdu;		//Ӧ�÷������ݵ�Ԫ
	unsigned char  Dobj[243];	//��Ϣ����1...N
	//82 01 03 00 00 00 01 01 00 00
}sSys_para;
//���ڷ��ͻ���
typedef struct
{
	unsigned int		len ; 				//֡����
	unsigned char		data[BUFSIZE] ;		//֡����

}UART_SEND_FIFO;
//���ڽ��ջ���
typedef struct
{
	unsigned int		len ; 				//֡����
	unsigned char		data[BUFSIZE] ;		//֡����

}UART_RECV_FIFO;

//��ȷ�����
typedef struct
{
	unsigned char		flag ; 				//��Ч��־
	unsigned short		Sidx;				//��ȷ�ϵ�����֡������ţ�+2��
	unsigned char		Fdat[255];			//��ȷ������֡
}sFor_Ack_Idx;							   	

//�����շ���ʱ
typedef struct 
{
	unsigned short  	AckTimS;			//��ȷ�ϳ�ʱ
	
	unsigned short  	SndDlyS;			//����ʱ����
	unsigned short  	SndTimS;			//�������ʱ	
	unsigned short 		RcvDlyS;			//����ʱ����	
	unsigned short  	RcvTimS;			//�������ʱ
	unsigned short  	GucHeartF1;         /* 	������ʱ			        */
	unsigned short		GucHeartT1;         /* 	��������			        */
	uint8_t					StopFlag;			//ֹͣ���ͱ�־
}sUart_DlyTim;

extern uint8_t sPTL_104_COMOUT_COUNT;

extern BALANCEDOWN_PARA	    sPTL_104_I_DW_KKXX;
extern CHGONLINE_PARA		sPTL_104_I_UP_ZXSJ;
extern CHGOFFLINE_PARA		sPTL_104_I_UP_LXSJ;
extern sPtl_104_i_100		sPTL_104_I_100;
extern sPtl_104_i_100		sPTL_104_I_100_BAK;
extern CHECKUP_PARA 		sPTL_104_I_UP_JQSJ;
extern CHECKDOWN_PARA		sPTL_104_I_DW_JQSJ;

extern sFor_Ack_Idx			ForAckIdx[FIFODEEP];
extern sPtl_104_i 			sPTL_104_I_SND;
extern sPtl_104_i 			sPTL_104_I_RCV;
extern sUart_DlyTim			UartDlyTim;
 
extern uint8_t Frame_Active_Packed(uint8_t *sBuf);
extern uint8_t Frame_Test_Packed(uint8_t *sBuf);
extern uint8_t Frame_Stop_Packed(uint8_t *sBuf);
extern uint8_t Frame_Ack_Packed(uint8_t *sBuf,uint16_t rSn);
extern uint8_t Frame_Inf_Packed(uint8_t *sBuf,sPtl_104_i * pPtl104);
extern uint8_t F104_ACK_STAT( void );
extern void F104_JQZ_PACK(sPtl_104_i * pPtl104);
extern void F104_ZXSJ_PACK(sPtl_104_i * pPtl104);
extern void F104_LXSJ_PACK(sPtl_104_i * pPtl104);

extern uint8_t Is_F104_UnACK(void);
extern void F104_LXSJ_Send(sPtl_104_i * pPtl104);
extern uint8_t F104_LOGIN_PACK(uint8_t *sBuf,uint8_t *pNo,uint16_t add);
extern void F104_JFCLCDQKSC_PACK(sPtl_104_i * pPtl104);
extern void SET_CHG_JSSJ(void );
extern void SET_CHG_KSSJ(void );

extern void PTL104_JQXX_unpack(sPtl_104_i * pPtl104);
extern void PTL104_JQXXWithOrder_unpack(sPtl_104_i * pPtl104);
extern void PTL104_JQXXWithOrderAndLogicNumber_unpack(sPtl_104_i * pPtl104);

extern void SET_JFCLSJ(CHASTRATEGY_UP_PARA * strategy_up);
extern uint8_t F104_JFCLCDQK_UP_PACK(sPtl_104_i *pPtl104);
extern void SET_ZXTZSJ(void);
extern uint8_t F104_ZXSJSJ_PACK(sPtl_104_i *pPtl104);
extern void PTL104_KKXX_With_LogicNumber_unpack(sPtl_104_i * pPtl104);
#endif


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
