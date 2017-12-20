#ifndef	__LCD_H__
#define	__LCD_H__

#include "t6963.h"


#define	LNIC  	1					//��ʾ��λ
#define CNTM	1				   	//ʵʱʱ��
#define CNXH	11		 			//�ź�ǿ��
#define CNZX	12		   			//���߱�ʶ
#define CNCK	13			   		//���б�ʶ
#define CNTK	14	  				//������ʶ

#define LNSBH	15  


#define UPCDLEN		20				//�����������ų���
#define	UPJELEN		12				//��������������
#define	UPJEDOT		10				//�������������С����λ��

#define ASTPST		11 				//"*"λ��ƫ����

#define MNYLEN		6  				//����ʾ��ʮ����λ��
#define MNYDPS		4				//����ʾ��С����λ��
#define CDNLEN		16	   			//���ţ���ʾ��ʮ����λ��
#define PWDLEN		16				//���루��ʾ��ʮ����λ��
#define OUTLEN		1			   	//�˿ڣ���ʾ��ʮ����λ��
#define PWRLEN		8				//��������ʾ��ʮ����λ��
#define PWRDPS		6 				//��������ʾ��С����λ��
#define SUMLEN		MNYLEN			//�ܶ��ʾ��ʮ����λ��
#define SUMDPS		MNYDPS			//�ܶ��ʾ��С����λ��
#define AMTLEN		MNYLEN			//�ܼƣ���ʾ��ʮ����λ��
#define AMTDPS		MNYDPS			//�ܼƣ���ʾ��С����λ��
#define BALLEN		MNYLEN			//����ʾ��ʮ����λ��
#define BALDPS		MNYDPS			//����ʾ��С����λ��

#define NONDPS		0				//��С����
#define CRRLEN		4				//��ѹ����ʾ��ʮ����λ��
#define CRRDPS		3				//��ѹ����ʾ��С����λ��
#define VLLLEN		4				//��ѹ����ʾ��ʮ����λ��
#define VLLDPS		3				//��ѹ����ʾ��С����λ��
#define TIMLEN		2				//ʱ�䣨��ʾ��ʮ����λ��

#define LNJGNR	10
#define LNJG	6
#define CNJG	6
#define CN4B	5

#define CNTS	1
#define LNKH	4
#define CNDW	12

#define LNJEBL	8
#define LNYEBL	10

#define LNKH	4
#define LNDY	6
#define LNDL	8
#define LNDD	10
#define LNJE	12
#define LNYE	14

#define CNDY	9
#define CNDL	9
#define CNDD	7
#define CNJE	8
#define CNKH	4


#define LNXXBL	12

#define LNKHBL	4

#define QrCODE_X    15
#define QrCODE_Y    64

//��ʾ��������
void dspstandbypage(void);

//��ʾ��ά��
void dspQR_Code(uint8_t ppixel[][65],int Version);

//ʱ��ͼ��
void dspTimeAndIcon(uint8_t mode );

extern uint8_t DspJQFHerr(uint8_t errcode);
//��ʾ�������ǹ
extern void Display_GUN_Insert(void);

extern void DisplayChargeInfo(void);

extern void DisplayChargeEndInfo(void);

extern void DisplayChargeError(void);

extern void DisplayOpenButton(void);

//���ݷ�תȻ�� 2λbcd��ռ��һ���ֽ� �ֿ�1λbcd��ռ��һ���ֽ�
void bcdtwo2bcdone(unsigned char *sbuf,unsigned char *dbuf,unsigned int dlen);
#endif
