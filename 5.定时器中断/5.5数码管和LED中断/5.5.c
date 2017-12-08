#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char 
sbit dula=P2^6;
sbit wela=P2^7;

uchar code dutable[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

void init();
void display(uchar);
void delayms(uchar);

uchar time,time0,time1;		//timeΪ�������ʾ���֣�time0,time1����T0,T1��ʱ������
uchar temp1=0x7f;			//LED

void main()
{
	
	init();
	while(1)
	{
		display(time);
	}
}

/*�жϷ������⹦�ܼĴ�������*/
void init()
{
	TMOD=0x11;			//�趨��ʱ��T1��T0�ļ���������ʽΪ1
	TH0=TH1=(65536-45872)/256;
	TL0=TL1=(65536-45872)%256;		//�趨T0��T1�Ķ�ʱ����Ϊ50ms
	ET0=ET1=1;			//��T1��T0�ж�
	TR0=TR1=1;			//������ʱ��T1��T0�ж�
	
	EX0=1;				//�����ⲿ�ж�
	IT0=0;				//�ⲿ�ж�0Ϊ�͵�ƽ����

	EA=1;				//�����ж�
}

/*�������ʾ����*/
void display(uchar num)
{
	uchar shi,ge;		   //����ʮλ���͸�λ��
	shi=num/10;
	ge=num%10;

	dula=1;
	P0=dutable[shi];
	dula=0;
	P0=0xff;			  //��Ӱ
	wela=1;
	P0=0xfe;
	wela=0;
	delayms(5);			  //��Ӱ

	dula=1;
	P0=dutable[ge];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfd;
	wela=0;
	delayms(5);
}

/*��ʱ����*/
void delayms(uchar xms)
{
	uchar i,j;
	for(i=xms;i>0;i--)
		for(j=115;j>0;j--);
}

/*T0��ʱ���жϷ������*/			 
void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;		   //50ms
	time0++;
	if(20==time0)				   //1s
	{
		time0=0;
		time++;
	}
	if(60==time)	
		time=0;
}

/*T1��ʱ���жϷ�������*/
void T1_time() interrupt 3
{
	TH1=(65536-45872)/256;		   //50ms
	TL1=(65536-45872)%256;
	time1++;
	if(10==time1)				   //500ms
	{
		time1=0;
		P1=temp1;
		temp1=_cror_(temp1,1);
	}
}

/*�ⲿ�ж�0�жϷ�������*/
void int0() interrupt 0
{
	TR0=0;
} 