#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit dula=P2^6;
sbit wela=P2^7;				    
uchar code weitable[]={		       //λѡ��ʾ����
0x7f,0xbf,0xdf,0xef,		    
0xf7,0xfb,0xfd,0xfe};
uchar code dutable[]={		       //��ѡ��ʾ����
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void delayms(uint xms);
void main()
{
	uchar num;
	while(1)
	{
		for(num=1;num<=8;num++)
		{
			dula=1;
			P0=dutable[num];		//�Ͷ�ѡ
			dula=0;
			P0=0xff;				//��ѡ����Ӱ
			wela=1;
			P0=weitable[num-1];		//��λѡ
			wela=0;
			delayms(500);			//��ÿ����ʾ����������Ϊ500,��һ����ʾ����������Ϊ1
		}
	}
}

void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
                                                      