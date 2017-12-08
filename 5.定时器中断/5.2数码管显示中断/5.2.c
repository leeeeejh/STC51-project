#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

sbit led1=P1^0;
sbit dula=P2^6;
sbit wela=P2^7;

uchar code dutable[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

uchar num0,num1,num2;
void display(uchar);
void delayms(uint);

void main()
{
	TMOD=0x11;					    //���ö�ʱ��0�Ͷ�ʱ��1�Ĺ�����ʽΪ1 (0001 0001)
	TH0=(65536-45872)/256;			//��ʱ������Ϊ50ms
	TL0=(65536-45872)%256;
	EA=1;							//�����ж�
	ET0=1;							//����ʱ��0�ж�
	ET1=1;							//����ʱ��1�ж�
	TR0=1;							//������ʱ��0
	TR1=1;							//������ʱ��1
	while(1)
	{
		if(20==num1)				//����20�Σ�Ϊ1s
		{
			num1=0;
			num2++;
		}
		if(60==num2)				//��60s����Ϊ0
			num2=0;
		display(num2);
	}
}

void display(uchar num)
{
	uchar shi,ge;				    //�����λ����
	shi=num/10;
	ge=num%10;
	
	dula=1;
	P0=dutable[shi];
	dula=0;
	P0=0xff;						   //��Ӱ
	wela=1;
	P0=0xbf;
	wela=0;
	delayms(5);

	dula=1;
	P0=dutable[ge];
	dula=0;
	P0=0xff;						   //��Ӱ
	wela=1;
	P0=0x7f;
	wela=0;
	delayms(5);
}

void delayms(uint xms)					//��ʱ�ӳ���
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;				 //��װ��ֵ��50ms
	TL0=(65536-45872)%256;
	num0++;
	if(4==num0)					//����4�Σ���200ms��led��˸һ��
	{
		num0=0;					  //T0��T1���жϿ��ܴ��ڳ�ͻ������T0���ۼӼ�������д��������
		led1=~led1;
	}
}

void T1_time() interrupt 3
{
	TH0=(65536-45872)/256;			    //��װ��ֵ��50ms
	TL0=(65536-45872)%256;
	num1++;
}