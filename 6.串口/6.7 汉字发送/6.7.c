#include<reg52.h>
#include<stdio.h>		    //printf����ͷ�ļ�

#define uchar unsigned char 
#define uint unsigned int

void init();
void delayms(uint xms);

void main()
{
	init();
	while(1)
	{
		TI=1;				 //printf����ʹ��ǰ����Ҫ��TI��1
		printf("���ݵ��ӿƼ���ѧ��ӭ��!\n");
		delayms(100);		 //��ʱ��ʹ�����������׿���
		puts("������Ϣ����ѧԺ��ӭ�㣡");
		while(!TI);
		TI=0;
		delayms(2000);		 //��ʱ��ʹ�����������׿���
	}
}

/*���ڳ�ʼ��*/
void init()
{
	TMOD=0x20;			 //T1����ģʽΪ2 8λ�Զ���װ
	TH1=253;			 
	TL1=253;			 //������9600
	TR1=1;				 //����T1��ʱ��

	SM0=0;
	SM1=1;				 //���ڹ�����ʽ1 10λ�첽
}		 

/*��ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}