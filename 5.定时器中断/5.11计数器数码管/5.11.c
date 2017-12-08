/*
��ʱ��0������ʽ1,�öŰ���һ�ν�GND,һ�ν�T0(P3^4)����,
ÿ�Ӵ�һ��,��������һ����,�����Ƶ�����ʵʱ��ʾ�������
��ǰ��λ,����100����
*/

/*�����ڽӴ���Ƭ�����ŵ�ʱ��ᷢ������,���½Ӵ�һ�β���������Ϊ1*/

#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char

uchar code dutable[]={		//����������ܶ�ѡ��
//0   1    2	3 
0x3f,0x06,0x5b,0x4f,
//4   5	   6 	7
0x66,0x6d,0x7d,0x07,
//8   9	   A	b
0x7f,0x6f,0x77,0x7c,
//C   d	   E	F
0x39,0x5e,0x79,0x71,
//-   .  ����ʾ
0x40,0x80,0x00};

sbit dula=P2^6;
sbit wela=P2^7;

void init();		  //��������ʼ������
uint read();		  //��ȡ�����м������Ĵ����е�ֵ
void display(uchar);  //�������ʾ����
void delayms(uchar);  //һ����ʱ����

void main()
{
	uint num;
	init();
	while(1)
	{
		num=read();
		if(100<=num)
		{
			num=0;
			TH0=0;	//���������ļĴ���ֵ����
			TL0=0;
		}
		display(num);
	}
}

/*��������ʼ������*/
void init()
{
	TMOD=0x05;		//���ü�����0�Ĺ�����ʽ1(0000 0101)
	TH0=0;			//���������Ĵ���ֵ����
	TL0=0;
	TR0=1;			//����������T0
}

/*��ȡ�����м������Ĵ����е�ֵ*/
uint read()
{
	uchar tl0,th01,th02;
	uint val;
	while(1)
	{
		th01=TH0;			  //�Ĵ�����ֵ����ʱ�仯,ֻ��һ��,
		tl0=TL0;			  //��������λʱ,���ܻ��������,
		th02=TH0;			  //����TH0��������Ҫ������,
		if(th01==th02)		  //��ȷ����ȡ��ʱ��û�з�����λ
			break;
	} 
	val=th01*256+tl0;
	return val;
}

/*�������ʾ����*/
void display(uchar date)
{
	uchar shi,ge;
	shi=date/10;
	ge=date%10;

	dula=1;
	P0=dutable[shi];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfe;
	wela=0;
	delayms(5);

	dula=1;
	P0=dutable[ge];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfd;
	wela=0;
	delayms(5);
}

/*һ����ʱ����*/
void delayms(uchar xms)
{
	uchar i,j;
	for(i=xms;i>0;i--)
		for(j=1114;j>0;j--);
}
