/*
S03:cancel ȡ��
S13:correct ����
S23:change �޸�
s33:cinfirm ȷ��

S00 �� S32
1 2 3
4 5 6
7 8 9 
  0
*/


#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
/*�����*/
sbit dula=P2^6; //���������������
sbit wela=P2^7;
/*Һ��*/
sbit lcdrs=P3^5;		   //RSҺ����������ѡ���
sbit lcdrw=P3^6;		   //WRҺ����дѡ���
sbit lcden=P3^4;		   //LCDENҺ��ʹ�ܶ�
/*led*/
sbit led1=P1^0;	   //�������������ʾ��
sbit led2=P1^1;
sbit led3=P1^2;
sbit led7=P1^6;	   //����λ��������ʾ��
sbit led8=P1^7;	   //����λ��������ʾ��
/*������*/
sbit beep=P2^3;	   //������

/*******************�Ӻ���*****************************
******************************************************/
/*Һ��*/
void yj_display(uint num);//Һ����ʾ����
void yj_write_com (uchar com);//Һ������д��
void yj_write_data(uchar date);//Һ������д��
/*���ⰴ��*/
void aj_Cancel();	   //ȡ��
void aj_Change();	   //�޸�
void aj_Correct();	   //����

/*��ʱ*/
void delayms(uchar xms);//ms����ʱ����
/*����ɨ��*/
void matrixkeyscan();//����ɨ�躯��             
/*��ʼ��*/
void init();	  //��ʼ������

/*����*/
uchar code inputtable[15]={"Input  Password"};//��ʼ����
uchar code changetable[13]={"Input New Key"}; //�޸�����
uchar code againtable[11]={"Input Again"};	  //������������
uchar code righttable[15]={"Password  Right"};//������ȷ
uchar code errortable[11]={"    Error  "};	  //�������
uchar code cleantable[16]={"                "};//�������
/*����Ĵ�����*/
uchar table1[6]; //�����ʼ����
uchar table2[6]; //�������������

uchar wei,error,xiugai;//wei:��¼����������λ��;error:��¼�����Դ���Ĵ���;xiugai:�޸Ŀ���
bit cancel,correct,change,confirm;//cancel:ȡ����;correct:������;change:�޸ļ�;confirm:ȷ�ϼ�


void main()
{
	uchar i,temp;	 //temp:��¼�����Ƿ�������ȷ

	init();  //��ʼ��
	while(1)
	{
		matrixkeyscan();	//����ɨ���ӳ���
		if(wei==6&&confirm==1)			//�ж�λ���Ƿ�Ϊ6λ�����Ѿ�������ȷ�ϰ���
		{
			temp=0;					 //��ʼ����������ȷ
			for(i=0;i<6;i++)
			{
				if(table2[i]!=table1[i])//����������벻��ȷ
				{
					temp=1;			    //���tempΪ1
					break;				//�˳���forѭ��
				}
			}
			if(temp==1)				  //������벻��ȷ
			{
				error--;	//ʣ����������1
				if(error==2)	//�������1��
					led1=0;		//����led1
				if(error==1)	//�������2��
				{
					led1=0;		//����led1
					led2=0;		//����led2
				}
				if(error==0)	//�������3��
				{
					led1=0;		//����led1
					led2=0;		//����led2
					led3=0;		//����led3
					beep=0;		//����������
					while(1);
				}
				/*��ʾ�������2��*/
				yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
				yj_write_com(0x80);
				for(i=0;i<11;i++)
				{
					yj_write_data(errortable[i]);
				}
				yj_write_data(3-error+'0');	//��ʾ�������
				for(i=10;i>0;i--)
					delayms(1000);	  //��ʱ10s
				/*�ָ�������ʾ����*/
				yj_write_com(0x01);	   //00000001B,��ʾ��0������ָ����0
				wei=0;	   //��¼���������λ��
				confirm=0;//���ȷ�ϼ���־
				yj_write_com(0x80);
				for(i=0;i<15;i++)
				{
					yj_write_data(inputtable[i]);
				}
				yj_write_com(0x80+0x40); 
				yj_write_com(0x0f);//�����˸
			}
			if(temp==0)			//���������ȷ
			{
				yj_write_com(0x01);	   //00000001B,��ʾ��0������ָ����0
				yj_write_com(0x80);
				for(i=0;i<15;i++)
				{
					yj_write_data(righttable[i]);	//��ʾ������ȷ����ʾ��Ϣ
				}
				yj_write_com(0x80+0x40);
				for(i=0;i<6;i++)
				{
					yj_write_data(table1[i]+'0');	//��ʾ����
				}
				xiugai=1;	//���������޸Ŀ��ش�
				do
				{
					uchar temp,key;
					P3=0xfb;
					temp=P3;
					temp=temp&0xf0;
					if(temp!=0xf0)
					{
						delayms(10);
						temp=P3;
						temp=temp&0xf0;	 //�ظ��ж�,ȷ�ϰ�������
						if(temp!=0xf0)
						{
							temp=P3;
							switch(temp)
							{
								case 0x7b:	change=1;	break;//�޸ļ�
							}
							while(temp!=0xf0)
							{
								temp=P3;
								temp=temp&0xf0;
							}
						}
					}
				}while(change==0);
				aj_Change();
			}
		}
	}
}

/*��ʼ��*/
void init()
{
	uchar i;
	
	dula=0;
	wela=0;			 //�ر�������������ˣ���ֹ����Һ��ʱ����ܻ������
	lcden=0;		//�ر�Һ��ʹ�ܶ�
	/*��ʼ��������*/
	wei=0;	   //��¼���������λ��
	error=3;   //��¼�����Դ���Ĵ���
	xiugai=0;	//�رտ��޸Ŀ���
	cancel=0;
	correct=0;
	change=0;
	confirm=0;

	for(i=0;i<6;i++)
	{
		table1[i]=0;
		table2[i]='f';
	}
	
	/*��ʼ��1602Һ��*/
	yj_write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	yj_write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	yj_write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
	/*��ʾ��ʼ����*/
	yj_write_com(0x80);
	for(i=0;i<15;i++)
	{
		yj_write_data(inputtable[i]);
	}
	yj_write_com(0x80+0x40);
	yj_write_com(0x0f);//�����˸
}


/******************************************************
*******************Һ����ʾ****************************
******************************************************/
void yj_display(uchar num)
{
	if(wei<=5) //����λ��С��6ʱ
	{	
		wei++;	
		yj_write_com(0x80+0x40+wei-1); //��Һ��������һλ��ʾ"*"
		yj_write_data('*');
		table2[wei-1]=num;	//����������봢�浽table2������
		if(wei==6)
		{
			yj_write_com(0x80+0x40+5);
			yj_write_com(0x0c);//ȡ�������˸
		}
	}
	else
	{
		wei=6;
		led8=0;
		beep=0;
		delayms(500);		//led8��0.5s,���ŷ�������0.5s��ʾλ���ѵ�����
		beep=1;
		led8=1;
	}
}

/*Һ������д��*/
void yj_write_com (uchar com)
{
	lcdrw=0;		   //ѡ��д
	lcdrs=0;		   //ѡ������
	P0=com;
	delayms(5);		   //��ʱʹҺ�������ȶ�
	lcden=1;		   //�����彫��������Һ��������
	delayms(5);
	lcden=0;
}

/*Һ������д��*/
void yj_write_data(uchar date)
{
	lcdrw=0;		  //ѡ��д
	lcdrs=1;		  //ѡ������
	P0=date;		  
	delayms (5);	  //��ʱʹҺ�������ȶ�
	lcden=1;		  //�����彫��������Һ��������
	delayms (5);
	lcden=0;
}


/******************************************************
*******************���ⰴ��****************************
******************************************************/
/*ȡ������*/
void aj_Cancel()
{
	uchar i;

	cancel=0;
	/*��ʼ��������*/
	wei=0;
	/*����ڶ�������*/
	yj_write_com(0x80+0x40);
	for(i=0;i<6;i++)
	{
		yj_write_data(cleantable[i]);
	}
	for(i=0;i<6;i++)
	{
		table2[i]='f';	  	//�����������еĴ����������Ϊ������
	}
	yj_write_com(0x80+0x40);
}

/*��������*/
void aj_Correct()
{
	uchar i;

	correct =0;
	wei -= 1;	
	table2[wei]='f';
	yj_write_com(0x80+0x40+wei);
	yj_write_data(' ');
	yj_write_com(0x80+0x40+wei);
	yj_write_com(0x0f);
} 

/*�޸İ���*/
void aj_Change()
{
	uchar i,k;	//k:���������Ƿ���ȷ���λ
	wei=0;	   //��¼���������λ��
	error=3;   //��¼�����Դ���Ĵ���
	xiugai=0;	//�رտ��޸Ŀ���
	change=0;
	confirm=0;
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
	/*��ʾ�޸Ľ���*/
	yj_write_com(0x80);
	for(i=0;i<13;i++)	   //��ʾ�޸Ľ����µĵ�һ������
	{
		yj_write_data(changetable[i]);
	}
	yj_write_com(0x80+0x40); //����Ƶ��ڶ��е�һλ
	yj_write_com(0x0f);	 //�����˸
	
	do
	{
		matrixkeyscan();	//��ȡ������
	}while(wei<6||confirm==0);

	for(i=0;i<6;i++)
	{
		table1[i]=table2[i]; //����Ϊyj_display�����д�����������븴�Ƶ�ԭʼ������
		table2[i]='f';		 //�����������еĴ����������Ϊ������
	}
	
	do
	{
		confirm=0;		 //ȡ��ȷ��λ
		k=1;			 //�ظ�����ɹ���־λ���1
		yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
		yj_write_com(0x80);	
		for(i=0;i<11;i++)		//��ʾ�ظ�����ģʽ�µĵ�һ������
		{
			yj_write_data(againtable[i]);
		}
		yj_write_com(0x80+0x40);//�ڶ�����ʾ
		yj_write_com(0x0f);		//�����ʾ

		wei=0;		  //�������������λ��
 	
		do
		{
			matrixkeyscan();	//��ȡ���������֤��������
		}while(wei<6||confirm==0);

		for(i=0;i<6;i++)
		{
			if(table2[i]!=table1[i])
			{
				k=0;		//�������,�ظ�����ɹ���־λ���0,Ҫ����������
				break;
			}
		}
	}while(0==k);
	/*��ʼ����ʾ*/
	wei=0;	   //��¼���������λ��
	error=3;   //��¼�����Դ���Ĵ���
	xiugai=0;	//�رտ��޸Ŀ���
	cancel=0;
	correct=0;
	change=0;
	confirm=0;
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
	/*��ʾ��ʼ����*/
	yj_write_com(0x80);
	for(i=0;i<15;i++)
	{
		yj_write_data(inputtable[i]);
	}
	yj_write_com(0x80+0x40);
	yj_write_com(0x0f);//�����˸
}


/******************************************************
*******************����ɨ��****************************
******************************************************/
/*�������ɨ��*/
void matrixkeyscan()             
{
	uchar temp,key;
	P3=0xfe;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)	//�ظ��ж�,ȷ�ϰ�������
		{
			temp=P3;
			switch(temp)
			{
				case 0xee:	key=1;	break;	//����1
				case 0xde:	key=2; 	break;	//����2
				case 0xbe:	key=3;	break;	//����3
				case 0x7e:	cancel=1;	break;//ȡ����
			}
			while(temp!=0xf0) //�ȴ������ͷ�
			{
				temp=P3;
				temp=temp&0xf0;
			}
			if(cancel==0)	 //û�а�ȡ����
			{
				yj_display(key);//��ʾ����
			}
			else 
			{	
				if(wei>=1)		 //�����Ѿ�������һ������
					aj_Cancel();	 //ȡ������,�˳���������
			}
		}
	}

	P3=0xfd;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;	 //�ظ��ж�,ȷ�ϰ�������
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xed:	key=4;	break;	//����4
				case 0xdd:	key=5;	break;	//����5
				case 0xbd:	key=6;	break;	//����6
				case 0x7d:	correct=1;	break;//������
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
			if(correct==0)	  //û�и�����
			{
				yj_display(key);  //��ʾ����
			}
			else 
			{
				if(wei>=1)		  //�����Ѿ�������һ������
					aj_Correct();	  //������һ��������ַ�
			}
		}
	}

	P3=0xfb;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;	 //�ظ��ж�,ȷ�ϰ�������
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xeb:	key=7;	break;	//����7
				case 0xdb:	key=8;	break;	//����8
				case 0xbb:	key=9;	break;	//����9
				case 0x7b:	change=1;	break;//�޸ļ�
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
			if(change==0)	 //û�а��޸ļ�
			{
				yj_display(key);  //��ʾ����
			}
			else 
			{
				if(xiugai==1)	//������޸Ŀ��ش�
					aj_Change();	//�޸�����;
			}
		}
	}

	P3=0xf7;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;	  //�ظ��ж�,ȷ�ϰ�������
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xd7:	key=0;	break;	//����0
				case 0x77:	confirm=1;	break;//ȷ�ϼ�	
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}	
			if(confirm==0)	//û�а�ȷ����
			{
				yj_display(key);  //��ʾ����
			}
			else 
			{
				if(wei==6);
				else
				{	
					led7=0;
					delayms(500);	 //led7��0.5s��ʾδ������6λ����
					led7=1;
				}
			}
		}
	}
}


/******************************************************
*******************��ʱ����****************************
******************************************************/
/*ms����ʱ*/
void delayms(uint xms)   
{
 	uint i,j;
 	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}









