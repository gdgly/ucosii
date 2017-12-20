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


#define 							UART1_BPS  	9600 												/* ����ͨ�Ų�����               */
/*********************************************************************************************************
  ����
*********************************************************************************************************/
uint8_t               RcvBufU1[UARTBUFLEN];                 /*  ���ڽ������ݻ�����          */
uint8_t               SndBufU1[UARTBUFLEN];                 /*  ���ڽ������ݻ�����          */
uint32_t              GulNumU1;                             /*  ���ڽ������ݵĸ���          */
uint32_t              RcvLenU1;                             /*  ���ڽ������ݵĸ���          */
uint32_t              SndLenU1;                             /*  ���ڽ������ݵĸ���          */
uint8_t         			RcvNewU1;                       /* ���ڽ��������ݵı�־         */


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
			memset(&RcvBufU1[0],0,sizeof(RcvBufU1));
		}
}

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
     * 8,n,1
     */
    LPC_UART1->LCR = 0x83;
    Fdiv = ( PeripheralClock / 16 ) /  (UART1_BPS*2);                    /*?��3y��y?��                      */
    LPC_UART1->DLM = Fdiv / 256;
    LPC_UART1->DLL = Fdiv % 256;
    LPC_UART1->LCR = 0x03;                                          /* ???��3y��y��??��                 */
    LPC_UART1->FCR = 0x87;                                          /* ��1?��2��?��??FIFO               */
    
    NVIC_EnableIRQ(UART1_IRQn);
  
    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;    
 
	UART1RxbufClear();
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

void UART1SendStr1 (uint8_t const *pucStr, uint32_t ulNum)
{
    UART1RxbufClear();

    while (1) {
        if (*pucStr == '\0') break;                                     /* �������������˳�             */
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


//Packet_Send_232Frame
void Packet_Send_232Frame(uint8_t dateLen, uint8_t * data,uint8_t len)
{
    uint8_t  cs=0,j,counter;
    SndBufU1[8] |= 0x80;
    SndBufU1[9] = dateLen;
    counter = len;
    memcpy(&SndBufU1[21],data,counter);
    //�ۼӼ���У��ֵ
    counter += 21;
    for(j=0;j<counter;j++)
        cs += SndBufU1[j]; 
    SndBufU1[counter++] = cs;
    SndBufU1[counter++] = 0x16;
    SndLenU1 = counter;    
}

//232���ö�ȡ׮��֡
void Packet_Send_Pile_No(void)
{
    Packet_Send_232Frame(0x12,Term_Para1.ChargeNo,sizeof(Term_Para1.ChargeNo));
}

//232���ö�ȡ��������Ϣ֡
void Packet_Send_Maching_No(void)
{
    Packet_Send_232Frame(0x12,Term_Para1.TerminalNo,sizeof(Term_Para1.TerminalNo));
}

//232���ö�ȡ������ip��Ϣ֡
void Packet_Send_ServerIP(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.ServerIP,sizeof(Term_Para1.ServerIP));
}

//232���ö�ȡ����ip��Ϣ֡
void Packet_Send_GateWay(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.GateWay,sizeof(Term_Para1.GateWay));
}

//232���ö�ȡ����ip��Ϣ֡
void Packet_Send_SelfIP(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.SelfIP,sizeof(Term_Para1.SelfIP));  
}

//232���ö�ȡ����������Ϣ֡
void Packet_Send_SelfMASK(void)
{
    Packet_Send_232Frame(0x0e,Term_Para1.SelfMASK,sizeof(Term_Para1.SelfMASK));  
}

//232���ö�ȡ����������Ϣ֡
void Packet_Send_SelfMAC(void)
{
    Packet_Send_232Frame(0x10,Term_Para1.SelfMAC,sizeof(Term_Para1.SelfMAC));       
}


//����232���óɹ�
void SendResetSucessFrame(uint8_t dateLen)
{
    uint8_t  cs=0,j,counter;
    SndBufU1[8] |= 0x80;
    SndBufU1[9] = dateLen;
    SndBufU1[10] = 0x00;
    SndBufU1[11] = 0x00;
    SndBufU1[12] = SndBufU1[16];
    SndBufU1[13] = SndBufU1[17];
    for(j=0;j<dateLen-3;j++)
        SndBufU1[14+j]= 0x00;  
    counter = 14+j;
    for(j=0;j<counter;j++)
        cs += SndBufU1[j];
    SndBufU1[counter++] = cs;
    SndBufU1[counter++] = 0x16;
    SndLenU1 = counter;       
}

//���յ�232����׮��֡
void Reset_Pile_No(void)
{
    memcpy(Term_Para1.ChargeNo,&SndBufU1[18],8);
    FRAMWrite(Term_Para1.ChargeNo,TERMINAL_PARA_ADD+2,8);
    SendResetSucessFrame(0x05);
}

//���յ�232���û�����֡
void Reset_Maching_No(void)
{
    memcpy(Term_Para1.TerminalNo,&SndBufU1[18],8);    
    FRAMWrite(Term_Para1.TerminalNo,TERMINAL_PARA_ADD+10,8);
    SendResetSucessFrame(0x05);    
}


//���յ�232���÷�����ip֡
void Reset_ServerIP(void)
{
    memcpy(Term_Para1.ServerIP,&SndBufU1[18],4);
    FRAMWrite(Term_Para1.ServerIP,TERMINAL_PARA_ADD+100,4);
    FRAMWrite(Term_Para1.ServerIP,TERMINAL_PARA_ADD_BAK+100,4);    
    SendResetSucessFrame(0x04);    
}


//���յ�232��������֡
void Reset_GateWay(void)
{
    memcpy(Term_Para1.GateWay,&SndBufU1[18],4); 
    FRAMWrite(Term_Para1.GateWay,TERMINAL_PARA_ADD+106,4); 
    SendResetSucessFrame(0x04);     
}


//���յ�232���ñ���ip֡
void Reset_SelfIP(void)
{
    memcpy(Term_Para1.SelfIP,&SndBufU1[18],4); 
    FRAMWrite(Term_Para1.SelfIP,TERMINAL_PARA_ADD+110,4);
    SendResetSucessFrame(0x04);     
}


//���յ�232������������
void Reset_SelfMASK(void)
{
    memcpy(Term_Para1.SelfMASK,&SndBufU1[18],4);
    FRAMWrite(Term_Para1.SelfMASK,TERMINAL_PARA_ADD+116,4);
    SendResetSucessFrame(0x04);     
}


//���յ�232���������ַ
void Reset_SelfMAC(void)
{
    memcpy(Term_Para1.SelfMAC,&SndBufU1[18],6);
    FRAMWrite(Term_Para1.SelfMAC,TERMINAL_PARA_ADD+120,6);
    SendResetSucessFrame(0x04);     
}



void CommandProcess(uint8_t *pdat,uint8_t no)
{
	uint8_t  temp,cmd;
    cmd = *(pdat+8);
    if(cmd == 0x01) //��ȡ��Ϣ
    {
        memcpy(SndBufU1,RcvBufU1,RcvLenU1);        
        temp = *(pdat+19);
        switch(temp){
            case 0x01:
                Packet_Send_Pile_No();
                break;
            case 0x02:
                Packet_Send_Maching_No();
                break;
            case 0x08:
                Packet_Send_ServerIP();
                break; 
            case 0x0A:
                Packet_Send_GateWay();
                break;
            case 0x0B:
                Packet_Send_SelfIP();
                break;
            case 0x0D:
                Packet_Send_SelfMASK();
                break;
            case 0x0E:
                Packet_Send_SelfMAC();
                break;            
            default:
                break;            
        }
        UART1SendStr(SndBufU1,SndLenU1);        
    }else if(cmd == 0x08) //д��Ϣ
    {
        memcpy(SndBufU1,RcvBufU1,RcvLenU1);        
        temp = *(pdat+16);
        switch(temp){
            case 0x01:
                Reset_Pile_No();
                break;
            case 0x02:
                Reset_Maching_No();
                break;
            case 0x08:
                Reset_ServerIP();
                break; 
            case 0x0A:
                Reset_GateWay();
                break;
            case 0x0B:
                Reset_SelfIP();
                break;
            case 0x0D:
                Reset_SelfMASK();
                break;
            case 0x0E:
                Reset_SelfMAC();
                break;            
            default:
                break;            
        }        
        UART1SendStr(SndBufU1,SndLenU1);
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
    //uint8_t timdiv;
	
	OSTimeDly(OS_TICKS_PER_SEC); 										//��ʱ�ȴ�ϵͳ����������������		
	UART1Init();                                                       /* ���ڳ�ʼ��                   */	

	while (1)
	{
        OSTimeDly(OS_TICKS_PER_SEC/4);	
        if(RcvNewU1)
        {                                                       
            if(CheckSum0904((uint8_t *)&RcvBufU1,RcvLenU1)){
        		CommandProcess((uint8_t *)&RcvBufU1,1);
            }
            else
            {
        		OSTimeDly(OS_TICKS_PER_SEC/4);
        		if(CheckSum0904((uint8_t *)&RcvBufU1,RcvLenU1))	{
    				CommandProcess((uint8_t *)&RcvBufU1,1);
                }
            }
            UART1RxbufClear();								
        }
        
        
	}
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
