#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit dula=P2^6;				//��ѡ
sbit wela=P2^7;				//λѡ

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

void init();				//�жϷ����ʼ��
void display(uchar);		//�������ʾ��ʱ������
void delayms(uint);			//һ����ʱ

uchar num,num1;

void main()
{
	init();					//��ʼ��
	while(1)
	{
		if(20==num)			//1s
		{
			num=0;
			num1++;
			if(6==num1)		//��6��
			{
				ET0=0;		//�رն�ʱ��,�ȴ��ⲿ�жϷ���
				PCON=0x01;	//�������״̬

							/*
							PCON=0x01,����ģʽ,��ʱ��ʱ�����Ṥ��
							PCON=0x02,����ģʽ,��ʱ��ʱ�����Ṥ��

							PCON=0x02�ȼ���PC0N=0x01����ET0=0;
							*/
			}
		}
		display(num1);		//�������ʾ��ʱ������
	}
}

/*�жϷ����ʼ��*/
void init()
{
	TMOD=0x01;				//���ö�ʱ��0������ʽ1(0000 0001)
	TH0=(65535-45872)/256;
	TL0=(65536-45872)%256;	//50ms
	EA=1;					//�����ж�
	ET0=1;					//����ʱ��0�ж�
	EX0=1;					//���ⲿ�ж�0
	EX1=1;					//���ⲿ�ж�1
	TR0=1;					//������ʱ��0
}

/*��ʱ��T0����*/
void T0_time() interrupt 1
{
	TH0=(65535-45872)/256;
	TL0=(65536-45872)%256;	//50ms
	num++;	
}

/*�ⲿ�ж�0����(P3^2�ӿ�)*/
void EX_int0() interrupt 0	
{
	PCON=0;					//�˳�����״̬
	ET0=1;					//�򿪶�ʱ��0�ж�
}

/*�ⲿ�ж�1����(P3^3�ӿ�)*/
void EX_int1() interrupt 2
{
	PCON=0;					//�˳�����״̬
	ET0=1;					//�򿪶�ʱ��0�ж�
}

/*�������ʾ��ʱ������*/
void display(uchar a)
{
	uchar shi,ge;
	shi=a/10;
	ge=a%10;
	
	dula=1;
	P0=dutable[shi];	   //��ʮλ��ѡ����
	dula=0;				   
	P0=0xff;			   //��λѡ����ǰ�ر�������ʾ,��ֹ��λѡ����ʱ																				
	wela=1;				   //ԭ����ѡ����ͨ��λѡ��������ɻ���
	P0=0xfe;			   //��λѡ����
	wela=0;
	delayms(5);			   //��ʱ
	
	dula=1;
	P0=dutable[ge];		  //�͸�λ��ѡ����
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfd;
	wela=0;
	delayms(5);	
}

/*һ����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}