#include<reg52.h>
#define uint unsigned int 
#define uchar unsigned char
sbit led1=P1^0;
void delay(uint s);
void main()
{
	uint cycle=600,pwm=0;              //��������
	while(1)
	{
		led1=1;
		delay(60000);				   //��״̬��ʱ�����Կ���Ϩ��Ĺ���
		for(pwm=1;pwm<cycle;pwm++)	   //�������ɰ������仯
		{
			led1=0;					   //�������Էǳ�����ٶȿ�-��-��-�أ�
			delay(pwm);				   //���𽥼Ӵ󿪵�ʱ����ʣ���С�ص�
			led1=1;					   //ʱ����ʣ��Ӷ�ʱ���ɰ������ı仯��
			delay(cycle-pwm);
		}	
		led1=0;
		for(pwm=cycle-1;pwm>0;pwm--)   //���������������仯
		{
			led1=0;
			delay(pwm);
			led1=1;
			delay(cycle-pwm);
		}
	}
}	

/*��ʱ����*/
void delay(uint s)
{
	while(--s);
}