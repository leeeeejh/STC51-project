#include<reg52.h>

void init();

void main()
{
	init();
	while(1)
	{
		P1=0xff;	//Ϩ�����еĵ�
	}
}

/*�жϷ������⹦�ܼĴ�������*/
void init()
{
	EX0=1;			//���ⲿ�ж�0,�ӿ�P3^2
	IT0=0;			//�͵�ƽ���䷽ʽ
	EA=1;			//�����ж�
}

/*�жϷ������*/
void int0() interrupt 0
{
	P1=0;
}