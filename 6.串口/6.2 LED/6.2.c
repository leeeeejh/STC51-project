#include<reg52.h>
#define uchar unsigned char 
#define uint unsigned int 

uchar a,flag;

void init();

void main()
{
	init();
	while(1)
	{
		if(1==flag)
		{
			ES=0;					//�رմ����ж�
			P1=a;					//����С��
			ES=1;				    //���������ж�
			flag=0;					//�����־λ
		}	
	}
}

void init()
{
	TMOD=0x20;						//���ö�ʱ��1�Ĺ�����ʽΪ2
	TH1=0xfd;						//T1��ʱ������ֵ
	TL1=0xfd;						
	TR1=1;							//����T1

	REN=1;							//�����ڽ���
	SM0=0;							//�趨���ڹ�����ʽ1
	SM1=1;

	EA=1;							//�����ж�
	ES=1;							//�������ж�
}

void ser() interrupt 4
{
	RI=0;						   //�ֶ�����
	a=SBUF;						   //��������	
	flag=1;						   //��־�Ƿ����յ�����
}