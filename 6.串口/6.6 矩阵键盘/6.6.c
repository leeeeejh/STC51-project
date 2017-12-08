/*�Ѿ�����̵ļ�ֵ��2400bps�ϴ����������������*/
#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

void init();
uchar KeyPro();
uchar KeyScan();
void delayms(uchar xms);

void main()
{
	init();		     	 //���ڳ�ʼ��
	while(1)
	{
		SBUF=KeyPro();	 //���ô�����ֵ�ļ�ֵ��ת����������ת����ļ�ֵ�����뷢��SBUF
		while(!TI);		 //�ȴ��������
		TI=0;			 //����
	}
}

void init()
{
	TMOD=0x20;		   //T1������ʽ2 8λ�Զ���װ
	TH1=244;		   
	TH1=244;		   //������2400��256-11059200/2400/12/32
	TR1=1;			   //������ʱ��T1
	
	SM0=0;			  
	SM1=1;			   //���ô��ڵĹ�����ʽ1��10λ�첽
}

uchar KeyPro()
{
	uchar key;
	switch(KeyScan())
	{
		//��һ�м�ֵ��
		case 0xee: key=0x01;	break;
		case 0xde: key=0x02;	break;
		case 0xbe: key=0x03;	break;
		case 0x7e: key=0x04;	break;

		//�ڶ��м�ֵ��
		case 0xed: key=0x05;	break;
		case 0xdd: key=0x06;	break;
		case 0xbd: key=0x07;	break;
		case 0x7d: key=0x08;	break;

		//�����м�ֵ��
		case 0xeb: key=0x09;	break;
		case 0xdb: key=0x0a;	break;
		case 0xbb: key=0x0b;	break;
		case 0x7b: key=0x0c;	break;

		//�����м�ֵ��
		case 0xe7: key=0x0d;	break;
		case 0xd7: key=0x0e;	break;
		case 0xb7: key=0x0f;	break;
		case 0x77: key=0x10;	break;
	}
	return key;
}

uchar KeyScan()
{
	uchar lie,hang;		   //�������ߺ����ߵĴ������
	P3=0xf0;			   //1111 0000
	if((P3&0xf0)!=0xf0)	   //�ж��Ƿ��а�������
	{
		delayms(5);		   //�����Ӱ
		if((P3&0xf0)!=0xf0) //�ж��Ƿ��а�������
		{
			lie=P3&0xf0;	 			//��������ֵ
			P3=lie|0x0f;
			hang=P3&0x0f;				//��������ֵ
			while((P3&0x0f)!=0x0f);		//���ּ��
			return(lie+hang);			//���ؼ�ֵ��
		}
	}
}

void delayms(uchar xms)
{
	uchar i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}