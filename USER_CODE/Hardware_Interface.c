/****************************************Copyright (c)****************************************************
**                            		Tellus Power Co. LTD
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           hardware_interface.c
** Last modified Date:  2013-08-08
** Last Version:        V0.01
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Grit.Zhang
** Created date:        2017-09-30
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
#include "hardware_interface.h"
#include "ucos_ii.h"

volatile uint8_t     I2C_sla;                                             /* I2C�����ӵ�ַ                */
volatile uint32_t    I2C_suba;                                            /* I2C�����ڲ��ӵ�ַ            */
volatile uint8_t     I2C_suba_num;                                        /* I2C�ӵ�ַ�ֽ���              */
volatile uint8_t     *I2C_buf;                                            /* ���ݻ�����ָ��               */
volatile uint32_t    I2C_num;                                             /* Ҫ��ȡ/д������ݸ���        */
volatile uint8_t     I2C_end;                                             /* I2C���߽�����־������������  */
                                                                          /* ��1                          */
volatile uint8_t     I2C_suba_en;         /*  �ӵ�ַ���ơ�
                                               0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
                                               1--��ȡ����
                                               2--д����
                                        */

// rf_FM1702 & FM25Cl64 SPI�ӿ�
void SSP0_Init(void)
{
    uint8_t uiRegVal = 0;

    LPC_SC->PCONP |= (1 << 21);                                           /* ����SSP0����                 */

	/* 
	 * �趨�����Ƶֵ
	 */
	uiRegVal = LPC_SC->PCLKSEL;
	if (uiRegVal < 4) {
			LPC_SC->PCLKSEL = 4;
	}
	
	LPC_IOCON->P0_15 &= ~0x07;
	LPC_IOCON->P0_15 |=  0x02;                                              /* SSP CLK                      */
	//LPC_IOCON->P0_16 &= ~0x07;    
	//LPC_IOCON->P0_16 |=  0x02;                                            /* SSP SSEL                     */
	LPC_IOCON->P0_17 &= ~0x07;
	LPC_IOCON->P0_17 |=  0x02;                                              /* SSP MISO                     */
	LPC_IOCON->P0_18 &= ~0x07;    
	LPC_IOCON->P0_18 |=  0x02;                                              /* SSP MOSI                     */
		
	LPC_IOCON->P0_16 &= ~0x07;                                              /* P0.16ΪSSP0��CS�ܽ�          */
	LPC_GPIO0->DIR |= (0x1<<16);
	LPC_GPIO0->SET |= (0x1<<16);

    LPC_SSP0->CR0  = 0x0807;												/* �������ݳ���Ϊ8λ��֡��ʽSPI��SCK����Ч��*/
    LPC_SSP0->CPSR = 0x02;			    		                            /* ����SSP��PCLK��õķ�Ƶ  */
                                                                            /* ֵ,��ģʽ����Ч,         */
                                                                            /* ��СֵΪ0x02	            */
    LPC_SSP0->CR1  = 0x02;		   			                                /* ����SSPΪ����ģʽ,       */	                                                                        /* ��ģʽ����Ч, ��СֵΪ0x02*/   
}

void SSP_Hardeare_SendData(LPC_SSP_TypeDef* SSPx, uint16_t Data)
{
    while ((SSPx->SR & 0x02) == 0);	    	                            /* �ȴ�����FIFO�����ռ�         */   
	SSPx->DR = Data;
    while ((SSPx->SR & 0x10) == 0x10);	 	                            /* �ȴ�����֡�������	        */ 
    return;																// ���SPIF��־
}

uint16_t SSP_Hardeare_ReceiveData(LPC_SSP_TypeDef* SSPx)
{
	uint8_t tmp ;
    while ((SSPx->SR & 0x04) == 0x04)
    {
    	tmp = SSPx->DR ;
    }	    				                    							/* ��ս���FIFO�����ռ�   	*/
    
    while ((SSPx->SR & 0x01) == 0);	    				                    /* ��շ���FIFO�����ռ�   	*/
	SSPx->DR = 0xff ;
    while ((SSPx->SR & 0x10) == 0x10);	 				                    /* �ȴ�����֡�������		*/

    while ((SSPx->SR & 0x04) == 0x00);	 				                    /* �ȴ�����֡�������		*/

	tmp = SSPx->DR ;

    return tmp;   
}

// W5100 SPI�ӿ�	
void SSP1_Init(void)
{
    uint8_t uiRegVal = 0;

    LPC_SC->PCONP |= (0x1<<10);                                         /* ����SSP1����                 */

    /* 
    * �趨�����Ƶֵ
    */
    uiRegVal = LPC_SC->PCLKSEL;
    if ( uiRegVal < 4 ) {
        LPC_SC->PCLKSEL = 4;
    }

    LPC_IOCON->P0_7 &= ~0x07;
    LPC_IOCON->P0_7 |=  0x02;                                       /* SSP CLK                      */
    LPC_IOCON->P0_8 &= ~0x07;
    LPC_IOCON->P0_8 |=  0x02;                                       /* SSP MISO                     */
    LPC_IOCON->P0_9 &= ~0x07;    
    LPC_IOCON->P0_9 |=  0x02;                                       /* SSP MOSI                     */

    LPC_IOCON->P0_6 &= ~0x07;                                  /* P0.6ΪSSP1��CS�ܽ�           */
    LPC_GPIO0->DIR |= (0x1<<6);
    LPC_GPIO0->SET |= (0x1<<6);

    LPC_SSP1->CR0  = 0x0807; 				/* �������ݳ���Ϊ8λ��֡��ʽSPI��SCK����Ч����һ��ʱ���ز�����λ����ΪĬ��ֵ 	*/
    LPC_SSP1->CPSR = 0x02;			    /* ����SSP��PCLK��õķ�Ƶֵ,��ģʽ����Ч, ��СֵΪ0x02 */
    LPC_SSP1->CR1  = 0x02;		   		/* ����SSPΪ����ģʽ,       */	 
}    

void SSP1_IO_Init(void)
{    
    PINSEL_ConfigPin ( 0, 6, 0);
	LPC_GPIO0->DIR |= SSP1_SS_PIN;

	PINSEL_ConfigPin ( 0, 7, 0);
	LPC_GPIO0->DIR |= SSP1_SCLK_PIN;
	PINSEL_ConfigPin ( 0, 9, 0); 
	LPC_GPIO0->DIR |= SSP1_MOSI_PIN;
    
	PINSEL_ConfigPin ( 0, 8, 0);
	LPC_GPIO0->DIR &= ~SSP1_MISO_PIN;
    
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_SS_PIN,1);
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_SCLK_PIN,0);
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_MOSI_PIN,1);
    GPIO_OutputValue(SSP1_SSP_PORT,SSP1_MISO_PIN,1);
}

// ��ʼ��I2C1
void i2c1Init(uint32_t ulFi2c)
{
    LPC_SC->PCONP |= (0x01 << 19);                                      /* ��I2C1ģ�鹦�ʿ���λ       */
    LPC_SC->PCONP |= (0x01 << 15);                                      /* ��GPIOģ�鹦�ʿ���λ       */
    LPC_IOCON->P0_0 = 0x03 | (0x00 << 3) | (0x01 << 10);                /* P0.0����ΪSDA1����          */
    LPC_IOCON->P0_1 = 0x03 | (0x00 << 3) | (0x01 << 10);                /* P0.1����ΪSCL1����          */

    if (ulFi2c>400000) ulFi2c = 400000;

    LPC_I2C1->SCLH = (PeripheralClock / ulFi2c) / 2;                   /* ����I2Cʱ��Ϊfi2c            */
    LPC_I2C1->SCLL = (PeripheralClock / ulFi2c) / 2;

    NVIC_EnableIRQ(I2C1_IRQn);                                          /* ʹ��I2C�ж�                  */
    NVIC_SetPriority(I2C1_IRQn, 11);                                     /* ����I2C1�ж����ȼ���ʹ���ж� */

    LPC_I2C1->CONCLR = 0x2C;
    LPC_I2C1->CONSET = 0x40;                                            /* ʹ����I2C                    */  
}
   
/*********************************************************************************************************
** Function name:           Wait_I2c_End
** Descriptions:            �����ʱ���ṩ��I2C���ߵȴ���ʱʹ��
** input parameters:        dly        ��ʱ������ֵԽ����ʱԽ��
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
uint8_t  Wait_I2c_End(uint32_t  dly)
{  uint32_t  i;

    if( I2C_end==1 ) return (1);
    for(; dly>0; dly--)
    for(i=0; i<5000; i++)
    {
        if( I2C_end==1 ) return (1);
    }
    return (0);
}


/*********************************************************************************************************
** Function name:       I2C_ReadNByte
** Descriptions:        �����ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** input parameters:    sla         ������ַ
**                      suba_type   �ӵ�ַ�ṹ    1�����ֽڵ�ַ    2��8+X�ṹ    2��˫�ֽڵ�ַ
**                      suba        �����ӵ�ַ
**                      s           ���ݽ��ջ�����ָ��
**                      num         ��ȡ�ĸ���
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
uint8_t I2C_ReadNByte (uint8_t sla, uint32_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
    if (num > 0)                                                        /* �ж�num�����ĺϷ���          */
    {                                                                   /* ��������                     */
        if (suba_type == 1)
        {                                                               /* �ӵ�ַΪ���ֽ�               */
            I2C_sla         = sla + 1;                                  /* �������Ĵӵ�ַ��R=1          */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ1�ֽ�            */
        }
        if (suba_type == 2)
        {                                                               /* �ӵ�ַΪ2�ֽ�                */
            I2C_sla         = sla + 1;                                  /* �������Ĵӵ�ַ��R=1          */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 2;                                        /* �����ӵ�ַΪ2�ֽ�            */
        }
        if (suba_type == 3)
        {                                                               /* �ӵ�ַ�ṹΪ8+X              */
            I2C_sla         = sla + ((suba >> 7 )& 0x0e) + 1;           /* �������Ĵӵ�ַ��R=1          */
            I2C_suba        = suba & 0x0ff;                             /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ8+x              */
        }
        I2C_buf     = s;                                                /* ���ݽ��ջ�����ָ��           */
        I2C_num     = num;                                              /* Ҫ��ȡ�ĸ���                 */
        I2C_suba_en = 1;                                                /* ���ӵ�ַ��                   */
        I2C_end     = 0;

                                                                        /* ���STA,SI,AA��־λ          */
        LPC_I2C1->CONCLR = (1 << 2)|                                          /* AA                           */
                           (1 << 3)|                                          /* SI                           */
                           (1 << 5);                                          /* STA                          */

                                                                        /* ��λSTA,����I2C����          */
        LPC_I2C1->CONSET = (1 << 5)|                                          /* STA                          */
                           (1 << 6);                                          /* I2CEN                        */

                                                                        /* �ȴ�I2C�������              */
        return( Wait_I2c_End(20));
    }
    return (FALSE);
} 

/*********************************************************************************************************
** Function name:       I2C_WriteNByte
** Descriptions:        �����ӵ�ַ����д��N�ֽ�����
** input parameters:    sla         ������ַ
**                      suba_type   �ӵ�ַ�ṹ    1�����ֽڵ�ַ    2��8+X�ṹ    2��˫�ֽڵ�ַ
**                      suba        �����ӵ�ַ
**                      s           ��Ҫд������ݵ�ָ��
**                      num         ��Ҫд������ݵĸ���
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
uint8_t I2C_WriteNByte(uint8_t sla, uint8_t suba_type, uint32_t suba, uint8_t *s, uint32_t num)
{
    if (num > 0)                                                        /* �����ȡ�ĸ���Ϊ0���򷵻ش�  */
                                                                        /* ��                           */
    {                                                                   /* ���ò���                     */
        if (suba_type == 1)
        {                                                               /* �ӵ�ַΪ���ֽ�               */
            I2C_sla         = sla;                                      /* �������Ĵӵ�ַ               */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ1�ֽ�            */
        }
        if (suba_type == 2)
        {                                                               /* �ӵ�ַΪ2�ֽ�                */
            I2C_sla         = sla;                                      /* �������Ĵӵ�ַ               */
            I2C_suba        = suba;                                     /* �����ӵ�ַ                   */
            I2C_suba_num    = 2;                                        /* �����ӵ�ַΪ2�ֽ�            */
        }
        if (suba_type == 3)
        {                                                               /* �ӵ�ַ�ṹΪ8+X              */
            I2C_sla         = sla + ((suba >> 7 )& 0x0e);               /* �������Ĵӵ�ַ               */
            I2C_suba        = suba & 0x0ff;                             /* �����ӵ�ַ                   */
            I2C_suba_num    = 1;                                        /* �����ӵ�ַΪ8+X              */
        }

        I2C_buf     = s;                                                /* ����                         */
        I2C_num     = num;                                              /* ���ݸ���                     */
        I2C_suba_en = 2;                                                /* ���ӵ�ַ��д����             */
        I2C_end     = 0;

                                                                        /* ���STA,SI,AA��־λ          */
        LPC_I2C1->CONCLR = (1 << 2)|                                          /* AA                           */
                           (1 << 3)|                                          /* SI                           */
                           (1 << 5);                                          /* STA                          */

                                                                        /* ��λSTA,����I2C����          */
        LPC_I2C1->CONSET = (1 << 5)|                                          /* STA                          */
                           (1 << 6);                                          /* I2CEN                        */

                                                                        /* �ȴ�I2C�������              */
        return( Wait_I2c_End(20));
                /*while (I2C_end == 0)
        {
         //
        }
        if (I2C_end == 1)
            return (TRUE);
        else
            return (FALSE);            */
    }
    return (FALSE);
}


/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/





