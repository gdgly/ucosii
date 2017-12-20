/****************************************Copyright (c)****************************************************
**                            ShangHai FuDian Technology Co.,LTD.
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL698.h
** Latest modified Date:       2014-5-20
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 AndyLau
** Created date:               2014-5-20
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2014-5-20
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __PTL698_H
#define __PTL698_H

#define		gx_ofs_jzq_len		1
#define		gx_ofs_jzq_ctl		6
#define		gx_ofs_jzq_addr		7
#define		gx_ofs_jzq_afn		12
#define		gx_ofs_jzq_seq		13
#define		gx_ofs_jzq_dat		14

#define		gx_ofs_jzq_mes			18
#define		gx_ofs_jzq_wcode	   	21    //������
#define		gx_ofs_jzq_RETwcode	   	26    //��Ӧ������
#define		gx_ofs_jzq_ResultCode	27    //�����

#define		gx_ofs_ctlb_dir			7	
#define		gx_ofs_ctlb_prm			6
#define		gx_ofs_ctlb_fcb			5
#define		gx_ofs_ctlb_fcv			4

#define		gx_ofs_seqb_TpV			7	
#define		gx_ofs_seqb_FIR			6
#define		gx_ofs_seqb_FIN			5
#define		gx_ofs_seqb_CON			4

#define		AFN_OK_NOK			0x00		//ȷ�ϨM����
#define		AFN_RESET			0x01		//��λ
#define		AFN_ROAD_CHK		0x02		//��·�ӿڼ��
#define		AFN_ROAD_SET		0x04		//�ŵ����ò���
#define		AFN_PW_CFG			0x06		//�����֤����ԿЭ��
#define		AFN_ROAD_GET		0x0A		//��ѯ�ŵ����ò���
#define		AFN_FILE_SEND		0x0F		//�ļ�����
#define		AFN_DATA_SEND		0x10		//����ת��
#define		AFN_PRAR_SET		0x84		//���񼯳����ò���
#define		AFN_CTRL_CMD		0x85		//���񼯳���������
#define		AFN_PRAR_GET		0x8A		//���񼯳���ѯ����
#define		AFN_REAL_DAT		0x8C		//���񼯳�����1 �����ݣ�ʵʱ���ݣ�
#define		AFN_ROLD_DAT		0x8D		//���񼯳�����2 �����ݣ���ʷ���ݣ�
#define		AFN_RERR_DAT		0x8E		//���񼯳�����3 �����ݣ��¼����ݣ�



typedef struct 	GX_FRAME_TIME
{
		uint8_t		PFC;			//BCD 2BYTE ����������	
		uint8_t		TIM[4];			//BIN 2BYTE ��������ַ
		uint8_t		DLY;			//BIN 1BYTE��վ��ַ�����ַ��־	
}gx_frm_tim;

typedef struct 	GX_FRAME_DATA
{
		uint8_t		AFN;			//Ӧ�ò㹦����AFN
		uint8_t		SEQ;			//֡������SEQ
		uint16_t		DA;				//��Ϣ��DA ����Ϣ��ԪDA1 ����Ϣ����DA2 �����ֽڹ��ɡ�	
		uint16_t		DT;				//��Ϣ��DT ����Ϣ��ԪDT1 ����Ϣ����DT2 �����ֽڹ��ɡ�
		uint16_t		PW;
		uint16_t		EC;				//�¼�������
		gx_frm_tim	struct_frm_Tp;
}gx_frm_dat;

typedef struct 	GX_FRAME_ADDR
{
		uint16_t		A1;				//BCD 2BYTE ����������	
		uint16_t		A2;				//BIN 2BYTE ��������ַ
		uint8_t		A3;				//BIN 1BYTE��վ��ַ�����ַ��־	
}gx_frm_adr;

typedef struct 	GX_FRAME
{
		uint16_t		gx_dat_len;
		
		uint8_t		gx_frm_ctl;
		
		gx_frm_adr	gx_jzq_adr;

		gx_frm_dat	gx_jzq_dat;
		
}GX_DAT_FRM;

/* ʱ��ṹ�嶨�� */	
typedef struct time_type
{
	uint8_t  hour;    // Сʱ
	uint8_t  min;     // ����
	uint8_t  sec;     // ����
	uint8_t  dow;     // ����
}TIME;
/* ���ڽṹ�嶨�� */
typedef struct date_type
{
	uint8_t  mon;     // �·�
	uint8_t  day;     // ����
	uint16_t year;    // ���
}DATE;


//�����ýṹ��
typedef struct UpProgram2{
	
		uint8_t	ChuangShangNUM[2];  //���̱��
		uint8_t	ChuangShangNUM_Ext;  //������չ���
		uint8_t	WorkCode;    //������
		uint8_t	ComNUM[4];         //�Ự��
		uint8_t	Mima[3];
		uint8_t	UpVersion; 
		uint8_t	LimitTime;
		uint8_t   bak[9];    //����

		uint8_t   UpGX_Result_CODE;           //���������
		uint8_t   UpGX_Result_OKorNO;        //1/��ʾ��ȷӦ�𡡡���0��ʾ����
		char    FileRoad[64];		
		char    FileName[32];
		uint8_t   UpFileVersion;
 
		uint32_t  FileSize;  //�ļ���С                          //Ҫ��ʼ��
		uint8_t   LastFileVersion;                        //Ҫ��ʼ��
		uint8_t   LastUpFileSta[2];  //�ϴ�����״̬     //Ҫ��ʼ��
		uint8_t   LastUpMask[128];     
 
		uint16_t  LastTotalBlock; //���� /������
		uint16_t  k;
		uint8_t   CrC[3];
}Struct_UpProgram;




#endif
/*********************************************************************************************************
**											END OF FILE												   	**
**********************************************************************************************************/


