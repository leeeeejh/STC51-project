#include<reg52.h>
#define uint unsigned int

void init();
void delayms(uint);

void main()
{
	init();
	while(1)
	{
		P1=0Xff;
	}
}                      

/*�жϷ������⹦�ܼĴ���*/
void init()
{
	EX1=1;			//���ⲿ�ж�1���ӿ�P3^3
	IT1=1;			//�����䴥����ʽ
	EA=1;			//�����ж�
}

/*�жϷ������*/
void int1() interrupt 2
{
	P1=0;
	delayms(500);		//��ʱ���ڹ۲�����ĸı�
}

/*��ʱ�ӳ���*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=115;j>0;j--);
}