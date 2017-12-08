#include<reg52.h>
#define uchar unsigned char 
#define uint unsigned int

void init();			//����ͨ�ŷ����ʼ������
void matrixkeyscan();	//�������ɨ�躯��
void send(uchar);		//���п�ͨ���������
void delayms(uint);		//һ����ʱ����
			
void main()
{
	init();				//��ʼ��
	while(1)
	{
		matrixkeyscan();	 //�������ɨ�����
	}
}		

/*����ͨ�ŷ����ʼ������*/
void init()
{
	TMOD=0x20;		//����T1��ʱ��������ʽ2,8λ�Զ�����
	TH1=253;		//9600������
	TL1=253;
	TR1=1;			//������ʱ��1
	
	SM0=0;			//���ô��ڹ�����ʽ1
	SM1=1;
	
	EA=1;			//�����ж�
	ES=1;			//�������ж�
}

/*�������ɨ�����*/
void matrixkeyscan()
{
	uchar temp,key;			 //temp���ڻ�ȡ������Ϣ,key���ڴ���
	P3=0xfe;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);		 //��ʱ����
		temp=P3;			 
		temp=temp&0xf0;		 
		if(temp!=0xf0)		 //�����жϷ���,����ȶ���
		{
			temp=P3;		 //��ȡ�����ϵ�ֵ
			switch(temp)
			{
				case 0xee:	key=1;	break;
				case 0xde:	key=2;	break;
				case 0xbe:	key=3;	break;
				case 0x7e:	key=4;	break;
			}
			while(temp!=0xf0)  //�ȴ������ͷ�
			{
				temp=P3;
				temp=temp&0xf0;
			}
			send(key);	   	   //�ͳ�������ͨ��
		}
	}
	
	P3=0xfd;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xed:	key=5;	break;
				case 0xdd:	key=6;	break;
				case 0xbd:	key=7;	break;
				case 0x7d:	key=8;	break;
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
			send(key);
		}
	}
   
	P3=0xfb;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xeb:	key=9;	break;
				case 0xdb:	key=10;	break;
				case 0xbb:	key=11;	break;
				case 0x7b:	key=12;	break;
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
			send(key);
		}
	}

	P3=0xf7;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xe7:	key=13;	break;
				case 0xd7:	key=14;	break;
				case 0xb7:	key=15;	break;
				case 0x77:	key=16;	break;
			}
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
			send(key);
		}
	}	
}

/*һ����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

/*����ͨ�����*/
void send(uchar num)
{
	SBUF=num;
	while(!TI);
	TI=0;
}
 