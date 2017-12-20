#ifndef _RTC_H_
#define _RTC_H_

#include "AllHand.h"

/*********************************************************************************************************
** Function name:       ExRTCSet
** Descriptions:        �����ⲿʱ��
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void ExRTCSet(PCF8563_DATE *pTimeAndDate );

/*********************************************************************************************************
** Function name:       ExRTCInit
** Descriptions:        �ⲿʱ�ӳ�ʼ��
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void ExRTCInit(void);

/*********************************************************************************************************
** Function name:       main_rtc
** Descriptions:        PCF8563ʱ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void main_rtc (void);


#endif

