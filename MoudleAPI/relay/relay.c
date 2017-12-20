/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           relay.c
** Last modified Date:  2017-11-16
** Last Version:        V0.01
** Descriptions:        it_isr  
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-11-16
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
#include "lpc177x_8x_gpio.h"
#include "relay.h"

void All_Relay_Init(void)
{
    P1P16OFF();         //220�̵����������             ���� P1.16
    PWRPININT();        //220����̵����Ŀ���״̬��ȡ   ���� P2.9
    GUNPININT();        // P1.9 ��ʼ�� ���ǹ״̬���˿�
    
	RLY1OFF();          //12V���                       ���� P2.15
	RLY2OFF();          //12V���                       ���� P2.14
	PWMCOFF();          //cp �������ӼĴ���             ���� P1.5
    P3P25ON();          //cp ���+-12V                  ���� p3.25
	CCRCOFF();          //CC�̵�������                  ���� P3.2

    
	LedLRL();		 							//��ߺ�ɫLED
	LedLGH();					 				
	LedLDH();									//���
    
    LedLCL();
	LedLCH();

					 				
	LedLDL();									//���
     
    LedLCL();
	LedLCH();
  
    
	LedRRL();                                   //�ұ���ɫLED
	LedRGH();
	LedRDH(); 									//�ұ�  
    
    LedRCL();
	LedRCH();
    LedRCL();
	LedRCH();
    LedRCL();
	LedRCH();
}

void LEDBlinkFlow(void)
{
    uint8_t delay;
	LedLRL();		 							//��ߺ�ɫLED
	LedLGH();	
	LedRRL();                                   //�ұ���ɫLED
	LedRGH();  
    for(delay=0;delay<8;delay++)
    {
        
    }
}


/*********************************************************************************************************
** Function name:       pwmDb1Init
** Descriptions:        ��ʼ��˫����PWM���
** input parameters:    ��
** output parameters:   �� 
** Returned value:      ��
*********************************************************************************************************/
void pwmDb1Init (uint8_t tim)
{
    uint32_t	pwmrval;
	if(tim==HIVCNT){
//		pwmstatus = 2;

//		PWMCON();
//		CCRCON();
	}
	else if(tim==PWMCNT){
//		pwmstatus = 1;
        
//		PWMCON();
//		CCRCON();
	}
	else{
//		pwmstatus = 0;

		PWMCOFF();
		CCRCOFF();
	}

//	if(tim==PWMCNT)
//		tim=bcd2hex(Term_Para1.DutyCycle);							 	//����ռ�ձ�

	LPC_SC->PCONP |= (0x01 << 6);                                       /* ��PWMģ�鹦�ʿ���λ       */  
	LPC_IOCON->P3_25  &= ~0x07;										    /* ����ѡ��                    */
	LPC_IOCON->P3_25  |= 2;                                             /* P3.25��PWM1[2]���           */
	pwmrval     = PeripheralClock/10000/2;
	
	LPC_PWM1->PR  = 0x09;                                               /* FPCLK_FREQ��Ƶ��PWM����Ƶ��  */
	                                                                    /* ΪFPCLK_FREQ/10000           */
	LPC_PWM1->MCR = 0x02;                                               /* PWMMR0��PWMTCƥ��ʱ��λPWMTC */
	LPC_PWM1->PCR = (1 << 10);                                        	/* ʹ��PWM2���                 */
	LPC_PWM1->MR0 = pwmrval;                                            /* PWM���ʿ���/������ֵ         */
	LPC_PWM1->MR1 = (pwmrval / HIVCNT) * tim;                                           
	LPC_PWM1->MR2 = (pwmrval / HIVCNT) * tim;    
	                                                                    /* ���ñ�������������λ��     */
	LPC_PWM1->LER = 0x7F;                                               /* ��������PWMƥ��ֵ            */
	LPC_PWM1->TCR = 0x02;                                               /* ��λPWMTC                    */
	LPC_PWM1->TCR = 0x09;                                               /* ʹ��PWM                      */
}









/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/

