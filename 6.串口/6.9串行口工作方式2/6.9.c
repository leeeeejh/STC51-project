/*
��ʽ2�ͷ�ʽ3��Ϊ11λ�첽ͨ�ſ�,����λΪ9λ,��һλ����
λTB8��RB8.
2��3Ψһ���������ڴ������ʲ�ͬ
*/

/*
���õ�Ƭ�����пڹ���ģʽΪ2,���ѭ������ʮ��������
0xaa,Ȼ����ʾ�����۲쵥Ƭ��P3^1�ڲ���.
ͨ���޸�TB8��ֵ,Ϊ0��1,�Ƚϲ���
*/

#include<reg52.h>
#define uchar unsigned char

void init();		 //���пڷ����ʼ������
void delayms(uchar); //һ����ʱ����

void main()
{
	init();
	TI=0;
	{
		SBUF=0xaa;
		delayms(1);

	}
}

void ser0() interrupt 4
{
	TI=0;
}
/*���пڷ����ʼ������*/
void init()
{
	SM0=1;
	SM1=0;			//���ô��пڹ�����ʽ2
	TB8=0;			//��������ʱ���ӵĵھ�λΪTB8,
					//��������ʱSCON�еĵھ�λΪRB8;

	EA=1;			//�����ж�
	ES=1;			//�������ж�
}

/*һ����ʱ����*/
void delayms(uchar xms)
{
	uchar i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}