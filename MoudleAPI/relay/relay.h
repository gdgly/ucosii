#ifndef __RELAY_H__
#define __RELAY_H__

#include "global.h"
//220v��ͨ�̵���		            
//	�������	P180	P1[16]
//			
//	��ȡ״̬	P132	P2[9]


//			
//			
//CP			
//	��ѹ���	P40	P1[31]
//			
//	���Ƽ̵���	P156	P1[5]
//			
//	�������+-12V	P56	P3[25]      
//			
//			
//CC(δ��)			
//	���	P185	P1[2]
//			
//	���Ƽ̵���	P207	P3[2]
//					
//12V����̵���			
//	�������1	P91	P2[14]
//			
//	���2	P99	P2[15]
//			
//			
//���ǹ��			
//			
//	���(5v�Ա�in)	P188	P1[9]

#define		HIVCNT		100			//����ߵ�ƽ
#define		PWMSTR	   	27	   		//16A (ϵ��0.6)
#define		PWMCNT	   	53			//32A (ϵ��0.6)
#define		PWMLOW	   	0			//�����ѹ

void All_Relay_Init(void);
void pwmDb1Init (uint8_t tim);


#endif