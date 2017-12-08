#include<reg52.h>
#define uchar unsigned char 
#define uint unsigned int 
sbit key1=P3^0;					 //����4����������
sbit key2=P3^1;
sbit key3=P3^2;
sbit key4=P3^3;

sbit dula=P2^6;
sbit wela=P2^7;

uchar code dutable[]={			  //��ѡ��ʾ����
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

uchar num,number;

void display(uchar monent);		  //��ʾ�ӳ���
void delayms(uint xms);			  //��ʱ�ӳ���
void keyscan();					  //�������̶�ȡ�ӳ���


void main()
{
	TMOD=0x01;					  //��ʱ����ʼ��
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	EA=1;
	ET0=1;
	TR0=1;

	while(1)
	{
		keyscan();
		display(num);
	}
}



void display(uchar monent)
{
	uchar shi,ge;				  //����ʮλ���͸�λ��
	shi=monent/10;
	ge=monent%10;

	dula=1;
	P0=dutable[shi];
	dula=0;
	P0=0xff;					  //��ѡ����Ӱ
	wela=1;
	P0=0xbf;
	wela=1;
	delayms(5);

	dula=1;
	P0=dutable[ge];
	dula=0;
	P0=0xff;					  //��ѡ����Ӱ
	wela=1;
	P0=0x7f;
	wela=0;
	delayms(5);
}

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void keyscan()
{
	if(0==key1)
	{
		delayms(10);			 //��ʱ����
		if(0==key1)
		{
			num++;
			if(60==num)
				num=0;
			while(!key1);		 //�ȴ������ͷ�
		}						 
	}

	if(0==key2)
	{
		delayms(10);
		if(0==key2)
		{
			if(0==num)
				num=60;
			num--;
			while(!key2);
		}
	}

	if(0==key3)
	{
		delayms(10);
		if(0==key3)
		{
			num=0;
			while(!key3);
		}
	}

	if(0==key4)
	{
		delayms(10);
		if(0==key4)
		{
			while(!key4);
			TR0=~TR0;
		}
	}
}

void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	number++;
	if(20==number)
	{							  //20����ʱ����Ϊ1��
		number=0;
		num++;
		if(60==num)
			num=0;
	}
}
