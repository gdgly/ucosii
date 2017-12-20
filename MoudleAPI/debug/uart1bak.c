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
** Descriptions:        ��дUART1����
**
** Rechecked by:        Zhang bin1
*********************************************************************************************************/
#include "UART1.h"

/*********************************************************************************************************
  ������궨��
*********************************************************************************************************/

/*********************************************************************************************************
  
*********************************************************************************************************/
uint8_t               RcvBufU1[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU1[UARTBUFLEN];                                      /*  ���ڽ������ݻ�����          */
uint32_t              GulNumU1;                                             		 /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU1;                                              /*  ���ڽ������ݵĸ���          */
uint32_t              SndLenU1;                                              /*  ���ڽ������ݵĸ���          */
uint8_t         			RcvNewU1;                            		/* ���ڽ��������ݵı�־         */


#define UART1_BPS  	9600 												/* ����ͨ�Ų�����               */
/*********************************************************************************************************
** Function name:	    UART1Init
** Descriptions:	    UART1��ʼ��������Ϊ8λ����λ��1λֹͣλ����У�飬������ΪUART1_BPS
** input parameters:    ulDly:   ֵԽ����ʱʱ��Խ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1Init (void)
{
		       
    uint32_t Fdiv;
    
		LPC_SC->PCONP |= 0x00000010;
    #if 0
        LPC_IOCON->P0_15 &= ~0x07;
        LPC_IOCON->P0_15 |= 1;                                      /* P0.15?aU1_TXD                */
        LPC_IOCON->P0_16 &= ~0x07;
        LPC_IOCON->P0_16 |= 1;                                      /* P0.16?aU1_RXD                */
    #endif
    #if 1
        LPC_IOCON->P3_16 &= ~0x07;
        LPC_IOCON->P3_16 |= 3;                                      /* P3.16?aU1_TXD                */
        LPC_IOCON->P3_17 &= ~0x07;
        LPC_IOCON->P3_17 |= 3;                                      /* P3.17?aU1_RXD                */
    #endif
    #if 0
        LPC_IOCON->P2_0  &= ~0x07;
        LPC_IOCON->P2_0  |= 2;                                      /* P2.0?aU1_TXD                 */
        LPC_IOCON->P2_1  &= ~0x07;
        LPC_IOCON->P2_1  |= 2;                                      /* P2.1?aU1_TXD                 */
    #endif
    /*
     * 8?a��y?Y??��??T?????��?��??��?1??����?1??
     */
    LPC_UART1->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) /  (UART1_BPS*2);                    /*?��3y��y?��                      */
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                          /* ???��3y��y��??��                 */
    LPC_UART1->FCR = 0x07;                                          /* ��1?��2��?��??FIFO               */
    
    NVIC_EnableIRQ(UART1_IRQn);
  
    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;    
 
}

/*********************************************************************************************************
** Function name:	    UART1RxbufClear
** Descriptions:    	��ս��ܻ���
** input parameters:    
**                      
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1RxbufClear(void )
{
		if(RcvNewU1!=0||RcvLenU1!=0){
			RcvNewU1 = 0;                                              		/* �����־*/
			RcvLenU1 = 0;
			memset(RcvBufU1,0,sizeof(RcvBufU1));
		}
}

/*********************************************************************************************************
** Function name:	    UART1SendByte
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1SendByte (uint8_t ucDat)
{
    LPC_UART1->THR = ucDat;                                                      /* д������                     */
    while ((LPC_UART1->LSR & 0x20) == 0);                                		/* �ȴ����ݷ������             */
}


/*********************************************************************************************************
** Function name:	    UART1SendStr
** Descriptions:    	�򴮿ڷ����ַ���
** input parameters:    pucStr:  Ҫ���͵��ַ���ָ��
**                      ulNum:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UART1SendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
		UART1RxbufClear();
    for (i = 0; i < ulNum; i++) {                                       /* ����ָ�����ֽ�����           */
        UART1SendByte (*pucStr++);
    }

}

/*********************************************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : UART1 �жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void UART1_IRQHandler(void)
{ 
    uint8_t uiIIRValue, uiLSRValue;
    uint8_t uiDummy = uiDummy;
// 		uint32_t UART1Status = 0;

  	uiIIRValue = LPC_UART1->IIR;

    uiIIRValue >>= 1;                                                     /* �жϱ�־�Ĵ���ֵ����1λ    */
    uiIIRValue &= 0x07;

    if ( uiIIRValue == IIR_RLS ) {                                        /* ������״̬�ж�             */
        uiLSRValue = LPC_UART1->LSR;
        /* 
         * ����жϡ���ż�жϡ�֡�жϡ�UART RBR�а����������ݺ�֡����жϴ���
         */
        if ( uiLSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
//             UART1Status = uiLSRValue;
            uiDummy = LPC_UART1->RBR;
            return;
        }
        if ( uiLSRValue & LSR_RDR ) {                                     /* ����FIFO��Ϊ���ж�         */
// 					  UART1Buffer[UART1Count] = LPC_UART1->RBR;
//             UART1Count++;
//             if ( UART1Count == UARTBUFLEN ) {
//                 UART1Count = 0;
//             }
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++){                 /*  ��������8���ֽ�             */
							if ( GulNumU1+RcvLenU1 >= UARTBUFLEN ) {
									UART1RxbufClear();
							}
							RcvBufU1[GulNumU1+RcvLenU1] = LPC_UART1->RBR;
            }
						RcvLenU1+=GulNumU1;
						RcvNewU1 = 1; 					
        }
    }
    else if ( uiIIRValue == IIR_RDA ) {                                   /* �������ݿ����ж�           */
//         UART1Buffer[UART1Count] = LPC_UART1->RBR;
//         UART1Count++;
//         if ( UART1Count == UARTBUFLEN ) {
//             UART1Count = 0;
//         }
            for (GulNumU1 = 0; GulNumU1 < 8; GulNumU1++){                 /*  ��������8���ֽ�             */
							if ( GulNumU1+RcvLenU1 >= UARTBUFLEN ) {
									UART1RxbufClear();
							}
							RcvBufU1[GulNumU1+RcvLenU1] = LPC_UART1->RBR;
            }
						RcvLenU1+=GulNumU1;
						RcvNewU1 = 1; 				
			
			
    }
    else if ( uiIIRValue == IIR_CTI ) {                                   /* �ַ���ʱ�ж�               */
          while ((LPC_UART1->LSR & 0x01) == 0x01){  
						if(RcvLenU1 >= UARTBUFLEN){
							UART1RxbufClear();
						}                       								/*  �ж������Ƿ�������        */ 
				    RcvBufU1[RcvLenU1] = LPC_UART1->RBR;
            RcvLenU1++;
					}
				  RcvNewU1 = 1; 											/* 	���յ���������֡ 			*/
    }
    else if ( uiIIRValue == IIR_THRE ) {                                  /* ���ͱ��ּĴ������ж�       */
        uiLSRValue = LPC_UART1->LSR;
        if ( uiLSRValue & LSR_THRE ) {
            //UART1TxEmpty = 1;
        } else {
            //UART1TxEmpty = 0;
        }
    }
}

/*********************************************************************************************************
** Function name:       main_UART1
** Descriptions:        UnionPay CardͨѶ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void main_uart1 (void)
{
  OSTimeDly(OS_TICKS_PER_SEC); 										//��ʱ�ȴ�ϵͳ����������������
  
  UART1Init();                                                       /* ���ڳ�ʼ��                   */
 
	UART1RxbufClear();
	while(1){
		OSTimeDly(OS_TICKS_PER_SEC/10);
		if(RcvNewU1){
			OSTimeDly(OS_TICKS_PER_SEC/10);
			uart2SendStr(RcvBufU1,RcvLenU1);
			UART1RxbufClear();
		}
	}
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
