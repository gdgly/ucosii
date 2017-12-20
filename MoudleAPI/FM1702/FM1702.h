/****************************************************************************
* File :  fm1702.h                                                            *
* COPYRIGHT BY HUOYAN LTD.COMPANY                                           *
* Version:      V1.3                                      			        *
*                                                                           *
* Created:      18.10.2005                                                  *
* Last Change:  21.10.2005                                                  *
*                                                                           *
* Author:       mark                                                         *
*                                                                           *
* Compiler:     KEIL C51 V7.10                                              *
*                                                                           *
* Description:  AT89S52-Firmware for FM1702 Demo Serial Reader              *
*                                                                           *
****************************************************************************/
#ifndef _FM1702_H_
#define _FM1702_H_

#include "AllHand.h"
#include "lcd.h"

#ifdef IN_FM1702_GLOBALS
#define  IN_FM1702_EXT
#else
#define  IN_FM1702_EXT  extern
#endif

#define 	SSP1CSRF		(1 << 16)	              			//p0.16                
#define 	SET_SSPCSRF()	LPC_GPIO0->DIR |= SSP1CSRF;LPC_GPIO0->SET |= SSP1CSRF	                            
#define 	CLR_SSPCSRF()	LPC_GPIO0->DIR |= SSP1CSRF;LPC_GPIO0->CLR |= SSP1CSRF	                            
                            
#define 	SSP0CSFR		(1 << 21)	              			//p1.21               
#define 	SET_SSPCSFR()	LPC_GPIO1->DIR |= SSP0CSFR;LPC_GPIO1->SET |= SSP0CSFR	                            
#define 	CLR_SSPCSFR()	LPC_GPIO1->DIR |= SSP0CSFR;LPC_GPIO1->CLR |= SSP0CSFR	

#define 	FMRST			(1<<24)								//p1.24
#define		SET_FMRST()		LPC_GPIO1->DIR |= FMRST;LPC_GPIO1->SET |= FMRST
#define		CLR_FMRST()		LPC_GPIO1->DIR |= FMRST;LPC_GPIO1->CLR |= FMRST

#define		FMIRQ			(1<<23)							//p1.23
#define 	FMIRQ_STS()		(!(LPC_GPIO1->PIN & FMIRQ))									 								 
#define 	FMIRQ_INI()		(LPC_GPIO1->DIR &= ~FMIRQ)  

#define     STA_OFFLINE		0
#define     STA_FAULT		1
#define     STA_STANDBY		2
#define     STA_WORK		3
#define     STA_WAIT		4

IN_FM1702_EXT	uint8_t	CHARING_STA  ;
//const uint8_t PSW_ZONE  =	0x80   ; 
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

extern void SET_LXTZSJ(CHGCARD_PARA *pChgcard);
extern void SET_JQSJ(CHGCARD_PARA * pChgcard);
extern void SET_ZXTZSJ(void);


IN_FM1702_EXT  CHGCARD_PARA	Chgcard[2]; 

//IN_FM1702_EXT OS_EVENT	*FRAMSem;

IN_FM1702_EXT  uint8_t FMRevBuffer[255]  ; 
IN_FM1702_EXT  uint8_t tagtype[255]  ; 
IN_FM1702_EXT  uint8_t cardtype  ;
IN_FM1702_EXT  uint8_t UID[4] ; 
IN_FM1702_EXT  uint8_t UID_BAK[5] ; 
IN_FM1702_EXT  uint8_t Secnr ; 

IN_FM1702_EXT  uint8_t DispCardNo[10];			//���ŵ�ASCII ��


#define HEX_0x80	0x80 
#define HEX_0x00	0x00 

/* FM1702������ */
#define Transceive	0x1E			/* ���ͽ������� */
#define Transmit	0x1a			/* �������� */
#define ReadE2		0x03			/* ��FM1702 EEPROM���� */
#define WriteE2		0x01			/* дFM1702 EEPROM���� */
#define Authent1	0x0c			/* ��֤������֤���̵�1�� */
#define Authent2	0x14			/* ��֤������֤���̵�2�� */
#define LoadKeyE2	0x0b			/* ����Կ��EEPROM���Ƶ�KEY���� */
#define LoadKey		0x19			/* ����Կ��FIFO���渴�Ƶ�KEY���� */
#define RF_TimeOut	0x3f			/* ����������ʱʱ�� */
#define Req			0x01
#define Sel			0x02

/* ��Ƭ���Ͷ��嶨�� */
#define TYPEA_MODE		0			/* TypeAģʽ */
#define TYPEB_MODE		1			/* TypeAģʽ */
#define SHANGHAI_MODE	2			/* �Ϻ�ģʽ */

#define TM0_HIGH	0xf0			/* ��ʱ��0��λ,4MS��ʱ */
#define TM0_LOW		0x60			/* ��ʱ��0��λ */
#define TIMEOUT		100			/* ��ʱ������4MS��100=0.4�� */
#define BLOCKADDR	17
/* ��Ƶ��ͨ�������붨�� */
#define RF_CMD_REQUEST_STD	0x26
#define RF_CMD_REQUEST_ALL	0x52
#define RF_CMD_ANTICOL		0x93
#define RF_CMD_SELECT		0x93
#define RF_CMD_AUTH_LA		0x60
#define RF_CMD_AUTH_LB		0x61
#define RF_CMD_READ		    0x30
#define RF_CMD_WRITE		0xa0
#define RF_CMD_INC		    0xc1
#define RF_CMD_DEC		    0xc0
#define RF_CMD_RESTORE		0xc2
#define RF_CMD_TRANSFER		0xb0
#define RF_CMD_HALT		    0x50



/* ����������붨�� ERR CODE  */
#define FM1702_OK			0		/* ��ȷ */
#define FM1702_NOTAGERR		1		/* �޿� */
#define FM1702_CRCERR		2		/* ��ƬCRCУ����� */
#define FM1702_EMPTY		3		/* ��ֵ������� */
#define FM1702_AUTHERR		4		/* ��֤���ɹ� */
#define FM1702_PARITYERR	5		/* ��Ƭ��żУ����� */
#define FM1702_CODEERR		6		/* ͨѶ����(BCCУ���) */
#define FM1702_SERNRERR		8		/* ��Ƭ���кŴ���(anti-collision ����) */
#define FM1702_SELECTERR	9		/* ��Ƭ���ݳ����ֽڴ���(SELECT����) */
#define FM1702_NOTAUTHERR	10		/* ��Ƭû��ͨ����֤ */
#define FM1702_BITCOUNTERR	11		/* �ӿ�Ƭ���յ���λ������ */
#define FM1702_BYTECOUNTERR	12		/* �ӿ�Ƭ���յ����ֽ����������������Ч */
#define FM1702_RESTERR		13		/* ����restore�������� */
#define FM1702_TRANSERR		14		/* ����transfer�������� */
#define FM1702_WRITEERR		15		/* ����write�������� */
#define FM1702_INCRERR		16		/* ����increment�������� */
#define FM1702_DECRERR		17		/* ����decrement�������� */
#define FM1702_READERR		18		/* ����read�������� */
#define FM1702_LOADKEYERR	19		/* ����LOADKEY�������� */
#define FM1702_FRAMINGERR	20		/* FM1702֡���� */
#define FM1702_REQERR		21		/* ����req�������� */
#define FM1702_SELERR		22		/* ����sel�������� */
#define FM1702_ANTICOLLERR	23		/* ����anticoll�������� */
#define FM1702_INTIVALERR	24		/* ���ó�ʼ���������� */
#define FM1702_READVALERR	25		/* ���ø߼�����ֵ�������� */
#define FM1702_DESELECTERR	26
#define FM1702_CMD_ERR		42		/* ������� */

/* FM1702�Ĵ������� */
#define FM_Page_Sel			0x00	/* ҳд�Ĵ��� */
#define FM_Command			0x01	/* ����Ĵ��� */
#define FM_FIFO				0x02	/* 64�ֽ�FIFO�������������Ĵ��� */
#define FM_PrimaryStatus	0x03	/* ��������������FIFO��״̬�Ĵ���1 */
#define FM_FIFO_Length		0x04	/* ��ǰFIFO���ֽ����Ĵ��� */
#define FM_SecondaryStatus	0x05	/* ����״̬�Ĵ���2 */
#define FM_InterruptEn		0x06	/* �ж�ʹ��/��ֹ�Ĵ��� */
#define FM_Int_Req			0x07	/* �ж������ʶ�Ĵ��� */
#define FM_Control			0x09	/* ���ƼĴ��� */
#define FM_ErrorFlag		0x0A	/* ����״̬�Ĵ��� */
#define FM_CollPos			0x0B	/* ��ͻ���Ĵ��� */
#define FM_TimerValue		0x0c	/* ��ʱ����ǰֵ */
#define FM_Bit_Frame		0x0F	/* λ֡�����Ĵ��� */
#define FM_TxControl		0x11	/* ���Ϳ��ƼĴ��� */
#define FM_CWConductance	0x12	/* ѡ�����TX1��TX2�������ߵ��迹 */
#define FM_ModConductance	0x13	/* ������������迹 */
#define FM_CoderControl		0x14	/* �������ģʽ��ʱ��Ƶ�� */
#define FM_DecoderControl	0x1a	/* ������ƼĴ��� */
#define FM_RxControl1		0x19	/*�����������*/
#define FM_RxControl2		0x1e	/* ������Ƽ�ѡ�����Դ */
#define FM_RxWait			0x21	/* ѡ����ͽ���֮���ʱ���� */
#define FM_ChannelRedundancy 0x22	/* RFͨ������ģʽ���üĴ��� */
#define FM_CRCPresetLSB		0x23
#define FM_CRCPresetMSB		0x24
#define FM_MFOUTSelect		0x26	/* mf OUT ѡ�����üĴ��� */
#define FM_TimerClock		0x2a	/* ��ʱ���������üĴ��� */
#define FM_TimerControl		0x2b	/* ��ʱ�����ƼĴ��� */
#define FM_TimerReload		0x2c	/* ��ʱ����ֵ�Ĵ��� */
#define FM_TypeSH			0x31	/* �Ϻ���׼ѡ��Ĵ��� */
#define FM_TestDigiSelect	0x3d	/* ���Թܽ����üĴ��� */

/* Status Values */
#define ALL		0x01
#define KEYB	0x04
#define KEYA	0x00
#define _AB		0x40
#define CRC_A	1
#define CRC_B	2
#define CRC_OK	0
#define CRC_ERR 1
#define BCC_OK	0
#define BCC_ERR 1

/* �����Ͷ��� */
#define MIFARE_8K		0			/* MIFAREϵ��8KB��Ƭ */
#define MIFARE_TOKEN	1			/* MIFAREϵ��1KB TOKEN��Ƭ */
#define SHANGHAI_8K		2			/* �Ϻ���׼ϵ��8KB��Ƭ */
#define SHANGHAI_TOKEN	3			/* �Ϻ���׼ϵ��1KB TOKEN��Ƭ */

#define mifare1			1
#define mifarepro		2
#define mifarelight		3
#define unknowncard     4
///*********************************************************************************************************
//** Function name:		SSP0_Init
//** Descriptions:		��SSP����������Ϊ����
//** input parameters:	��
//** output parameters:	��
//**********************************************************************************************************/
//extern void  SSP0_Init(void)	; 

/**************************************************************************************************
**	��������:	ddelay()
**	��	��	:	FRAM��д��ʱ����
**	��ڲ���:	dely
**	���ڲ���:	��
**  ����ֵ  ��  ��
**	ȫ�ֱ���:	��
**	����ģ��:	��
**	��	��	:	��Сӱ
**	��	��	:	2009��03��06��
**************************************************************************************************/
 extern void ddelay(INT16U dely);

/**************************************************************************************************
**	��������:	FMSendByte()
**	��	��	:	��FRAM����һ�ֽ�����
**	��ڲ���:	dat				:	��Ҫ���͵�����
**	���ڲ���:	��
**  ����ֵ  ��  ��
**	ȫ�ֱ���:	��
**	����ģ��:	ddelay();
**	��	��	:	��Сӱ
**	��	��	:	2012��04��09��
**************************************************************************************************/
extern void  FMSendByte(uint8_t data) ;

/**************************************************************************************************
**	��������:	FMReadByte()
**	��	��	:	��FRAM����һ�ֽ�����
**	��ڲ���:	��
**	���ڲ���:	��
**  ����ֵ  ��  data			:	����������
**	ȫ�ֱ���:	��
**	����ģ��:	ddelay();
**	��	��	:	��Сӱ
**	��	��	:	2012��04��09��
**************************************************************************************************/
extern INT16U  FMReadByte(void ) ;

/**************************************************************************************************
**	��������:	FMWrite()
**	��	��	:	��FRAMд����
**	��ڲ���:	*dataptr	:	ָ����Ҫ���͵����ݵ�ָ��
**				addr		:	FRAM����ʼ�洢��ַ
**				length		:	���ݳ���
**	���ڲ���:	��
**  ����ֵ  ��  ��
**	ȫ�ֱ���:	��
**	����ģ��:	FramSendByte��FramState
**	��	��	:	��Сӱ
**	��	��	:	2012��04��09��
**************************************************************************************************/
extern void FMWrite(uint8_t dataptr,uint8_t addr);

/**************************************************************************************************
**	��������:	FRAMRead()
**	��	��	:	��FRAM
**	��ڲ���:	*dataptr	:	ָ����Ҫ��ŵ����ݵ�ָ��
**				addr		:	FRAM����ʼ�洢��ַ
**				length		:	���ݳ���
**	���ڲ���:	��
**  ����ֵ  ��  ��
**	ȫ�ֱ���:	��
**	����ģ��:	FramSendByte��FramReadByte��FramState
**	��	��	:	��Сӱ
**	��	��	:	2012��04��09��
**************************************************************************************************/
extern void	FMRead(uint8_t *dataptr, uint8_t addr);

///////////////////////////////////////////////////////////////////////
// IC��������
///////////////////////////////////////////////////////////////////////
extern void cmd_process(void);

/****************************************************************/
/*����: Clear_FIFO */
/*����: �ú���ʵ�����FM1702��FIFO������*/
/*����: N/A */
/*���: TRUE, FIFO�����*/
/* FALSE, FIFOδ�����*/
/****************************************************************/
extern BOOLEAN FMClear_FIFO(void);

/****************************************************************/
/*����: Write_FIFO */
/*����: �ú���ʵ����FM1702��FIFO��д��x bytes����*/
/*����: count, ��д���ֽڵĳ���*/
/* buff, ָ���д�����ݵ�ָ��*/
/*���: N/A */
/****************************************************************/
extern void FMWrite_FIFO(uint8_t count, uint8_t *buff);
/****************************************************************/
/*����: Read_FIFO */
/*����: �ú���ʵ�ִ�FM1702��FIFO�ж���x bytes����*/
/*����: buff, ָ��������ݵ�ָ��*/
/*���: N/A */
/****************************************************************/
extern uint8_t FMRead_FIFO(uint8_t *buff);

/****************************************************************/
/*����: Judge_Req */
/*����: �ú���ʵ�ֶԿ�Ƭ��λӦ���źŵ��ж�*/
/*����: *buff, ָ��Ӧ�����ݵ�ָ��*/
/*���: TRUE, ��ƬӦ���ź���ȷ*/
/* FALSE, ��ƬӦ���źŴ���*/
/****************************************************************/
extern BOOLEAN FMJudge_Req(uint8_t *buff);

/****************************************************************/
/*����: Check_UID */
/*����: �ú���ʵ�ֶ��յ��Ŀ�Ƭ�����кŵ��ж�*/
/*����: N/A */
/*���: TRUE: ���к���ȷ*/
/* FALSE: ���кŴ���*/
/****************************************************************/
extern BOOLEAN FMCheck_UID(void);

/****************************************************************/
/*����: Save_UID */
/*����: �ú���ʵ�ֱ��濨Ƭ�յ������к�*/
/*����: row: ������ͻ����*/
/* col: ������ͻ����*/
/* length: �Ӆ�����UID���ݳ���*/
/*���: N/A */
/****************************************************************/
extern void FMSave_UID(uint8_t row, uint8_t col, uint8_t length);

/****************************************************************/
/*����: Set_BitFraming */
/*����: �ú������ô��������ݵ��ֽ���*/
/*����: row: ������ͻ����*/
/* col: ������ͻ����*/
/*���: N/A */
/****************************************************************/
extern void FMSet_BitFraming(uint8_t row, uint8_t col);

/****************************************************************/
/*����: Init_FM1702 */
/*����: �ú���ʵ�ֶ�FM1702��ʼ������*/
/*����: mode:����ģʽ, 0:TYPEAģʽ*/
/* 1:TYPEBģʽ*/
/* 2:�Ϻ�ģʽ*/
/*���: N/A */
/****************************************************************/
extern void FMInit_FM1702(void );

/****************************************************************/
/*����: Command_Send */
/*����: �ú���ʵ����FM1702��������Ĺ���*/
/*����: count, ����������ĳ���*/
/* buff, ָ����������ݵ�ָ��*/
/* Comm_Set, ������*/
/*���: TRUE, �����ȷִ��*/
/* FALSE, ����ִ�д���*/
/****************************************************************/
extern BOOLEAN FMCommand_Send(uint8_t count, uint8_t *buff, uint8_t Comm_Set);

/****************************************************************/
/*����:  */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵĿ�Ƭ�Ĳ���*/
/*����: mode: ALL(�������FM1702������Χ֮�ڵĿ�Ƭ) */
/* STD(�����FM1702������Χ֮�ڴ���HALT״̬�Ŀ�Ƭ) */
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_REQERR: Ӧ�����*/
/****************************************************************/
extern uint8_t FMRequest(uint8_t mode);

/****************************************************************/
/*����: Read_E2 */
/*����: �ú���ʵ�ִ�FM1702��EE�ж�������*/
/*����: lsb, EE��ַ(���ֽ�) */
/* msb, EE��ַ(���ֽ�) */
/* count, ����������EE���ֽڸ���*/
/* buff, ָ����������ݵ�ָ��*/
/*���: TRUE, EE������ȷ����*/
/* FALSE, EE���ݶ�������*/
/****************************************************************/
extern BOOLEAN FMRead_E2(uint8_t lsb, uint8_t msb, uint8_t count, uint8_t *buff);


/****************************************************************/
/*����: Write_E2 */
/*����: �ú���ʵ����FM1702��EE��д������*/
/*����: lsb, EE��ַ(���ֽ�) */
/* msb, EE��ַ(���ֽ�) */
/* count, ��д������EE���ֽڸ���*/
/* buff, ָ���д�����ݵ�ָ��*/
/*���: TRUE, EE������ȷд��*/
/* FALSE, EE����д������*/
/****************************************************************/
extern BOOLEAN FMWrite_E2(uint8_t lsb, uint8_t msb, uint8_t count, uint8_t *buff);

/****************************************************************/
/*����: MIF_Halt */
/*����: �ú���ʵ����ͣMIFARE��*/
/*����: N/A */
/*���: FM1702_OK: Ӧ����ȷ*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_NOTAGERR: �޿�*/
/****************************************************************/
extern uint8_t FMMIF_Halt(void);


///////////////////////////////////////////////////////////////////////
// ת����Կ��ʽ
///////////////////////////////////////////////////////////////////////
extern uint8_t FMM500HostCodeKey(  uint8_t *uncoded, uint8_t *coded)   ;

/****************************************************************/
/*����: Load_keyE2 */
/*����: �ú���ʵ�ְ�E2���������FM1702��keyRevBuffer��*/
/*����: Secnr: EE��ʼ��ַ*/
/*���: True: ��Կװ�سɹ�*/
/* False: ��Կװ��ʧ��*/
/****************************************************************/
extern BOOLEAN FMLoad_keyE2_CPY(uint8_t Secnr, uint8_t Mode);


/****************************************************************/
/*����: AntiColl */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵĿ�Ƭ�ķ���ͻ���*/
/*����: N/A */
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_SERNRERR: ��Ƭ���к�Ӧ�����*/
/* FM1702_OK: ��ƬӦ����ȷ*/
/****************************************************************/
extern uint8_t FMAntiColl(void);
/****************************************************************/
/*����: Select_Card */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵ�ĳ�ſ�Ƭ����ѡ��*/
/*����: N/A */
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_SELERR: ѡ������*/
/****************************************************************/

extern uint8_t FMSelect_Card(void);


/****************************************************************/
/*����: Authentication */
/*����: �ú���ʵ��������֤�Ĺ���*/
/*����: UID: ��Ƭ���кŵ�ַ*/
/* SecNR: ������*/
/* mode: ģʽ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_AUTHERR: Ȩ����֤�д�*/
/****************************************************************/
extern uint8_t FMAuthentication(uint8_t *UID, uint8_t SecNR, uint8_t mode);

/****************************************************************/
/*����: MIF_Read */
/*����: �ú���ʵ�ֶ�MIFARE�������ֵ*/
/*����: buff: �������׵�ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_READ(uint8_t *buff, uint8_t Block_Adr);

/****************************************************************/
/*����: MIF_Write */
/*����: �ú���ʵ��дMIFARE�������ֵ*/
/*����: buff: �������׵�ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_WRITEERR: д�������ݳ���*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_Write(uint8_t *buff, uint8_t Block_Adr);

/****************************************************************/
/*����: MIF_Increment */
/*����: �ú���ʵ��MIFARE���Զ���ֵ����*/
/*����: buff: �ĸ��ֽ���ֵ��ʼ��ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_INCRERR: ��Ƭ�������ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_Increment(uint8_t *buff, uint8_t Block_Adr);

/****************************************************************/
/*����: MIF_Decrement */
/*����: �ú���ʵ��MIFARE���Զ���ֵ����*/
/*����: buff: �ĸ��ֽ���ֵ��ʼ��ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_DECRERR: ��Ƭ�ۿ����ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_Decrement(uint8_t *buff, uint8_t Block_Adr);

/****************************************************************/
/*����: MIF_Restore */
/*����: �ú���ʵ��MIFARE���Զ��ָ�,���ݲ���*/
/*����: Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_RESTERR: ��Ƭ�ָ�,���ݲ���ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_Restore(uint8_t Block_Adr);

/****************************************************************/
/*����: MIF_Transfer */
/*����: �ú���ʵ��MIFARE������Ǯ���������*/
/*����: Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_TRANSERR: ��Ƭ�ָ�,���ݲ���ʧ��*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_Transfer(uint8_t Block_Adr);


/****************************************************************/
/*����: HL_Active */
/*����: �ú���ʵ�ָ߼�MIFARE����������*/
/*����: Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���: ����״̬��*/
/* �������ݴ���RevBuffer��*/
/****************************************************************/
extern uint8_t FMHL_Active(uint8_t Block_Adr, uint8_t Mode);

/****************************************************************/
/*����: MIF_Initival */
/*����: �ú���ʵ��MIFARE����ʼ��ֵ����*/
/*����: buff: �ĸ��ֽڳ�ʼ����ֵ��ʼ��ַ*/
/* Block_Adr: ���ַ*/
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_NOTAUTHERR: δ��Ȩ����֤*/
/* FM1702_EMPTY: �����������*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_WRITEERR: д�������ݳ���*/
/* FM1702_OK: Ӧ����ȷ*/
/****************************************************************/
extern uint8_t FMMIF_Initival(uint8_t *buff, uint8_t Block_Adr);

/****************************************************************/
/*����: HL_Read */
/*����: �ú���ʵ�ָ߼�������*/
/*����: Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���: ����״̬��*/
/* �������ݴ���RevBuffer��*/
/****************************************************************/
extern uint8_t FMHL_Read(uint8_t *buff, uint8_t Block_Adr, uint8_t Mode);

/****************************************************************/
/*����: HL_Write */
/*����: �ú���ʵ�ָ߼�д����*/
/*����: buff: ��д�����ݵ��׵�ַ*/
/* Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���:����״̬��*/
/****************************************************************/
extern uint8_t FMHL_Write(uint8_t *buff, uint8_t Block_Adr, uint8_t Mode);

/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16������ת��ΪASCII��
* Input			: 
* Output		: 
* Note(s)		: ע�⣬ת����ascii���������ʮ���ֽ�
* Contributor	: 10/12/2010	wangyao
***********************************************************************************************/
//extern void  HextoAscii(uint32_t hexdata,uint8_t *data) ;
/**************************************************************************************************
**	��������:	CompareArray()
**	��	��	:	�Ƚ����������Ƿ�һ��
**	��ڲ���:	*pt1			:	ָ��Ƚϵ������ָ��
**				*pt2			:	ָ��Ƚϵ������ָ��
**				length			:	�Ƚϵ�����ĳ���
**	���ڲ���:	��
**  ����ֵ  ��  1:�������		0 ���鲻���
**	ȫ�ֱ���:	��
**	����ģ��:	��
**	��	��	:	��Сӱ
**	��	��	:	2009��04��14��
**************************************************************************************************/
//extern uint8_t	CompareArray(uint8_t *pt1, uint8_t *pt2, uint8_t length) ; 

/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16������ת��ΪASCII��
* Input			: 
* Output		: 
* Note(s)		: ע�⣬ת����ascii���������ʮ���ֽ�
* Contributor	: 10/12/2010	wangyao
***********************************************************************************************/
//extern void  HextoAscii(uint32_t hexdata,uint8_t *data) ; 

/**************************************************************************************************
**	��������:	CompareArray()
**	��	��	:	�Ƚ����������Ƿ�һ��
**	��ڲ���:	*pt1			:	ָ��Ƚϵ������ָ��
**				*pt2			:	ָ��Ƚϵ������ָ��
**				length			:	�Ƚϵ�����ĳ���
**	���ڲ���:	��
**  ����ֵ  ��  1:�������		0 ���鲻���
**	ȫ�ֱ���:	��
**	����ģ��:	��
**	��	��	:	��Сӱ
**	��	��	:	2009��04��14��
**************************************************************************************************/
extern uint8_t	CompareArray(uint8_t *pt1, uint8_t *pt2, uint8_t length) ; 

/***********************************************************************************************
* Function		: BSP_SaveNewKey
* Description	: ���������õ�����
* Input			: addr---�����ַ
                  KEY---��Կ
* Output		: 1--�ɹ�  0--���ɹ�
* Note(s)		: 
* Contributor	: 17/06/2010	wangyao
***********************************************************************************************/
extern uint8_t SavePassword(const uint8_t *KEY) ; 

/***********************************************************************************************
* Function		: FM1702SLHostCodeKey
* Description	: �����ʽ��ת��
* Input			: uncoded--û��ת��������16������
*                 coded--��ʽת���������
* Output		: 
* Note(s)		: ��Կ�����Թ涨�ĸ�ʽ�����EEPROM�ڣ�ÿһBYTE����Կ��Ϊ����λ�͸���λ��
ÿ��λ��λ���Լ�λ�����һ��byte�ڣ�LoadKeyE2��LoadKeyָ��ᰴ���������Կ�Ƿ��������ָ�ʽ��
ʹ�����ָ�ʽ��6byte�Ŀ���������12byteEEPROM�ڣ�����keyΪA0A1A2A3A4A5,����EEPROM�д�ŵ�
��ʽΪ5AF05AE15AD25AC35AB45AA5�������A0��5AF0��ʾ��Ҳ����A��5A��ʾ������5��A��ȡ����A�Ǳ���
* Contributor	: 17/06/2010	wangyao
***********************************************************************************************/
extern void FM1702SLHostCodeKey(const uint8_t *uncoded, uint8_t *coded)  ;  


#endif

//////////////////////////////////////////////////////////////////////////////
// End of File
//////////////////////////////////////////////////////////////////////////////
