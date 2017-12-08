#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char 
sbit beep=P2^3;
uchar a,flag;

void init();

void main()
{
	init();
	while(1)
	{
		if(1==flag)
		{
			ES=0;			  //�رմ����ж�
			if(a=='Y')		  //����������ΪYʱ����������
				beep=0;
			if(a=='N')		  //��SBUFΪNʱ�رշ�����
				beep=1;
			ES=1;			  //���������ж�
			flag=0;			  //��־����
		}
	}
}

void init()
{
	TMOD=0x20;			//�趨T1��ʱ���Ĺ�����ʽΪ2
	TH1=0xfd;			//T1��ʱ��װ��ֵ
	TL1=0xfd;
	TR1=1;				//����T1��ʱ��

	REN=1;			    //�����ڽ���
	SM0=0;				//�趨���ڹ�����ʽ1
	SM1=1;
		
	EA=1;				//�����ж�
	ES=1;				//�������ж�
}

void ser() interrupt 4
{
	RI=0;				//�ֶ�����
	a=SBUF;				//��������
	flag=1;				//��־
}