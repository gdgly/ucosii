/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           it_isr.c
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
#include "AllHand.h"



/*********************************************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTC�жϴ�����
* Input          : ��
* Output         : ��
* Return         : ��
*********************************************************************************************************/
void  RTC_IRQHandler (void)
{  	                                                 					
	static uint8_t tmp;
    uint32_t ulTimes;
    uint32_t ulDatas;
    
	ulTimes = LPC_RTC->CTIME0;                                               /* ��ȡ������ʱ�ӼĴ���         */
	ulDatas = LPC_RTC->CTIME1;

	RTCtimedate.year   	= (ulDatas >> 16) & 0xfff; 
	RTCtimedate.month  	= (ulDatas >> 8) & 0x0f; 
	RTCtimedate.day   	= ulDatas & 0x1f; 
	RTCtimedate.week 	= (ulTimes >> 24) & 0x07; 
	RTCtimedate.hour 	= (ulTimes >> 16) & 0x1f;
	RTCtimedate.minute	= (ulTimes >> 8) & 0x3f;
	RTCtimedate.second	= ulTimes & 0x3f;

	LPC_RTC->ILR = 0x01;													/* ����жϱ�־                 */														   	
	
	time_flg |= SECFLG;//secflg = 1;   									   	//���־
	
    Time_Seconds++;
    
//    if((time_flg&PTL104_TIME_TO_HEART) == 0){
//        if(Time_Seconds%SECONDS_PER_HEART_BEAT == 0){
//            time_flg |= PTL104_TIME_TO_HEART;
//        }
//    }
//    if(time_flg&PTL104_HEART_WAIT_ACK){
//        Wait_Heart_Ack_Seconds++;
//        if(Wait_Heart_Ack_Seconds == TIMEOUT_HEART_W_A){
//            time_flg |= PTL104_TIME_TO_HEART;
//            //��ʱ��ʶ��1
//        }
//    }
//    if((time_flg&PTL104_EVENT_WAIT_ACK) == 0){
//        if(Time_Seconds%SECONDS_PER_EVENT_UPDATE == 0){
//            time_flg |= PTL104_TIME_TO_EVENT;
//        }
//    }
//    if(time_flg&PTL104_HEART_WAIT_ACK){
//        Wait_Event_Ack_Seconds++;
//        if(Wait_Event_Ack_Seconds == TIMEOUT_EVENT_W_A){
//            time_flg |= PTL104_TIME_TO_EVENT;//�ط��¼��ϱ�֡
//            //��ʱ��ʶ��1
//        }
//    }
    
    
//	if(MsaConFlag == 1){
//		UartDlyTim.GucHeartF1++;												//����֡��Ӧ��ʱ 
//		AckTime++;																//��ʱ��ʱ
//	}
//	if(sysupdatetimer)
//	{
//		sysupdatetimer --;
//	}	
//	if(sysupdatetimer == 0) SysUpdateFlag = 0;
//	if(PTL104State == 1 || PTL104State == 2)
//		PTL104StateCount++;
//	if(PTL104StateCount >= 10)
//	{
//		TcpConFlag=0;										                //���µ�¼��վ	
//		MsaConFlag=0;
//		wirelessConnectState = 0;
//		PTL104State = 0;
//		PTL104StateCount = 0;
//	}
//	if(readCardInterval != 0)
//		readCardInterval--;
//	if(wirelessConnectState == 11)
//		gprsSisoCount++;
//	if(gprsSisoCount >= 150){
//		wirelessConnectState = 0;
//		gprsSisoCount = 0;
//	}
//	DspDlyNs++;															    //��ʱ��ʱ
//		
//	sSysOvrTim.ADCVALCNT++;	 												//AD�����쳣����
//	if(SysCtlWrod.status == WORK3 || SysCtlWrod.activech==1) 		
//		SysCtlWrod.time++;
//	if(tmp++%2){
//		SYSRUNON();
//	}
//	else {
//		SYSRUNOFF();
//	}

//	if(sysresetflag){
//		if(sysresetcnt++>30){			
//			wdt_entry();  								                    //���Ź���λϵͳ
//			while(1);			
//			//zyReset(ZY_HARD_RESET);	 				                    //��������ϵͳ��λ
//		}	
//	}

}


extern volatile uint8_t     I2C_sla;                                             /* I2C�����ӵ�ַ                */
extern volatile uint32_t    I2C_suba;                                            /* I2C�����ڲ��ӵ�ַ            */
extern volatile uint8_t     I2C_suba_num;                                        /* I2C�ӵ�ַ�ֽ���              */
extern volatile uint8_t     *I2C_buf;                                            /* ���ݻ�����ָ��               */
extern volatile uint32_t    I2C_num;                                             /* Ҫ��ȡ/д������ݸ���        */
extern volatile uint8_t     I2C_end;                                             /* I2C���߽�����־������������  */
                                                                          /* ��1                          */
extern volatile uint8_t     I2C_suba_en;         /*  �ӵ�ַ���ơ�
                                               0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
                                               1--��ȡ����
                                               2--д����
                                        */

/*********************************************************************************************************
** Function name:       I2C1_IRQHandler
** Descriptions:        Ӳ��I2C�жϷ������
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ע�⴦���ӵ�ַΪ2�ֽڵ������
*********************************************************************************************************/
void  I2C1_IRQHandler(void)
{   /*
     *��ȡI2C״̬�Ĵ���I2DAT,����ȫ�ֱ��������ý��в��������������־,����ж��߼�,�жϷ���
     */
    switch (LPC_I2C1->STAT & 0xF8)
    {   /* ����״̬�������Ӧ�Ĵ���     */
        case 0x08:    /* �ѷ�����ʼ����,�����ͺ������ն���,װ��SLA+W����SLA+R */
        if(I2C_suba_en == 1)/* SLA+R */                                 /* ָ���ӵ�ַ��                 */
        {   LPC_I2C1->DAT = I2C_sla & 0xFE;                                   /* ��д���ַ                   */
        }
        else                                                            /* SLA+W                        */
        {   LPC_I2C1->DAT = I2C_sla;                                          /* ����ֱ�ӷ��ʹӻ���ַ         */
        }
                                                                        /* ����SIλ                     */
        LPC_I2C1->CONCLR = (1 << 3)|                                    /* SI                           */
                           (1 << 5);                                    /* STA                          */
        break;

        case 0x10:    /*�ѷ����ظ���ʼ���� */                           /* �����ͺ������ն���           */
                                                                        /* װ��SLA+W����SLA+R           */
        LPC_I2C1->DAT = I2C_sla;                                              /* �������ߺ��ط��ӵ�ַ       */
        LPC_I2C1->CONCLR = 0x28;                                              /* ����SI,STA                   */
        break;

        case 0x18:
        case 0x28:                                                      /* �ѷ���I2DAT�е�����,�ѽ���ACK*/
        if (I2C_suba_en == 0)
        {
            if (I2C_num > 0)
            {    LPC_I2C1->DAT = *I2C_buf++;
                LPC_I2C1->CONCLR = 0x28;                                      /* ����SI,STA                   */
                I2C_num--;
            }
            else                                                        /* û�����ݷ�����               */
            {                                                           /* ֹͣ����                     */
                LPC_I2C1->CONSET = (1 << 4);                                  /* STO                          */
                LPC_I2C1->CONCLR = 0x28;                                      /* ����SI,STA                   */
                I2C_end = 1;                                            /* �����Ѿ�ֹͣ                 */
            }
        }

        if(I2C_suba_en == 1)                                            /* ����ָ����ַ��,��������������*/
        {
            if (I2C_suba_num == 2)
            {   LPC_I2C1->DAT = ((I2C_suba >> 8) & 0xff);
                LPC_I2C1->CONCLR = 0x28;                                      /* ����SI,STA                   */
                I2C_suba_num--;
                break;
            }

            if(I2C_suba_num == 1)
            {   LPC_I2C1->DAT = (I2C_suba & 0xff);
                LPC_I2C1->CONCLR = 0x28;                                      /* ����SI,STA                   */
                I2C_suba_num--;
                break;
            }

            if (I2C_suba_num == 0)
            {
                LPC_I2C1->CONCLR = 0x08;
                LPC_I2C1->CONSET = 0x20;
                I2C_suba_en = 0;                                        /* �ӵ�ַ������                 */
                break;
            }
        }

        if (I2C_suba_en == 2)                                           /* ָ���ӵ�ַд,�ӵ�ַ��δָ��, */
                                                                        /* �����ӵ�ַ                 */
        {
            if (I2C_suba_num > 0)
            {    if (I2C_suba_num == 2)
                {    LPC_I2C1->DAT = ((I2C_suba >> 8) & 0xff);
                    LPC_I2C1->CONCLR = 0x28;
                    I2C_suba_num--;
                    break;
                }
                if (I2C_suba_num == 1)
                {    LPC_I2C1->DAT    = (I2C_suba & 0xff);
                    LPC_I2C1->CONCLR = 0x28;
                    I2C_suba_num--;
                    I2C_suba_en  = 0;
                    break;
                }
            }
        }
        break;

        case 0x40:                                                      /* �ѷ���SLA+R,�ѽ���ACK        */
        if (I2C_num <= 1)                                               /* ��������һ���ֽ�           */
        {    LPC_I2C1->CONCLR = 1 << 2;                                       /* �´η��ͷ�Ӧ���ź�           */
        }
        else
        {    LPC_I2C1->CONSET = 1 << 2;                                       /* �´η���Ӧ���ź�             */
        }
        LPC_I2C1->CONCLR = 0x28;                                              /* ����SI,STA                   */
        break;

        case 0x20:                                                      /* �ѷ���SLA+W,�ѽ��շ�Ӧ��     */
        case 0x30:                                                      /* �ѷ���I2DAT�е����ݣ��ѽ���  */
                                                                        /* ��Ӧ��                       */
        case 0x38:                                                      /* ��SLA+R/W�������ֽ��ж�ʧ��  */
                                                                        /* ��                           */
        case 0x48:                                                      /* �ѷ���SLA+R,�ѽ��շ�Ӧ��     */
        LPC_I2C1->CONCLR = 0x28;
        I2C_end = 0xFF;
        break;

        case 0x50:                                                      /* �ѽ��������ֽڣ��ѷ���ACK    */
        *I2C_buf++ = LPC_I2C1->DAT;
        I2C_num--;
        if (I2C_num == 1)                                               /* �������һ���ֽ�             */
        {   LPC_I2C1->CONCLR = 0x2C;                                          /* STA,SI,AA = 0                */
        }
        else
        {   LPC_I2C1->CONSET = 0x04;                                          /* AA=1                         */
            LPC_I2C1->CONCLR = 0x28;
        }
        break;

        case 0x58:                                                      /* �ѽ��������ֽڣ��ѷ��ط�Ӧ�� */
        *I2C_buf++ = LPC_I2C1->DAT;                                           /* ��ȡ���һ�ֽ�����           */
        LPC_I2C1->CONSET = 0x10;                                              /* ��������                     */
        LPC_I2C1->CONCLR = 0x28;
        I2C_end = 1;
        break;

        default:
        break;
    }
}

/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

