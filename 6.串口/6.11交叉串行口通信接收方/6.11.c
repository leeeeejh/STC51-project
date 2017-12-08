#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit dula=P2^6;
sbit wela=P2^7;

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

void init();		//����ͨ�ŷ����ʼ��
void display(uchar);		//�������ʾ����

void main()
{
	init();			//��ʼ��
	P0=0xfe;		//λѡ�е�һλ�����
	wela=1;
	wela=0;
	while(1);
}

/*�����жϷ���*/
void ser0() interrupt 4
{
	uchar a;		//�Ĵ洮�ڽ��յ�������
	RI=0;			//�����жϱ�־λ����
	a=SBUF;			//��ȡ��������
	dispaly(a);		//�������ʾ
}

/*����ͨ�ŷ����ʼ������*/
void init()
{
	TMOD=0x20;		//���ö�ʱ��1�Ĺ�����ʽ2
	TH1=253;		//������9600
	TL1=253;		
	TR1=1;			//������ʱ��T1

	SM0=0;
	SM1=1;			//���ڹ�����ʽ1
	REN=1;			//�����ڽ�������

	EA=1;			//�����ж�
	ES=1;			//�������ж�
}

/*�������ʾ����*/
void display(uchar num)
{
	P0=dutable[num];
	dula=1;
	dula=0;
}