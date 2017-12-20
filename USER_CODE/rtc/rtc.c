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
** Version:             V1.01
** Descriptions:        ��дI2C����
**
** Rechecked by:        CaiHua
*********************************************************************************************************/
#include "rtc.h"

/*********************************************************************************************************
** Function name:       rtcInit
** Descriptions:        ��ʼ�� LPC1778 RTC
** input parameters:    ucrtcTime:�����ʼ��ʱ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint32_t rtcInit (PCF8563_DATE *pTimeAndDate )
{	
  	OS_ENTER_CRITICAL();

    LPC_SC->PCONP |= (1 << 9);                                              /* ���� RTC Ҫ�õ��ⲿʱ��   */

    if ( LPC_RTC->RTC_AUX & (0x1<<4) )                                      /* RTC ����ʧЧ̽���־    */
    {
    	LPC_RTC->RTC_AUX |= (0x1<<4);                                       /* ���                      */
    }
	
	LPC_RTC->CCR   = 0x00;                                                  /* ��ֹʱ�������            */
    LPC_RTC->ILR   = 0x03;                                                  /* ����ж�                  */
    LPC_RTC->CIIR  = 0x1;                                                   /* ʹ���������ж�            */
	//LPC_RTC->CIIR = 0;                                                    /* ������������              */
	LPC_RTC->AMR   = 0xff;                                                  /* ��ֹ�����ж�              */

	//LPC_RTC->CALIBRATION = 0x0;                                           /* У׼�Ĵ�����0             */

		
    LPC_RTC->YEAR  = pTimeAndDate->year;                                    /* ����ʱ������Ĵ���        */
    LPC_RTC->MONTH = pTimeAndDate->month;
    LPC_RTC->DOM   = pTimeAndDate->day;
    LPC_RTC->DOW   = pTimeAndDate->week;
    LPC_RTC->HOUR  = pTimeAndDate->hour;
    LPC_RTC->MIN   = pTimeAndDate->minute;
    LPC_RTC->SEC   = pTimeAndDate->second;
    
    LPC_RTC->CIIR  = 0x01;
    //zyIsrSet(NVIC_RTC, (unsigned long)rtcIntIsr,PRIO_EIGHTEEN);           /* �����жϲ�ʹ��                  */
    NVIC_EnableIRQ(RTC_IRQn);                                               /* �����жϲ�ʹ��                  */
    NVIC_SetPriority(RTC_IRQn, 18);
   
    LPC_RTC->CIIR = 0x00000001;                                             /* Set counter increment interrupt */
    LPC_RTC->ILR = (0x01 | 0x02);                                           /* ��������������ж� ��־         */

    LPC_RTC->CCR   = 0x01;                                                  /* ����RTC                         */   
    OS_EXIT_CRITICAL();
    return 1; 
}

/*********************************************************************************************************
** Function name:       ExRTCSet
** Descriptions:        �����ⲿʱ��
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ExRTCSet(PCF8563_DATE *pTimeAndDate )
{	 
    PCF8563_SS(Stop);                                                   /* ֹͣPCF8563                  */                             
    PCF8563_Set(pTimeAndDate);                                          /* ��ʼ������                   */	                                
    PCF8563_Set_Timer(TimerOn|TimerClk64Hz,64);                         /* ���ö�ʱ��ÿ�������һ��     */
    PCF8563_INT_State(SetINT|TIE|TITP);                                 /* ʹ�ܶ�ʱ���ж�               */
    PCF8563_SS(Start);                                                  /* ����PCF8563                  */                                           
} 
/*********************************************************************************************************
** Function name:       ExRTCInit
** Descriptions:        �ⲿʱ�ӳ�ʼ��
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ExRTCInit(void)
{
    PCF8563_SS(Stop);                                                   /* ֹͣPCF8563                  */
    TimeAndDate.year     = 2014;                                        /* ��ʼ����                     */
    TimeAndDate.month    = 6;                                           /* ��ʼ����                     */
    TimeAndDate.day      = 5;                                           /* ��ʼ����                     */
    TimeAndDate.week     = 4;                                           /* ��ʼ����                     */
    TimeAndDate.hour     = 12;                                          /* ��ʼ��ʱ                     */
    TimeAndDate.minute   = 12;                                          /* ��ʼ����                     */
    TimeAndDate.second   = 12;                                          /* ��ʼ����                     */
    PCF8563_Set(&TimeAndDate);                                          /* ��ʼ������                   */
    PCF8563_Set_Timer(TimerOn|TimerClk64Hz,64);                         /* ���ö�ʱ��ÿ�������һ��     */
    PCF8563_INT_State(SetINT|TIE|TITP);                                 /* ʹ�ܶ�ʱ���ж�               */
    PCF8563_SS(Start);                                                  /* ����PCF8563                  */   

}
/*********************************************************************************************************
** Function name:       main_rtc
** Descriptions:        PCF8563ʱ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void main_rtc (void)
{
	if(PCF8563_Read(&TimeAndDate)==FALSE){
		ExRTCInit();
	}
	if(TimeAndDate.year<=2000 || TimeAndDate.month >12 || TimeAndDate.month == 0 || 
		TimeAndDate.day >31 || TimeAndDate.day == 0 || TimeAndDate.hour >=24 || 
			TimeAndDate.minute >=60|| TimeAndDate.second >=60 )	{
		ExRTCInit();
	}
    rtcInit(&TimeAndDate);
}
/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
