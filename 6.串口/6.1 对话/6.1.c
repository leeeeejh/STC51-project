#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char 
uchar flag,a;
uchar code table[]={"I GET A "};

void init();

void main()
{
	uchar i;
	init();
	while(1)
	{
		if(1==flag)
		{
			ES=0;							 //�رմ����ж�
			for(i=0;i<8;i++)
			{
				SBUF=table[i];				 //��������
				while(!TI);					 //�ȴ��Ƿ������
				TI=0;						 //�ֶ���0
			}
			SBUF=a;
			while(!TI);						 
			TI=0;
			ES=1;							  
			flag=0;
		}	
	}
}

void init()
{
	TMOD=0X20;				 //�趨��ʱ��1�Ĺ�����ʽΪ2
	TH1=0xfd;				 //T1��ʱ��װ��ֵ
	TL1=0xfd;
	TR1=1;					 //������ʱ��T1

	REN=1;					 //�����ڽ���
	SM0=0;
	SM1=1;					 //���ô��ڵĹ�����ʽΪ1

	EA=1;					 //�����ж�
	ES=1;					 //�������ж�
}

void ser() interrupt 4
{
	RI=0;					  //�ֶ���0
	a=SBUF;					  //��������
	flag=1;					  //��־�ѽ���
}