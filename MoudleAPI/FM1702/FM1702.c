/****************************************************************************
* File :  main.c                                                            *
* COPYRIGHT BY HUOYAN LTD.COMPANY                                           *
* Version:      V1.3                                      			        *
*                                                                           *
* Created:      18.10.2005                                                  *
* Last Change:  21.10.2005                                                  *
*                                                                           *
* Author:       NIL                                                         *
*                                                                           *
* Compiler:     KEIL C51 V7.10                                              *
*                                                                           *
* Description:  AT89S52-Firmware for FM1702 Demo Serial Reader              *
*                                                                           *
****************************************************************************/
#define IN_FM1702_GLOBALS 
#include "FM1702.h"


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
void ddelay(INT16U dely)
{
	INT8U i, j;
	for(i  =0; i <= dely/10; i++)
	{
		for(j = 0; j <= dely; j++)
		{
			;
		}
	}
}

///*********************************************************************************************************
//** Function name:		SSP0_Init
//** Descriptions:		��SSP����������Ϊ����
//** input parameters:	��
//** output parameters:	��
//**********************************************************************************************************/
//void  SSP0_Init(void)
//{
//    uint8_t uiRegVal = 0;
//	
//    //PCONP 	|= (1 << 21);		    									/* ��SSP0��Դ                 */
//    LPC_SC->PCONP |= (0x1 << 21);                                           /* ����SSP0����                 */

//	/* 
//	 * �趨�����Ƶֵ
//	 */
//	uiRegVal = LPC_SC->PCLKSEL;
//	if (uiRegVal < 4) {
//			LPC_SC->PCLKSEL = 4;
//	}
//	
//	LPC_IOCON->P0_15 &= ~0x07;
//	LPC_IOCON->P0_15 |=  0x02;                                              /* SSP CLK                      */
//	//LPC_IOCON->P0_16 &= ~0x07;    
//	//LPC_IOCON->P0_16 |=  0x02;                                            /* SSP SSEL                     */
//	LPC_IOCON->P0_17 &= ~0x07;
//	LPC_IOCON->P0_17 |=  0x02;                                              /* SSP MISO                     */
//	LPC_IOCON->P0_18 &= ~0x07;    
//	LPC_IOCON->P0_18 |=  0x02;                                              /* SSP MOSI                     */
//		
//	//PINSEL0 |= (2ul << 30);												/* ���ùܽŹ��� p0.15                */
//	//PINSEL1 |= (0 << 0 ) | (2 << 2 ) | (2 << 4 );						    /* ���ùܽŹ��� p0.16  17 18              */
//	//FIO0DIR2 |= 1 << 0 ;												    /* cs �ܽ� p0.16 */
//	
//	//SET_SSPCSRF() ; 
//	LPC_IOCON->P0_16 &= ~0x07;                                              /* P0.16ΪSSP0��CS�ܽ�          */
//	LPC_GPIO0->DIR |= (0x1<<16);
//	LPC_GPIO0->SET |= (0x1<<16);

////	PINSEL0 |= (0 << 6);	
////	FIO0DIR0 |= 1 << 3 ;												    /* cs �ܽ� p0.3 */
////	SET_SSPCSFR() ; 

//    LPC_SSP0->CR0  = 0x0807;												/* �������ݳ���Ϊ8λ��֡��ʽSPI��SCK����Ч��*/
//    LPC_SSP0->CPSR = 0x02;			    		                            /* ����SSP��PCLK��õķ�Ƶ  */
//                                                                            /* ֵ,��ģʽ����Ч,         */
//                                                                            /* ��СֵΪ0x02	            */
//    LPC_SSP0->CR1  = 0x02;		   			                                /* ����SSPΪ����ģʽ,       */
//	  //0x01                 		   			                            /* ����SSPΪ����ģʽ,       */	
//	                                                                        /* ��ģʽ����Ч, ��СֵΪ0x02*/
//}


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
void  FMSendByte(INT8U data)
{  
    //while ((SSP0SR & 0x02) == 0);	    				                    /* �ȴ�����FIFO�����ռ�   	*/
    while (!(LPC_SSP0->SR & 0x01));	    				                    /* �ȴ�����FIFO�����ռ�   	*/
		LPC_SSP0->DR = data;
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 				                /* �ȴ�����֡�������		*/
}

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
INT16U  FMReadByte(void )
{  
    INT8U tmp ;
    while ((LPC_SSP0->SR & 0x04) == 0x04){
    	tmp = LPC_SSP0->DR ;
    }	    				                    							/* ��ս���FIFO�����ռ�   	*/
    
    while ((LPC_SSP0->SR & 0x01) == 0);	    				                /* ��շ���FIFO�����ռ�   	*/
	LPC_SSP0->DR = 0xff ;
    while ((LPC_SSP0->SR & 0x10) == 0x10);	 				                /* �ȴ�����֡�������		*/

    while ((LPC_SSP0->SR & 0x04) == 0x00);	 				                /* �ȴ�����֡�������		*/

	tmp = LPC_SSP0->DR ;

    return tmp;
}

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
void FMWrite(INT8U dataptr,INT8U addr)
{	
	INT8U	err;
	
//	OSSemPend(FRAMSem,0,&err);
	addr = addr << 1  ; 
	addr &= 0x7e ; 
	
	CLR_SSPCSRF() ;
	ddelay(5) ;
	FMSendByte(addr);

	FMSendByte(dataptr);

	SET_SSPCSRF();
	ddelay(5) ;
	
//	OSSemPost(FRAMSem);
}
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
void	FMRead(INT8U *dataptr, INT8U addr)
{	
	INT8U	err;
	
//	OSSemPend(FRAMSem,0,&err);
	addr = addr << 1  ; 
	addr |= 0x80 ; 

	CLR_SSPCSRF() ;
	ddelay(5) ;
	
	FMSendByte(addr);
	
	*dataptr = FMReadByte();
	
	SET_SSPCSRF();
	ddelay(5) ;

//	OSSemPost(FRAMSem);
}


///////////////////////////////////////////////////////////////////////
// IC��������
///////////////////////////////////////////////////////////////////////
void cmd_process(void)
{
    INT8U cmd;
    INT8U status;
	
	cmd = FMRevBuffer[0];
	switch(cmd)
	{
		case 1:     // Halt the card     //��ֹ���Ĳ���
			status=FMMIF_Halt();			
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;
			break;			
		case 2:     
        	//MIF_Halt();     //RF_CMD_REQUEST_ALL=0x52, all
        	status = FMRequest(RF_CMD_REQUEST_ALL);	//RF_CMD_REQUEST_STD=0x26,  Idle
        	if(status != FM1702_OK)
        	{
        	    status = FMRequest(RF_CMD_REQUEST_ALL);		
        	    if(status != FM1702_OK)
        	    {
        	        FMRevBuffer[0] = 1;
        	        FMRevBuffer[1] = FM1702_REQERR;
        	        break;
        	    }
        	}
			if(tagtype[0]==2)
				cardtype=mifarepro;     // Mifare Pro ��
			else if(tagtype[0]==4)
				cardtype=mifare1;       // Mifare One ��
			else if(tagtype[0]==16)
				cardtype=mifarelight;   // Mifare Light ��
			else
				cardtype=unknowncard;

			FMRevBuffer[0]=3;	
			FMRevBuffer[1]=status;
			FMRevBuffer[2]=tagtype[0];
			FMRevBuffer[3]=tagtype[1];
			break;
	    case 3:                         // ����ͻ ������ϵ�к� MLastSelectedSnr
			status = FMAntiColl();;
			if(status!=FM1702_OK)
			{
				FMRevBuffer[0]=1;	
				FMRevBuffer[1]=FM1702_ANTICOLLERR;
				break;
			}
			//memcpy(MLastSelectedSnr,&RevBuffer[2],4);
			FMRevBuffer[0]=5;
			FMRevBuffer[1]=status;
			FMRevBuffer[2]=UID[0];
			FMRevBuffer[3]=UID[1];
			FMRevBuffer[4]=UID[2];
			FMRevBuffer[5]=UID[3];
			break;	
		case 4:		                    // ѡ�� Select Card
			status = FMSelect_Card();
			if(status!=FM1702_OK)
			{
				FMRevBuffer[0]=1;	
				FMRevBuffer[1]=FM1702_SELERR;
				break;
			}
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;			
			break;
        case 6:                        //������Կ
			status = FMLoad_keyE2_CPY(FMRevBuffer[2],FMRevBuffer[1]);   //%40
			status = FMAuthentication(UID, FMRevBuffer[2], FMRevBuffer[1]);	
        	if(status != FM1702_OK)
        	{
        		FMRevBuffer[0]=1;
			    FMRevBuffer[1]=status;
			    break;
        	}
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;			
			break;	
		case 8:                       //����
			status=FMMIF_READ(&FMRevBuffer[2],FMRevBuffer[1]);
			if(status != FM1702_OK)
			{
			    FMRevBuffer[0]=1;
			    FMRevBuffer[1]=status;
			    break;
			}
			else
			{
				if(cardtype==mifare1||cardtype==mifarepro)
					FMRevBuffer[0]=17;
				else if(cardtype==1)
					FMRevBuffer[0]=9;
				else
					FMRevBuffer[0]=16;
			}
			FMRevBuffer[1]=status;			
			break;	
		case 9:                  //д��
			status=FMMIF_Write(&FMRevBuffer[2],FMRevBuffer[1]);
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=status;			
			break;	
		case 10:                //��ֵ��ֵ
			if(FMRevBuffer[1] == RF_CMD_INC)
			{
			    status = FMMIF_Increment(&FMRevBuffer[3],FMRevBuffer[2]);
			    FMMIF_Transfer(FMRevBuffer[2]);
			}
			else if(FMRevBuffer[1] == RF_CMD_DEC)
			{
			    status = FMMIF_Decrement(&FMRevBuffer[3],FMRevBuffer[2]);   
			    FMMIF_Transfer(FMRevBuffer[2]);
			}
			else
			{
			    status = 1;
			}
			FMRevBuffer[0]=1;	
			FMRevBuffer[1]=status;
			break;
		case 12:    
			FMRevBuffer[0]=1;
			FMRevBuffer[1]=0;
			break;
		default:
		    FMRevBuffer[0] = 1;
		    FMRevBuffer[1] = 1;
		    break;	
	}
}


/****************************************************************/
/*����: Clear_FIFO */
/*����: �ú���ʵ�����FM1702��FIFO������*/
/*����: N/A */
/*���: TRUE, FIFO�����*/
/* FALSE, FIFOδ�����*/
/****************************************************************/
BOOLEAN FMClear_FIFO(void)
{
	INT8U	temp;
	INT32U	i;
	
	FMRead(&temp , FM_Control);		        /* ���FIFO */
	temp = (temp | 0x01);
	FMWrite(temp, FM_Control);

	for(i = 0; i < RF_TimeOut; i++) 		/* ���FIFO�Ƿ���� */
	{
		FMRead(&temp , FM_FIFO_Length );	/* ���FIFO */
		if(temp == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

/****************************************************************/
/*����: Write_FIFO */
/*����: �ú���ʵ����FM1702��FIFO��д��x bytes����*/
/*����: count, ��д���ֽڵĳ���*/
/* buff, ָ���д�����ݵ�ָ��*/
/*���: N/A */
/****************************************************************/
void FMWrite_FIFO(INT8U count, INT8U *buff)
{
	for(; count > 0 ; count --)
	{
		FMWrite(*buff++ , FM_FIFO );
	}
}

/****************************************************************/
/*����: Read_FIFO */
/*����: �ú���ʵ�ִ�FM1702��FIFO�ж���x bytes����*/
/*����: buff, ָ��������ݵ�ָ��*/
/*���: N/A */
/****************************************************************/
INT8U FMRead_FIFO(INT8U *buff)
{
	INT8U temp  ;

	FMRead(&temp , FM_FIFO_Length );	

	if(temp == 0)
	{
		return 0;
	}
	else if (temp >= 24)		
	{
		temp = 24;	
	}

	for(; temp > 0 ; temp --)
	{
		FMRead(buff++ , FM_FIFO );	
	}

	return temp;
}

/****************************************************************/
/*����: Judge_Req */
/*����: �ú���ʵ�ֶԿ�Ƭ��λӦ���źŵ��ж�*/
/*����: *buff, ָ��Ӧ�����ݵ�ָ��*/
/*���: TRUE, ��ƬӦ���ź���ȷ*/
/* FALSE, ��ƬӦ���źŴ���*/
/****************************************************************/
BOOLEAN FMJudge_Req(INT8U *buff)
{
	INT8U temp1, temp2;
	
	temp1 = *buff;
	temp2 = *(buff + 1);

	if((temp1 == 0x02) || (temp1 == 0x04) || (temp1 == 0x05) || (temp1 == 0x53) || (temp1 == 0x03))
	{
		if (temp2 == 0x00)
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************/
/*����: Check_UID */
/*����: �ú���ʵ�ֶ��յ��Ŀ�Ƭ�����кŵ��ж�*/
/*����: N/A */
/*���: TRUE: ���к���ȷ*/
/* FALSE: ���кŴ���*/
/****************************************************************/
BOOLEAN FMCheck_UID(void)
{
	
	INT8U	temp;
	INT8U	i;
	
	temp = 0x00;
	for(i = 0; i < 5; i++)
	{
		temp = temp ^ UID[i];
	}

	if(temp == 0)
	{
		return TRUE;
	}

	return FALSE;
}

/****************************************************************/
/*����: Save_UID */
/*����: �ú���ʵ�ֱ��濨Ƭ�յ������к�*/
/*����: row: ������ͻ����*/
/* col: ������ͻ����*/
/* length: �Ӆ�����UID���ݳ���*/
/*���: N/A */
/****************************************************************/
void FMSave_UID(INT8U row, INT8U col, INT8U length)
{
	INT8U	i;
	INT8U	temp;
	INT8U	temp1;
	
	if((row == 0x00) && (col == 0x00))
	{
		for(i = 0; i < length; i++)
		{
			UID[i] = FMRevBuffer[i];
		}
	}
	else
	{
		temp  = FMRevBuffer[0];
		temp1 = UID[row - 1];
		switch(col)
		{
		case 0:		temp1 = 0x00; row = row + 1;              break;
		case 1:		temp = temp & 0xFE; temp1 = temp1 & 0x01; break;
		case 2:		temp = temp & 0xFC; temp1 = temp1 & 0x03; break;
		case 3:		temp = temp & 0xF8; temp1 = temp1 & 0x07; break;
		case 4:		temp = temp & 0xF0; temp1 = temp1 & 0x0F; break;
		case 5:		temp = temp & 0xE0; temp1 = temp1 & 0x1F; break;
		case 6:		temp = temp & 0xC0; temp1 = temp1 & 0x3F; break;
		case 7:		temp = temp & 0x80; temp1 = temp1 & 0x7F; break;
		default:	break;
		}

		FMRevBuffer[0] = temp;
		UID[row - 1] = temp1 | temp;
		for(i = 1; i < length; i++)
		{
			UID[row - 1 + i] = FMRevBuffer[i];
		}
	}
}

/****************************************************************/
/*����: Set_BitFraming */
/*����: �ú������ô��������ݵ��ֽ���*/
/*����: row: ������ͻ����*/
/* col: ������ͻ����*/
/*���: N/A */
/****************************************************************/
void FMSet_BitFraming(INT8U row, INT8U col)
{
	INT8U temp ; 
	
	switch(row)
	{
		case 0:		FMRevBuffer[1] = 0x20; break;
		case 1:		FMRevBuffer[1] = 0x30; break;
		case 2:		FMRevBuffer[1] = 0x40; break;
		case 3:		FMRevBuffer[1] = 0x50; break;
		case 4:		FMRevBuffer[1] = 0x60; break;
		default:	break;
	}

	switch(col)
	{
		case 0:		temp = 0x00 ; break;
		case 1:		temp = 0x11 ; break;
		case 2:		temp = 0x22 ; break;
		case 3:		temp = 0x33 ; break;
		case 4:		temp = 0x44 ; break;
		case 5:		temp = 0x55 ; break;
		case 6:		temp = 0x66 ; break;
		case 7:		temp = 0x77 ; break;
		default:	break;
	}

	FMWrite(temp, FM_Bit_Frame); 
	FMRevBuffer[1] = (FMRevBuffer[1] | col);
}

/****************************************************************/
/*����: Init_FM1702 */
/*����: �ú���ʵ�ֶ�FM1702��ʼ������*/
/*����: mode:����ģʽ, 0:TYPEAģʽ*/
/* 1:TYPEBģʽ*/
/* 2:�Ϻ�ģʽ*/
/*���: N/A */
/****************************************************************/
void FMInit_FM1702(void )
{
	
	INT8U  temp ;

	//FIO1DIR3 |= FMRST ; 
	//LPC_GPIO1->DIR |= FMRST;
	
	SET_FMRST() ;			            	/* FM1702��λ */
	OSTimeDly(OS_TICKS_PER_SEC/5);										// ɨ����
//	OSTimeDly(100); 						/*�ȴ�Լ140ms, 11.0592*/
	CLR_FMRST() ;
	OSTimeDly(OS_TICKS_PER_SEC/5);										// ɨ����
//	OSTimeDly(100);							/*�ȴ�Լ140ms, 11.0592*/
	for( temp = 1 ; temp != 0 ;)			/* �ȴ�Command = 0,FM1702��λ�ɹ� */
	{
		FMRead(&temp , FM_Command );
	} 					

	FMWrite( HEX_0x80 , FM_Page_Sel);

	for(temp = HEX_0x80 ; temp & HEX_0x80 ; ) /* ��ʱ */
	{
		FMRead(&temp , FM_Command );
	}

	FMWrite(HEX_0x00 , FM_Page_Sel);		//��ʼ�� SPI ���
	
	FMWrite(0x0b , FM_TimerClock  );		//address 2AH    /* ��ʱ���������üĴ��� */
	FMWrite(0x02 , FM_TimerControl  );		//address 2BH	 /* ��ʱ�����ƼĴ��� */
	FMWrite(0x42 , FM_TimerReload );		//address 2CH    /* ��ʱ����ֵ�Ĵ��� */
	FMWrite(0x7f , FM_InterruptEn );   		//address 06H    /* �ж�ʹ��/��ֹ�Ĵ��� */ 
	FMWrite(0x7f , FM_Int_Req  );			//address 07H    /* �ж������ʶ�Ĵ��� */
	FMWrite(0x02 , FM_MFOUTSelect  );		//address 26H    /* mf OUT ѡ�����üĴ��� */  
	FMWrite(0x5b , FM_TxControl  );			//���õ�����������ԴΪ�ڲ�������, ��������TX1��TX2
	FMWrite(0x01 , FM_RxControl2 );
	FMWrite(0x05 , FM_RxWait  );

	FMWrite(0x00 , FM_TypeSH  );

}

/****************************************************************/
/*����: Command_Send */
/*����: �ú���ʵ����FM1702��������Ĺ���*/
/*����: count, ����������ĳ���*/
/* buff, ָ����������ݵ�ָ��*/
/* Comm_Set, ������*/
/*���: TRUE, �����ȷִ��*/
/* FALSE, ����ִ�д���*/
/****************************************************************/
BOOLEAN FMCommand_Send(INT8U count, INT8U *buff, INT8U Comm_Set)
{

	INT32U		j;
	INT8U	temp, temp1;
	
	FMWrite(0x00 , FM_Command );
	temp = FMClear_FIFO();
	FMWrite_FIFO(count,buff);

	FMWrite(Comm_Set , FM_Command  );			/* ����ִ�� */

	for(j = 0; j < RF_TimeOut; j++) 			/* �������ִ�з� */
	{
		FMRead(&temp , FM_Command );
		FMRead(&temp1 , FM_Int_Req );
		temp1 = temp1 & 0x80;
		if((temp == 0x00) || (temp1 == 0x80))
		{
			return TRUE;
		}
	}

	return FALSE;
}
/****************************************************************/
/*����:  */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵĿ�Ƭ�Ĳ���*/
/*����: mode: ALL(�������FM1702������Χ֮�ڵĿ�Ƭ) */
/* STD(�����FM1702������Χ֮�ڴ���HALT״̬�Ŀ�Ƭ) */
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_OK: Ӧ����ȷ*/
/* FM1702_REQERR: Ӧ�����*/
/****************************************************************/
INT8U FMRequest(INT8U mode)
{
	
	INT8U temp;
	
	FMWrite(0x63, FM_CRCPresetLSB );		//CRCPresetLSB address is 0x23
	FMWrite(0x3f, FM_CWConductance);

	FMRevBuffer[0] = mode;		            /* ģʽѡ�� */

	FMWrite(0x07 , FM_Bit_Frame );		
	FMWrite(0x03 , FM_ChannelRedundancy);	
	FMRead(&temp , FM_Control );
	temp = temp & (0xf7);
	FMWrite(temp, FM_Control );			    //Control reset value is 00

	temp = FMCommand_Send(1, FMRevBuffer, Transceive);   //Transceive=0x1E	/* ���ͽ������� */
	if(temp == FALSE)
	{
		return FM1702_NOTAGERR;
	}

	FMRead_FIFO(FMRevBuffer);		    /* ��FIFO�ж�ȡӦ����Ϣ��RevBuffer[]�� */
	temp = FMJudge_Req(FMRevBuffer);	/* �ж�Ӧ���ź��Ƿ���ȷ */
	if(temp == TRUE)
	{
		tagtype[0] = FMRevBuffer[0];
		tagtype[1] = FMRevBuffer[1];
		return FM1702_OK;
	}

	return FM1702_REQERR;
}

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
BOOLEAN FMRead_E2(INT8U lsb, INT8U msb, INT8U count, INT8U *buff)
{
	INT8U	temp;
	
	*buff = lsb;
	*(buff + 1) = msb;
	*(buff + 2) = count;
	temp = FMCommand_Send(3, buff, ReadE2);
	FMRead_FIFO(buff);
	if(temp == FALSE) return(TRUE);
	return(FALSE);
}

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
BOOLEAN FMWrite_E2(INT8U lsb, INT8U msb, INT8U count, INT8U *buff)
{
	INT8U temp, i;

	for(i = 0; i < count; i++){
		*(buff + count - i + 2) = *(buff - i + count);
	}

	*buff = lsb;
	*(buff + 1) = msb;
	temp = FMCommand_Send(count + 2, buff, WriteE2);
	FMRead(&temp , FM_SecondaryStatus);
	temp = temp & 0x40;
	if(temp == 0x40){
		FMWrite(0x00, FM_Command );			//added 2006/03/15
		return TRUE;
	}

	return FALSE;
}

/****************************************************************/
/*����: MIF_Halt */
/*����: �ú���ʵ����ͣMIFARE��*/
/*����: N/A */
/*���: FM1702_OK: Ӧ����ȷ*/
/* FM1702_PARITYERR: ��żУ���*/
/* FM1702_CRCERR: CRCУ���*/
/* FM1702_NOTAGERR: �޿�*/
/****************************************************************/
INT8U FMMIF_Halt(void)
{
	INT8U	temp;
	INT32U	i;
	
	FMWrite(0x63, FM_CRCPresetLSB );
	FMWrite(0x3f, FM_CWConductance );
	FMWrite(0x03, FM_ChannelRedundancy);
	*FMRevBuffer = RF_CMD_HALT;
	*(FMRevBuffer + 1) = 0x00;
	temp = FMCommand_Send(2, FMRevBuffer, Transmit);
	if(temp == TRUE){
		for(i = 0; i < 0x50; i++){
		}
		return FM1702_OK;
	}
	else{
		FMRead(&temp , FM_ErrorFlag);
		if((temp & 0x02) == 0x02){
			return(FM1702_PARITYERR);
		}

		if((temp & 0x04) == 0x04){
			return(FM1702_FRAMINGERR);
		}

		return(FM1702_NOTAGERR);
	}
}

///////////////////////////////////////////////////////////////////////
// ת����Կ��ʽ
///////////////////////////////////////////////////////////////////////
INT8U FMM500HostCodeKey(  INT8U *uncoded, INT8U *coded)   
{
//    INT8U status = FM1702_OK;
    INT8U cnt = 0;
    INT8U ln  = 0;     
    INT8U hn  = 0;      
    for (cnt = 0; cnt < 6; cnt++){
        ln = uncoded[cnt] & 0x0F;
        hn = uncoded[cnt] >> 4;
        coded[cnt * 2 + 1] = (~ln << 4) | ln;
        coded[cnt * 2 ] = (~hn << 4) | hn;
    }
    return FM1702_OK;
}

/****************************************************************/
/*����: Load_keyE2 */
/*����: �ú���ʵ�ְ�E2���������FM1702��keyRevBuffer��*/
/*����: Secnr: EE��ʼ��ַ*/
/*���: True: ��Կװ�سɹ�*/
/* False: ��Կװ��ʧ��*/
/****************************************************************/
BOOLEAN FMLoad_keyE2_CPY(INT8U Secnr, INT8U Mode)
{

//	INT8U status=0;
	INT8U	temp=0;	
    INT8U	msb = 0;
    INT8U	lsb = 0;
	INT8U coded_keys[12];

	INT8U	temp1=0;
	if(Secnr >= 0x20){
	  temp1 = Secnr - 0x20;
	  Secnr = 0x20 + temp1 * 4;
	}

	temp = Secnr * 12;

	if(Mode == RF_CMD_AUTH_LA){
		if(temp >= 0x80){
			lsb = temp - 0x80;
			msb = 0x01;
		}
		else{
			lsb = temp + 0x80;
			msb = 0x00;
		}
	}
	else{
		msb = 0x01;
		lsb = temp + 0x40;
	}
	FMRevBuffer[3] = 0xff ; 
	FMRevBuffer[4] = 0xff ; 
	FMRevBuffer[5] = 0xff ; 
	FMRevBuffer[6] = 0xff ; 
	FMRevBuffer[7] = 0xff ; 
	FMRevBuffer[8] = 0xff ; 

	FMM500HostCodeKey(&FMRevBuffer[3],coded_keys);////////////////
//	status = Write_E2(lsb, msb, 12, coded_keys);	     ////////////////
//	RevBuffer[0] = lsb;
//	RevBuffer[1] = msb;
//	
//	FMRevBuffer[0] = 0X80;
//	FMRevBuffer[1] = 0 ;
//	temp = FMCommand_Send(2, FMRevBuffer, LoadKeyE2); //ע�͵�������Ϊ�ô��Ǵ�EEPROM������Կ

//	FMCommand_Send(12,coded_keys,LoadKey); //��FIFO������Կ ���޸Ĺ�ѧ�� 09��6��2��

	msb = lsb;
	lsb = msb;

	FMCommand_Send(12,coded_keys,LoadKey); //��FIFO������Կ ���޸Ĺ�ѧ�� 09��6��2��
	FMRead(&temp , FM_ErrorFlag) ;
	if((temp & 0x40) == 0x40){
		return FALSE;
	}

	return TRUE;
}


/****************************************************************/
/*����: AntiColl */
/*����: �ú���ʵ�ֶԷ���FM1702������Χ֮�ڵĿ�Ƭ�ķ���ͻ���*/
/*����: N/A */
/*���: FM1702_NOTAGERR: �޿�*/
/* FM1702_BYTECOUNTERR: �����ֽڴ���*/
/* FM1702_SERNRERR: ��Ƭ���к�Ӧ�����*/
/* FM1702_OK: ��ƬӦ����ȷ*/
/****************************************************************/
INT8U FMAntiColl(void)
{
	INT8U	temp;
	INT8U	i;
	INT8U	row, col;
	INT8U	pre_row;
	
	row = 0;
	col = 0;
	pre_row = 0;
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance );

	FMRevBuffer[0] = RF_CMD_ANTICOL;
	FMRevBuffer[1] = 0x20;
	FMWrite(0x03 , FM_ChannelRedundancy  );
	temp = FMCommand_Send(2, FMRevBuffer, Transceive);
	while(1)
	{
		if(temp == FALSE){
			return(FM1702_NOTAGERR);
		}

		//temp = ErrorFlag;

		FMRead(&temp , FM_FIFO_Length  );
		if(temp == 0){
			return FM1702_BYTECOUNTERR;
		}

		FMRead_FIFO(FMRevBuffer);
		FMSave_UID(row, col, temp);			/* ���յ���UID����UID������ */
		
		
		FMRead(&temp , FM_ErrorFlag );				    /* �жϽӅ������Ƿ���� */
		temp = temp & 0x01;
		if(temp == 0x00){
			temp = FMCheck_UID();			/* У���յ���UID */
			if(temp == FALSE){
				return(FM1702_SERNRERR);
			}

			return(FM1702_OK);
		}
		else{
			FMRead(&temp , FM_CollPos);				/* ��ȡ��ͻ���Ĵ��� */
			row = temp / 8;
			col = temp % 8;
			FMRevBuffer[0] = RF_CMD_ANTICOL;
			FMSet_BitFraming(row + pre_row, col);	/* ���ô��������ݵ��ֽ��� */
			pre_row = pre_row + row;
			for(i = 0; i < pre_row + 1; i++){
				FMRevBuffer[i + 2] = UID[i];
			}

			if(col != 0x00){
				row = pre_row + 1;
			}
			else{
				row = pre_row;
			}

			temp = FMCommand_Send(row + 2, FMRevBuffer, Transceive);
		}
	}
}

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
INT8U FMSelect_Card(void)
{
	INT8U	temp, i;
	
	FMWrite(0x63 , FM_CRCPresetLSB );
	FMWrite(0x3f , FM_CWConductance );
	FMRevBuffer[0] = RF_CMD_SELECT;
	FMRevBuffer[1] = 0x70;
	for(i = 0; i < 5; i++){
		FMRevBuffer[i + 2] = UID[i];
	}

	FMWrite(0x0f , FM_ChannelRedundancy );	
	temp = FMCommand_Send(7, FMRevBuffer, Transceive);
	if(temp == FALSE){
		return(FM1702_NOTAGERR);
	}
	else{
		FMRead(&temp , FM_ErrorFlag );
		if((temp & 0x02) == 0x02) return(FM1702_PARITYERR);
		if((temp & 0x04) == 0x04) return(FM1702_FRAMINGERR);
		if((temp & 0x08) == 0x08) return(FM1702_CRCERR);
		FMRead(&temp , FM_FIFO_Length);
		if(temp != 1) return(FM1702_BYTECOUNTERR);
		FMRead_FIFO(FMRevBuffer);	/* ��FIFO�ж�ȡӦ����Ϣ */
		temp = *FMRevBuffer;

		if((temp == 0x18) || (temp == 0x08) || (temp == 0x88) || (temp == 0x53))	/* �ж�Ӧ���ź��Ƿ���ȷ */
			return(FM1702_OK);
		else
			return(FM1702_SELERR);
	}
}

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
INT8U FMAuthentication(INT8U *UID, INT8U SecNR, INT8U mode)
{
	INT8U	i;
	INT8U	temp, temp1;

	INT8U temp0;
	
	if(SecNR >= 0x20){
		temp0 = SecNR -0x20;
		SecNR = 0x20 + temp0 * 4;
	}
	
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	if(mode == RF_CMD_AUTH_LB)			
		FMRevBuffer[0] = RF_CMD_AUTH_LB;
	else
		FMRevBuffer[0] = RF_CMD_AUTH_LA;
	FMRevBuffer[1] = SecNR * 4 + 3;
	for(i = 0; i < 4; i++){
		FMRevBuffer[2 + i] = UID[i];
	}

	FMWrite(0x0f , FM_ChannelRedundancy);	
	temp = FMCommand_Send(6, FMRevBuffer, Authent1);
	if(temp == FALSE){
		return 0x99;
	}

	FMRead(&temp , FM_ErrorFlag );             //ErrorFlag address is 0x0A.
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	temp = FMCommand_Send(0, FMRevBuffer, Authent2);	
	if(temp == FALSE){
		return 0x88;
	}

	FMRead(&temp , FM_ErrorFlag );
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	FMRead(&temp1 , FM_Control );
	temp1 = temp1 & 0x08;	
	if(temp1 == 0x08){
		return FM1702_OK;
	}

	return FM1702_AUTHERR;
}

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
INT8U FMMIF_READ(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	
	FMWrite(0x63 , FM_CRCPresetLSB );
	FMWrite(0x3f , FM_CWConductance );
	FMWrite(0x3f , FM_ModConductance );
	FMWrite(0x0f , FM_ChannelRedundancy );

	buff[0] = RF_CMD_READ;
	buff[1] = Block_Adr;
	temp = FMCommand_Send(2, buff, Transceive);
	if(temp == 0){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_ErrorFlag );
	if((temp & 0x02) == 0x02) return FM1702_PARITYERR;
	if((temp & 0x04) == 0x04) return FM1702_FRAMINGERR;
	if((temp & 0x08) == 0x08) return FM1702_CRCERR;
	FMRead(&temp , FM_FIFO_Length);
	if(temp == 0x10){
		FMRead_FIFO(buff);
		return FM1702_OK;
	}
	else if(temp == 0x04){
		FMRead_FIFO(buff);
		return FM1702_OK;
	}
	else{
		return FM1702_BYTECOUNTERR;
	}
}

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
INT8U FMMIF_Write(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	INT8U	*F_buff;

	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance );
	F_buff = buff + 0x10;
	FMWrite(0x07 , FM_ChannelRedundancy );    /* Note: this line is for 1702, different from RC500*/
	*F_buff = RF_CMD_WRITE;
	*(F_buff + 1) = Block_Adr;
	temp = FMCommand_Send(2, F_buff, Transceive);
	if(temp == FALSE){
		return(FM1702_NOTAGERR);
	}

	FMRead(&temp , FM_FIFO_Length );
	if(temp == 0){
		return(FM1702_BYTECOUNTERR);
	}

	FMRead_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
    	case 0x00:	return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	break;
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_WRITEERR);
	}

	temp = FMCommand_Send(16, buff, Transceive);
	if(temp == TRUE){
		return(FM1702_OK);
	}
	else{
		FMRead(&temp , FM_ErrorFlag  );
		if((temp & 0x02) == 0x02)
			return(FM1702_PARITYERR);
		else if((temp & 0x04) == 0x04)
			return(FM1702_FRAMINGERR);
		else if((temp & 0x08) == 0x08)
			return(FM1702_CRCERR);
		else
			return(FM1702_WRITEERR);
	}
}

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
INT8U FMMIF_Increment(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	INT8U	*F_buff;
	
	FMWrite(0x63 , FM_CRCPresetLSB );
	FMWrite(0x3f , FM_CWConductance );
	F_buff = buff + 4;
	*F_buff = RF_CMD_INC;
	*(F_buff + 1) = Block_Adr;
	FMWrite(0x07 , FM_ChannelRedundancy);
	temp = FMCommand_Send(2, F_buff, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length);
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
        case 0x00:	/* break; */return(FM1702_NOTAUTHERR);
        case 0x04:	return(FM1702_EMPTY);
        case 0x0a:	break;
        case 0x01:	return(FM1702_CRCERR);
        case 0x05:	return(FM1702_PARITYERR);
        default:	return(FM1702_INCRERR);
	}
        
	temp = FMCommand_Send(4, buff, Transmit);
	if(temp == FALSE){
		return FM1702_INCRERR;
	}

	return FM1702_OK;
}

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
INT8U FMMIF_Decrement(INT8U *buff, INT8U Block_Adr)
{
	INT8U		temp;
	INT8U	*F_buff;
	
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	F_buff = buff + 4;
	*F_buff = RF_CMD_DEC;
	*(F_buff + 1) = Block_Adr;
	FMWrite(0x07 , FM_ChannelRedundancy);
	temp = FMCommand_Send(2, F_buff, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length );
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(F_buff);
	temp = *F_buff;
	switch(temp)
	{
    	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	break;
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_DECRERR);
	}

	temp = FMCommand_Send(4, buff, Transmit);
	if(temp == FALSE){
		return(FM1702_DECRERR);
	}

	return FM1702_OK;
}

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
INT8U FMMIF_Restore(INT8U Block_Adr)
{
	INT8U	temp, i;
	
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	FMWrite(0x07 , FM_ChannelRedundancy);
	*FMRevBuffer = RF_CMD_RESTORE;
	*(FMRevBuffer + 1) = Block_Adr;
	temp = FMCommand_Send(2, FMRevBuffer, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length  );
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(FMRevBuffer);
	temp = *FMRevBuffer;
	switch(temp)
	{
    	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	break;
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_RESTERR);
	}

	for(i = 0; i < 4; i++) FMRevBuffer[i] = 0x00;
	temp = FMCommand_Send(4, FMRevBuffer, Transmit);
	if(temp == FALSE){
		return FM1702_RESTERR;
	}

	return FM1702_OK;
}

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
INT8U FMMIF_Transfer(INT8U Block_Adr)
{
	
	INT8U	temp;
	FMWrite(0x63 , FM_CRCPresetLSB);
	FMWrite(0x3f , FM_CWConductance);
	FMWrite(0x07 , FM_ChannelRedundancy);
	FMRevBuffer[0] = RF_CMD_TRANSFER;
	FMRevBuffer[1] = Block_Adr;
	temp = FMCommand_Send(2, FMRevBuffer, Transceive);
	if(temp == FALSE){
		return FM1702_NOTAGERR;
	}

	FMRead(&temp , FM_FIFO_Length );
	if(temp == 0){
		return FM1702_BYTECOUNTERR;
	}

	FMRead_FIFO(FMRevBuffer);
	temp = *FMRevBuffer;
	switch(temp)
	{
    	case 0x00:	/* break; */return(FM1702_NOTAUTHERR);	
    	case 0x04:	return(FM1702_EMPTY);
    	case 0x0a:	return(FM1702_OK);
    	case 0x01:	return(FM1702_CRCERR);
    	case 0x05:	return(FM1702_PARITYERR);
    	default:	return(FM1702_TRANSERR);
	}
}

/****************************************************************/
/*����: HL_Active */
/*����: �ú���ʵ�ָ߼�MIFARE����������*/
/*����: Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���: ����״̬��*/
/* �������ݴ���RevBuffer��*/
/****************************************************************/
INT8U FMHL_Active(INT8U Block_Adr, INT8U Mode)
{
	INT8U	temp ;
	
	Secnr = Block_Adr / 4;
	FMMIF_Halt();	/* Halt */
	temp = FMRequest(RF_CMD_REQUEST_STD);		
	if(temp != FM1702_OK){
		return(FM1702_REQERR);
	}

	temp = FMAntiColl();				
	if(temp != FM1702_OK){
		return(FM1702_ANTICOLLERR);
	}

	temp = FMSelect_Card();				
	if(temp != FM1702_OK){
		return(FM1702_SELERR);
	}

	FMLoad_keyE2_CPY(Secnr, Mode);	//%40	
	temp = FMAuthentication(UID, Secnr, Mode);	
	if(temp != FM1702_OK){
		return(FM1702_AUTHERR);
	}

	return FM1702_OK;
}

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
INT8U FMMIF_Initival(INT8U *buff, INT8U Block_Adr)
{
	INT8U	temp;
	INT8U		i;
	
	for(i = 0; i < 4; i++){
		*(buff + 4 + i) = ~(*(buff + i));
	}

	for(i = 0; i < 4; i++){
		*(buff + 8 + i) = *(buff + i);
	}

	*(buff + 12) = Block_Adr;
	*(buff + 13) = ~Block_Adr;
	*(buff + 14) = Block_Adr;
	*(buff + 15) = ~Block_Adr;
	temp = FMMIF_Write(buff, Block_Adr);
	return temp;
}

/****************************************************************/
/*����: HL_Read */
/*����: �ú���ʵ�ָ߼�������*/
/*����: Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���: ����״̬��*/
/* �������ݴ���RevBuffer��*/
/****************************************************************/
INT8U FMHL_Read(INT8U *buff, INT8U Block_Adr, INT8U Mode)
{	
	INT8U	temp;
	
	temp = FMHL_Active(Block_Adr, Mode);
	if(temp != FM1702_OK)
		return temp;
	
	temp = FMMIF_READ(&(*buff), Block_Adr);
	if(temp != FM1702_OK)
		return temp;

	return FM1702_OK;
}

/****************************************************************/
/*����: HL_Write */
/*����: �ú���ʵ�ָ߼�д����*/
/*����: buff: ��д�����ݵ��׵�ַ*/
/* Secnr: ������*/
/* Block_Adr: ���ַ*/
/*���:����״̬��*/
/****************************************************************/
INT8U FMHL_Write(INT8U *buff, INT8U Block_Adr, INT8U Mode)
{
	
	INT8U	temp;
//	INT8U xbyte[16] = {0,0x10,0,0,1,2,3,4,5,6,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa} ; 
	

	temp = FMHL_Active(Block_Adr, Mode);
	if(temp != FM1702_OK){
		return temp;
	}

//	temp = FMMIF_Write(xbyte, Block_Adr);
	
	temp = FMMIF_Write(buff, Block_Adr);
	if(temp != FM1702_OK){
		return FM1702_WRITEERR;
	}

//	temp = FMMIF_READ(&(*buff), Block_Adr);
//	if(temp != FM1702_OK)
//	{
//		return temp;
//	}

	return FM1702_OK;
}


void main_rfic( void )
{
	do {
   		OSTimeDly(OS_TICKS_PER_SEC/10);
	}while(1);
}

/***********************************************************************************************
* Function		: HextoAscii
* Description	: 16������ת��ΪASCII��
* Input			: 
* Output		: 
* Note(s)		: ע�⣬ת����ascii���������ʮ���ֽ�
* Contributor	: 10/12/2010	wangyao
***********************************************************************************************/
//void  HextoAscii(uint32 hexdata,uint8 *data)
//{
//	uint32 temp[10]={0};
//	uint32 hex;
//	uint8 i,j,k;
//	uint32 chushu=10;
////	INT32U yushu=0;
//	hex=hexdata	;
//	if(hex<=0xffffffff)
//	{
//		for(i=0;i<10;i++)
//		{
//			chushu=10;
//			if(i==9)
//			{
//				for(j=0;j<i-1;j++)
//					chushu*=10;
//			    temp[9-i] = hex/chushu;
//				temp[9-i]+='0';	
//			}
//			else
//			{
//				
//				for(j=0;j<i;j++)
//					chushu*=10;
//				temp[9-i] = hex%((INT32U)chushu);
//				hex-=temp[9-i];
//				for(k=0;k<i;k++)
//					temp[9-i]=temp[9-i]/10;
//				temp[9-i]+='0';
//			}
//		}
//	}
//	for(i=0;i<10;i++)
//		data[i]=(INT8U)temp[i];
//}
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
//uint8	CompareArray(uint8 *pt1, uint8 *pt2, uint8 length)
//{
//	for(;length != 0; length--)
//	{
//		if(*pt1++ != *pt2++)
//		{
//			return 0;
//		}
//	}
//	return 1;
//}
/***********************************************************************************************
* Function		: BSP_SaveNewKey
* Description	: ���������õ�����
* Input			: addr---�����ַ
                  KEY---��Կ
* Output		: 1--�ɹ�  0--���ɹ�
* Note(s)		: 
* Contributor	: 17/06/2010	wangyao
***********************************************************************************************/
INT8U SavePassword(const INT8U *KEY)
{ 
	INT32U addr=0x80;//������Ƶ����棬�����ⲿʹ����Ա�����ʲô��˼
	INT8U  psw[12] ; 

	FM1702SLHostCodeKey(KEY,psw);

	if(FMWrite_E2(addr , 0x00 , 12 , psw ))
		return 1 ; 
	else 
	    return 0 ;  
}

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
void FM1702SLHostCodeKey(const INT8U *uncoded, INT8U *coded)   
{
	INT8U  cnt = 0;
	INT8U  ln  = 0;     // low nibble
	INT8U  hn  = 0;     // high nibble
	for (cnt = 0; cnt < 6; cnt++)
	{
		ln = uncoded[cnt] & 0x0F;  //ȡ����λ
		hn = uncoded[cnt] >> 4;    //ȡ����λ 
		coded[cnt * 2 + 1] = ((~ln) << 4) | ln;
		coded[cnt * 2 ] = ((~hn) << 4) | hn;
	}
}

//////////////////////////////////////////////////////////////////////////////
// End of File
//////////////////////////////////////////////////////////////////////////////

