#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit dula=P2^6;
sbit wela=P2^7;
uchar code dutable[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
 
void init();
void display(uchar);
void delayms(uint);

uchar a;

void main()
{
	init();	   			//�������ó�ʼ��
	while(1)
	{					
			ES=0;
			display(a);	//ȡ������SBUF��ֵ�����������ʾ
			ES=1;	
	}			
}

void init()
{
	TMOD=0x20;			//T1����ģʽ2 8λ�Զ���װ
	TH1=253;
	TL1=253;			//������9600
	TR1=1;				//������ʱ��1�ж�
	
	SM0=0;
	SM1=1;				//�趨���ڹ�����ʽ
	REN=1;				//�����ڽ�������

	EA=1;				//�����ж�
	ES=1;				//�������ж�
}

void display(uchar num)
{
	uchar ge,shi,bai;
	bai=num/100;
	shi=num%100/10;
	ge=num%10;

	dula=1;
	P0=dutable[bai];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfe;
	wela=0;
	delayms(5);

	dula=1;
	P0=dutable[shi];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfd;
	wela=0;
	delayms(5);

	dula=1;
	P0=dutable[ge];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfb;
	wela=0;
	delayms(5);
}

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}	

void ser() interrupt 4
{
	RI=0;						   //�ֶ�����
	a=SBUF;						   //��������	
}