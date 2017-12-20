/****************************************Copyright (c)****************************************************
**                            ShangHai FuDian Technology Co.,LTD.
**
**                                 http://www.telluspower.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                  PTL698.h
** Latest modified Date:       2014-5-20
** Latest Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:                 AndyLau
** Created date:               2014-5-20
** Version:
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Modified by:                
** Modified date:              2014-5-20
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include 	"..\config.h"

Struct_UpProgram 	Struct_UpProgram_True;
GX_DAT_FRM 			Tmp_Frm_GX;							   	//���ݻ���
DATE				CurDATA;								//��ǰ����
TIME				CurTIME;								//��ǰʱ��
/****************************************************************************
  * ��	       �ƣ�uint8 Is_Time_Value(uint8 *RxBuff)
  * ��           �ܣ��ж������Ƿ�����Чʱ����
  * ��ڲ�����
  * ���ڲ�����1: ��Ч    0:ʱ����Ч
************************************ ****************************************/
uint8 Is_Time_Value(uint8 *RxBuff) 
{
	int32 tmp;

	if(CurDATA.day>Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY==0)
		return 1;
	
	if(CurDATA.day>Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[3])
		;
	else
	{
		tmp = Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[2]*60*60+ Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[1]*60 + Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM[0];
		if(tmp>CurTIME.hour*60*60+CurTIME.min*60+CurTIME.sec-Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY)
			return 1;
	}
	return 0;
}

void F_Access_JZQ_GX(uint8 * RxBuff,uint8 * RetBuff,uint8 Mode)
{

 	switch(Tmp_Frm_GX.gx_jzq_dat.AFN)
	{
	  	case 0x0F:
		{
			switch(Tmp_Frm_GX.gx_jzq_dat.DT)
			{
					case F1:		//
						
					break;
					
					case F2:		//
						
					break;
					
					case F6:																//�ļ�����
					{		
							Struct_UpProgram_True.LimitTime = Struct_UpProgram_True_No1.LimitTime;	//ȡ��ʱʱ��
							
							Struct_UpProgram_True.ChuangShangNUM[0] = *(RxBuff+mVar);		//��ȡ���̱��
							mVar+=1;
							Struct_UpProgram_True.ChuangShangNUM[1] = *(RxBuff+mVar);
							mVar+=1;
							Struct_UpProgram_True.ChuangShangNUM_Ext = *(RxBuff+mVar);
							mVar+=1;
							Struct_UpProgram_True.WorkCode = *(RxBuff+mVar);
							mVar+=1;
							memcpy((uint8 *)&Struct_UpProgram_True.ComNUM[0],RxBuff+mVar,4);
							mVar+=4;
	
							switch(Struct_UpProgram_True.WorkCode)
							{
									case 0x01:	//����Զ�������Ự												
										    if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
										    {											
										  		Struct_UpProgram_True.UpGX_Result_OKorNO=1;  					//���̱����ͬ
										    	Struct_UpProgram_True.UpGX_Result_CODE=0x00;    				//����� ������ȷӦ��
												memcpy((uint8 *)&Struct_UpProgram_True.Mima[0],RxBuff+mVar,3); 	//��ʱ��������
												mVar+=3;														
												Struct_UpProgram_True.UpVersion = *(RxBuff+mVar);				//�����汾(������ʽ)
												mVar+=1;
												Struct_UpProgram_True.LimitTime =*(RxBuff+mVar);				//��ʱʱ��
												mVar+=1;
												UpGX_program_Sta = 1;											//����������һ��
									   		}
											else
											{
												Struct_UpProgram_True.UpGX_Result_OKorNO=0;
												Struct_UpProgram_True.UpGX_Result_CODE=0;	 				
											}										
//ȷ��/����Ӧ��													
mreplyy:										   						
										   nVar+=3;				 //���̱��&���̹�����չ�� 3bytes										   
										   *(RetBuff+nVar)=0; 	 //������1bytes
										   nVar++;				 										  
										   nVar+=4;				 //�Ự�� 4bytes										   
										   *(RetBuff+nVar)=Struct_UpProgram_True.WorkCode; //��Ӧ������
										   nVar++;	
										   if(Struct_UpProgram_True.UpGX_Result_OKorNO==0)
										   		Struct_UpProgram_True_No1.LimitTime = 0;	//�����˳���ʱ����                                                 
										   *(RetBuff+nVar)=(Struct_UpProgram_True.UpGX_Result_OKorNO<<6)+Struct_UpProgram_True.UpGX_Result_CODE; 	//  ����� 
										   nVar++;											
	
										   if( (*(RxBuff+nVar+gx_ofs_jzq_seq)&0x80)==0x80 )  
										   {  	//��ʱ��												   
	                                             memcpy( (uint8 *)(RetBuff+nVar+gx_ofs_jzq_ResultCode+1),(uint8 *)(RxBuff+nVar+gx_ofs_jzq_ResultCode+6),6);   //ʱ�갴ԭ������          
	                                             nVar+= 6;
												 MyTp=1;
										   }
										   else 
										   {   	//��ʱ��											   
	                                             MyTp=0 ;  //  nVar+= (gx_ofs_jzq_ResultCode);												
										   }											   
										  
										   F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  //�������										   
									
									break;
	
									case 0x11:  //ֻ���ڽ���11 ������ 	//����ͨ����Ҫ�ǻ�ȡ   ȡ��Ŀ¼  ȡ���ļ��� 
										if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									    {
									    	if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
									    	{												    													
										  		Struct_UpProgram_True.UpGX_Result_OKorNO=1;  		//���̱����ͬ
										    	Struct_UpProgram_True.UpGX_Result_CODE=0x00;    	//����� ������ȷӦ��
	
												memcpy((uint8 *)&Struct_UpProgram_True.FileRoad[0],(uint8 *)(RxBuff+mVar),64);  // ȡ��Ŀ¼
												mVar+=64;
											    memcpy((uint8 *)&Struct_UpProgram_True.FileName[0],(uint8 *)(RxBuff+mVar),32);  // ȡ���ļ���
											    mVar+=32;
											    Struct_UpProgram_True.LastFileVersion = *((uint8 *)(RxBuff+mVar));					// ȡ�������ļ�֮�汾��
//andy 2009.04.22													
												if(Struct_UpProgram_True_No1.LastFileVersion == 0xff)
												{
													if(Struct_UpProgram_True.LastFileVersion != 0)
													{
														Struct_UpProgram_True.UpGX_Result_OKorNO=0x0;					
														Struct_UpProgram_True.UpGX_Result_CODE=0x0D;				//�ļ��汾�� 			
													}
												}
												else if(Struct_UpProgram_True_No1.LastFileVersion == Struct_UpProgram_True.LastFileVersion )
												{
													if(Return_Update_Status()==2)	//�����ɹ������ļ����������Ը��߰汾����
													{
														Struct_UpProgram_True.UpGX_Result_OKorNO=0x0;					
														Struct_UpProgram_True.UpGX_Result_CODE=0x0D;					//�ļ��汾�� 				
													}
												}
												else if(Struct_UpProgram_True_No1.LastFileVersion > Struct_UpProgram_True.LastFileVersion )
												{
													
													Struct_UpProgram_True.UpGX_Result_OKorNO=0x0;					
													Struct_UpProgram_True.UpGX_Result_CODE=0x0D;					//�ļ��汾�� 				
												}													
//andy 2009.04.22
												UpGX_program_Sta = 2;									//����������һ��
									    	}
											else
											{
												Struct_UpProgram_True.UpGX_Result_OKorNO=0;
												Struct_UpProgram_True.UpGX_Result_CODE=3;	 			//�Ự�Ŵ�			
											}	
								   		}
										else
										{
											Struct_UpProgram_True.UpGX_Result_OKorNO=0;					
											Struct_UpProgram_True.UpGX_Result_CODE=0;					//���̴���� 				
										}	
	            					    goto mreplyy;
	
									break;
	
	
								case 0x13:
								//ֻ���ڽ���13 ������ 
								//���������ļ�����   �ش�14										   
									   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									   {
										   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
										   {													
												nVar+=3;				 	//���̱��&���̹�����չ�� 3bytes
											   
												*(RetBuff+nVar)=0x14; 	 	//������1bytes
												nVar++;				 
												  
												nVar+=4;					//�Ự�� 4bytes
	
												*(RetBuff+nVar)=Struct_UpProgram_True_No1.LastFileVersion;  //ԭ�ļ��汾
												nVar++;
												
	                                            memcpy((uint8 *)(RetBuff+nVar),(uint8 *)&Struct_UpProgram_True_No1.FileSize,4); 	//�ļ���С
												nVar+=4;	
												
												memcpy((uint8 *)(RetBuff+nVar),(uint8 *)&Struct_UpProgram_True_No1.LastUpFileSta[0],2);  //����״̬														
												nVar+=2;
												
												item = Struct_UpProgram_True_No1.LastTotalBlock%8;				//�����ֽ���
												
												if(item==0)
													item = Struct_UpProgram_True_No1.LastTotalBlock/8;	
												else
													item = Struct_UpProgram_True_No1.LastTotalBlock/8 + 1;
												
												if(item>0 && item<128)
												{
													memcpy((uint8 *)(RetBuff+nVar),(uint8 *)&Struct_UpProgram_True_No1.LastUpMask[0],item );  //Ҫ��ʼ��
													nVar+=item; 
												}
												
												if( (*(RxBuff+nVar+gx_ofs_jzq_seq)&0x80)==0x80 )  
												{  //��ʱ��														   
													memcpy( (uint8 *)(RetBuff+nVar+gx_ofs_jzq_ResultCode+1),(uint8 *)(RxBuff+nVar+gx_ofs_jzq_ResultCode+6),6);   //ʱ�갴ԭ������          
													nVar+= 6;
													MyTp=1;
												}
												else 
												{   //��ʱ��
													MyTp=0 ;  //  nVar+= (gx_ofs_jzq_ResultCode);												
												}
	
												F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  //�������
												
	
											    UpGX_program_Sta = 3;								   //����������һ��
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;
											   Struct_UpProgram_True.UpGX_Result_CODE=3;			   //�Ự�Ŵ�	
											   goto mreplyy;
										   }   
									   }
									   else
									   {
										   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
										   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //���̴���� 		
										   goto mreplyy;
									   }  
						 		break;
	                                    												
								case 0x15: //ֻ���ڽ���15 ������  //���ÿ�����									   
	                                   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									   {
										   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
										   {	
										   		
												Struct_UpProgram_True_No1.bak[0] = 0;
										   		memcpy((uint8 *)&Struct_UpProgram_True_No1.FileSize,RxBuff+mVar,4);
												mVar+=4;
												//��ȡ������														
												Struct_UpProgram_True_No1.LastTotalBlock=*(RxBuff+mVar+1)*256+*(RxBuff+mVar);														
												Struct_UpProgram_True_No1.LastTotalBlock&=0x3ffc;
												Struct_UpProgram_True_No1.LastTotalBlock>>=2;   	   // ������	
												if((*(RxBuff+mVar+1)>>6)>0)
												{	//ȫ�´���,���µ�����״̬
													Struct_UpProgram_True_No1.k = 0x4000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//����״̬:2	�����ɹ�
													Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
													Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	
													memset((uint8 *)&Struct_UpProgram_True_No1.LastUpMask[0],0,128);
												}
												else
												{
													Struct_UpProgram_True_No1.k = 0x4000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//����״̬:2	�����ɹ�
													Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
													Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	
												}
												mVar += 2;
												
												//�����ļ��汾
												Struct_UpProgram_True_No1.LastFileVersion = *(RxBuff+mVar);														
												mVar+=1;																							
	
												UpGX_program_Sta = 4;								   //����������һ��
												
												Struct_UpProgram_True.UpGX_Result_OKorNO=1;			   //��ȷ
												Struct_UpProgram_True.UpGX_Result_CODE=0x00;			
												goto mreplyy;
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;
											   Struct_UpProgram_True.UpGX_Result_CODE=3;			   //�Ự�Ŵ�	
											   goto mreplyy;
										   }   
									   }
									   else
									   {
										   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
										   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //���̴���� 		
										   goto mreplyy;
									   }  
									   break;											 
	
								case 0x17: //ֻ���ڽ���17 ������ //�����������ݰ�											  
								 
									   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
									   {
										   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
										   {
												  Struct_UpProgram_True_No1.k = *(RxBuff+26) +  *(RxBuff+27) * 256; 	//���ݴ�С&�ļ�����
												  Struct_UpProgram_True_No1.k &= 0x0fff;		//��ǰ����
												  Struct_UpProgram_True_No1.k += 1;				//��ǰ֡���+1

												  Struct_UpProgram_True.k  = *(RxBuff+29+254)+*(RxBuff+29+254+1)*256;											   
												  mTtemp = PSW_CRC_FILE((BYTE *)(RxBuff+29),1,Struct_UpProgram_True.k,254,1234);											   
												  if(mTtemp==0)
												  {
												   	   sprintf( (char *)ZhuanFa_Buff,"CRC ERR-%04d-%02x:%02x:%02x\n\r ",Struct_UpProgram_True_No1.k,BcdCurTime.hour,BcdCurTime.min,BcdCurTime.sec);																										  
													   ZhuanFa_Flag = 0x01;
													   ZhuanFa_Len = 32;
													   F_SendData_BY_CHB(ZhuanFa_Buff,ZhuanFa_Len);
												
													   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
													   Struct_UpProgram_True.UpGX_Result_CODE=0x0b;				   //CRC��	
													   goto mreplyy;
												  } 												
										  												  
												  Struct_UpProgram_True_No1.k = *(RxBuff+26) +  *(RxBuff+27) * 256; //���ݴ�С&�ļ�����
												  KaiNum = Struct_UpProgram_True_No1.k & 0x0fff; 												  
												  Struct_UpProgram_True_No1.k = *(RxBuff+27) +  *(RxBuff+28) * 256; //���ݴ�С&�ļ�����
												  item = Struct_UpProgram_True_No1.k/16;													  
												  Struct_UpProgram_True_No1.k = KaiNum;
												  if(item>256)	 													 // ���ֽ����ݲ��ܴ���256byte			  
												  {														  		
//												  		sprintf((char *)ZhuanFa_Buff,"Frame E1!!! %04d-%02d:%02x:%02x:%02x\n\r ",Struct_UpProgram_True_No1.k,item,BcdCurTime.hour,BcdCurTime.min,BcdCurTime.sec);																	 
//														ZhuanFa_Flag = 0x01;
//														ZhuanFa_Len = 32;
//														F_SendData_BY_CHB(ZhuanFa_Buff,ZhuanFa_Len);							//CHB ����	����	
//														//OSTimeDly(OS_TICKS_PER_SEC*2);																
////														SysResetEN = 1;
														return;																
														item = 256;
												  }	
												  else
												  {
													    sprintf( (char *)ZhuanFa_Buff,"%04d-%02x:%02x:%02x\n\r ",Struct_UpProgram_True_No1.k+1,BcdCurTime.hour,BcdCurTime.min,BcdCurTime.sec);																										  
													    ZhuanFa_Flag = 0x01;
													    ZhuanFa_Len = 32;
													    F_SendData_BY_CHB(ZhuanFa_Buff,ZhuanFa_Len);	
												  }
												  
												  memcpy((uint8 *)&Struct_UpProgram_True_No1.Updata[Struct_UpProgram_True_No1.k][0],RxBuff+29,item); 	//����
												  memcpy((uint8 *)&Struct_UpProgram_True_No1.CrC[0],RxBuff+29+item,2); 				//crc
	
												  Struct_UpProgram_True_No1.bak[0]++;		//ʵ�ʽ��տ������
												  
												  item = KaiNum%8;
												  KaiNum = KaiNum/8;														  
												  item = 1<<item;
												  if(KaiNum>=128)
												  {
													    SysResetEN = 1;
													    return;
												  }
												  	
												  Struct_UpProgram_True_No1.LastUpMask[KaiNum] |= item; 				// �ı��MASK   
									 
												  Struct_UpProgram_True_No1.k = *(RxBuff+26) +  *(RxBuff+27) * 256; 	//���ݴ�С&�ļ�����
												  Struct_UpProgram_True_No1.k &= 0x0fff;		//��ǰ����
												  Struct_UpProgram_True_No1.k += 1;				//��ǰ֡���+1
	
												  
	
												
												Struct_UpProgram_True_No1.bak[8]=0;
												for(i=0;i<Struct_UpProgram_True_No1.LastTotalBlock;i++)
												{
													j = i/8;
													k = 1<<(i%8);
													if((Struct_UpProgram_True_No1.LastUpMask[j] & k)==0)
														break;
												}																	
												if(i == Struct_UpProgram_True_No1.LastTotalBlock)
												{	
													Struct_UpProgram_True_No1.bak[8]=0xee;	//�յ����������ļ�����֡
												}
												else if(Struct_UpProgram_True_No1.k==Struct_UpProgram_True_No1.LastTotalBlock)
												{																
													Struct_UpProgram_True_No1.bak[8]=0xff;	//��վ������ϣ���δ��ȫ���������ļ�����֡
												}
	
												UpGX_program_Sta=5;  //�ı�״̬					
												return;		 
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;
											   Struct_UpProgram_True.UpGX_Result_CODE=3;			   //�Ự�Ŵ�	
											   goto mreplyy;
										   }   
									   }
									   else
									   {
										   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
										   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //���̴���� 		
										   goto mreplyy;
									   }  
									   break;	
	
										   		   
									case 0x18: ////ֻ���ڽ���18������//�ر��ļ�����
										   if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
										   {
											   if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
											   {
											   		if(Struct_UpProgram_True_No1.bak[8]!=0xee)
											   		{
													   	Struct_UpProgram_True.UpGX_Result_OKorNO=0;
													   	Struct_UpProgram_True.UpGX_Result_CODE=5;			//05H���ļ���������
	
														F_Save_update_File();								//�������������ļ��������
	
														SysResetEN = 1;
											
													   	goto mreplyy;
												    }

											   		UpGX_program_Sta=6;	 //�ı�״̬															
											   		Struct_UpProgram_True.UpGX_Result_OKorNO=1;
												   	Struct_UpProgram_True.UpGX_Result_CODE=0;			   //��ȷ
												   	goto mreplyy;
															
											   }
											   else
											   {
												   	Struct_UpProgram_True.UpGX_Result_OKorNO=0;
												   	Struct_UpProgram_True.UpGX_Result_CODE=3;			   //�Ự�Ŵ�	
												   	goto mreplyy;
											   }   
										   }
										   else
										   {
											   Struct_UpProgram_True.UpGX_Result_OKorNO=0;				   
											   Struct_UpProgram_True.UpGX_Result_CODE=0;				   //���̴���� 		
											   goto mreplyy;
										   }  
								   	break;	
									
									case 0x02: //ֻ���ڽ���02 ������ //�ر������Ự											     
										  if(Struct_UpProgram_True.ChuangShangNUM[0]==CHUANGSHANG1 &&Struct_UpProgram_True.ChuangShangNUM[1]==CHUANGSHANG2 ) 
											{
												if(memcmp((uint8 *)&Struct_UpProgram_True_No1.ComNUM[0],(uint8 *)&Struct_UpProgram_True.ComNUM[0],4)==0)
												{
												 	   UpGX_program_Sta=0;  //�ı�״̬	
												 
													   Struct_UpProgram_True.UpGX_Result_OKorNO=1;
													   Struct_UpProgram_True.UpGX_Result_CODE=0;				//��ȷ 
													   nVar+=3;				 //���̱��&���̹�����չ�� 3bytes
													   
													   *(RetBuff+nVar)=0; 	 //������1bytes
													   nVar++;				 
													  
													   nVar+=4;				 //�Ự�� 4bytes
													   
													   *(RetBuff+nVar)=Struct_UpProgram_True.WorkCode; //��Ӧ������
													   nVar++;												   
													                                                        
													   *(RetBuff+nVar)=(Struct_UpProgram_True.UpGX_Result_OKorNO<<6)+Struct_UpProgram_True.UpGX_Result_CODE; 	//  ����� 
													   nVar++;											

													   if( (*(RxBuff+nVar+gx_ofs_jzq_seq)&0x80)==0x80 )  
													   {  	//��ʱ��												   
		                                                     memcpy( (uint8 *)(RetBuff+nVar+gx_ofs_jzq_ResultCode+1),(uint8 *)(RxBuff+nVar+gx_ofs_jzq_ResultCode+6),6);   //ʱ�갴ԭ������          
		                                                     nVar+= 6;
															 MyTp=1;
													   }
													   else 
													   {   	//��ʱ��											   
		                                                     MyTp=0 ;  //  nVar+= (gx_ofs_jzq_ResultCode);												
													   }
													   //F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  //�������	

													   if(Struct_UpProgram_True_No1.bak[8]==0xff)
													   {
													   		Struct_UpProgram_True.UpGX_Result_OKorNO=0;				//���:ʧ��
													   		Struct_UpProgram_True.UpGX_Result_CODE=5;				//ԭ��:�ļ��޷�����
													   		F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  		//�������	
															
													   		
															//������ϵ���֡©��������״̬��0:ʧ��
															//����״̬-- �ϴ�������� 0:����ʧ��/2:�ɹ�/1:���ڴ�/3:�ļ�������
															Struct_UpProgram_True_No1.k = 0x4000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//����״̬:2	�����ɹ�
															Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
															Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	
												
														    F_Save_update_File();		//�������������ļ��������
													   }
													   else if(Struct_UpProgram_True_No1.bak[8]==0xee)
													   {		
															F_ACK_Read_Para_UpGrate(RetBuff,0,1,nVar,Mode,MyTp,0);  			//�������

															//���յ���������֡������״̬��2:�ɹ�
															//����״̬-- �ϴ�������� 2:�ɹ�/0:����ʧ��/1:���ڴ�/3:�ļ�������
															Struct_UpProgram_True_No1.k = 0x8000 + Struct_UpProgram_True_No1.LastTotalBlock*4 + 0; 				//����״̬:2	�����ɹ�
															Struct_UpProgram_True_No1.LastUpFileSta[0] = Struct_UpProgram_True_No1.k % 256;
															Struct_UpProgram_True_No1.LastUpFileSta[1] = Struct_UpProgram_True_No1.k / 256;	

															F_Save_Para_update_File();	//���������ļ�����
															
														    F_Update();					//ִ����������
													   }

													   return;		 
												}
												else
												{
													 Struct_UpProgram_True.UpGX_Result_OKorNO=0;
													 Struct_UpProgram_True.UpGX_Result_CODE=3;				//�Ự�Ŵ�	 
													 goto mreplyy;
												}	
											}
											else
											{
												Struct_UpProgram_True.UpGX_Result_OKorNO=0; 				
												Struct_UpProgram_True.UpGX_Result_CODE=0;					//���̴����		 
												goto mreplyy;
											}  
									 break; 
									 
									 default:
									 break;
								}
					}
					break;
					default:
					break;
			}
		}
		break;
		default:
		break;
	}
}

void  Attand_ZhuZhan_HB_Command(uint8 *RxBuff,uint8 *RetBuff,uint8 Mode)
{	  
	  //int32     tmp;
	  uint8		ret;  
	  
	  Tmp_Frm_GX.gx_dat_len = *(RxBuff+gx_ofs_jzq_len)/4 + *(RxBuff+gx_ofs_jzq_len+1)*256/4;
	  Tmp_Frm_GX.gx_frm_ctl = *(RxBuff+gx_ofs_jzq_ctl);
	  
	  Tmp_Frm_GX.gx_jzq_adr.A1 = *(RxBuff+gx_ofs_jzq_addr)+*(RxBuff+gx_ofs_jzq_addr+1)*256;
	  Tmp_Frm_GX.gx_jzq_adr.A2 = *(RxBuff+gx_ofs_jzq_addr+2)+*(RxBuff+gx_ofs_jzq_addr+3)*256;
	  Tmp_Frm_GX.gx_jzq_adr.A3 = *(RxBuff+gx_ofs_jzq_addr+4);	  
	  Tmp_Frm_GX.gx_jzq_dat.AFN = *(RxBuff+gx_ofs_jzq_afn);
	  Tmp_Frm_GX.gx_jzq_dat.SEQ = *(RxBuff+gx_ofs_jzq_seq);
	  Tmp_Frm_GX.gx_jzq_dat.DA = *(RxBuff+gx_ofs_jzq_dat)+*(RxBuff+gx_ofs_jzq_dat+1)*256;
	  Tmp_Frm_GX.gx_jzq_dat.DT = *(RxBuff+gx_ofs_jzq_dat+2)+*(RxBuff+gx_ofs_jzq_dat+3)*256;
	  Tmp_Frm_GX.gx_jzq_dat.PW	= *(RxBuff+Tmp_Frm_GX.gx_dat_len-1)*256+*(RxBuff+Tmp_Frm_GX.gx_dat_len-2);
	  Tmp_Frm_GX.gx_jzq_dat.EC	= *(RxBuff+Tmp_Frm_GX.gx_dat_len-1)*256+*(RxBuff+Tmp_Frm_GX.gx_dat_len-2);
	  ret = Tmp_Frm_GX.gx_jzq_dat.SEQ & 0x80;	  
	  if(ret)
	  {
	  		Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.PFC = *(RxBuff+Tmp_Frm_GX.gx_dat_len);
			memcpy(Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.TIM,RxBuff+Tmp_Frm_GX.gx_dat_len+1,4);
	  		Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY = *(RxBuff+Tmp_Frm_GX.gx_dat_len+ 5);
			if(Tmp_Frm_GX.gx_jzq_dat.struct_frm_Tp.DLY != 0)
			{
				ret = Is_Time_Value(RxBuff);
		 		if(ret==0)
		 			return;	
			}
	  }	

	  F_Access_JZQ_GX(RxBuff,RetBuff,Mode);


	  return;	  

}

/*********************************************************************************************************
**											END OF FILE												   	**
**********************************************************************************************************/
