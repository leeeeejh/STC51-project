#include<reg52.h>
#define uchar unsigned char 
#define uint unsigned int

sfr WDT_CONTR=0xe1;		//���嵥Ƭ���¼���Ŀ��Ź��Ĵ���,
sbit led1=P1^0;			

void delayms(uchar);	//ms����ʱ����

void main()
{
	WDT_CONTR=0x35;	   	//D7  D6    D5     D4       D3    D2  D1   D0
				 		//--  -- EN_WDT CLR_WDT IDLE_WDT PS2  PS1  PS0
						//0	  0		1	   1	    0	  1	   0	1
	led1=0;				//����led
	delayms(500);
	led1=1;				//Ϩ��led
	while(1)
	{
		delayms(1000);	//1s��ʱ
		WDT_CONTR=0x35;
	}
}

/*ms����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}	
