#include<reg52.h>
#define uchar unsigned char
sbit led1=P1^0;
uchar num;

int main()
{
	TMOD=0x01;
	TH0=(65536-45872)/256;  	  //��ʱ����Ϊ50ms
	TL0=(65536-45872)%256;
	EA=1;						  //�����ж�
	ET0=1;						  //����ʱ��0�ж�
	TR0=1;						  //������ʱ��0
	while(1)
	{
		if(20==num)
		{
			num=0;
			led1=~led1;			   //�����״̬ȡ��
		}
	}
}

void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;
	num++;
}