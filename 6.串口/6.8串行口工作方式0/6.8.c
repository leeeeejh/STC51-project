/*
ͬ����λ������TXD(P3^0)�������,��RXD(P3^1)��������
���õ�Ƭ�����пڹ���ģʽ0(��ͬ������ͨ��,������
�����ͬ����λ�Ĵ�������.),���ģʽ����0xaa,
��˫ʾ�����۲�P3^0��P3^1�ڲ���.
*/

#include<reg52.h>
#define uchar unsigned char 

void init();
void delayms(uchar);

void main()
{
	init();			//���пڳ�ʼ������
	TI=0;			//�����жϱ�־λ����
	while(1)
	{	   
		SBUF=0xaa;
		delayms(1);	//1ms��ʱ
	}
}

/*���п��жϷ���*/
void ser0() interrupt 4
{
	TI=0;
}

void init()
{
	SCON=0;			//SCON: SM0 SM1 SM2 REN TB8 RB8 TI  RI 
                    //50h =  0   0   0   0   0   0  0   0 b
					//���ô��ڹ�����ʽΪ0
	EA=1;			//�����ж�
	ES=1;			//�������ж�
}

void delayms(uchar xms)
{
	uchar i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}