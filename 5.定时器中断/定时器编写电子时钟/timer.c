/********************************************************************************
�����ö�ʱ����д����ʱ�ӳ����趨�������ܼ���һ�����ܼ���һ����ֵ�������һ����
ֵ��С�����趨ʱ���롣�����ܽ����ݴ��뵽AT2404�У��ٴ�������Դ�����ӱ�Ӷϵ�ʱ�̿�ʼ
������ʱ
**********************************************************************************/
#include <reg52.h>
#define uchar unsigned char 
#define uint unsigned int 
sbit lcdrs=P3^5;		   //RSҺ����������ѡ���
sbit lcdrw=P3^6;		   //WRҺ����дѡ���
sbit lcden=P3^4;		   //LCDENҺ��ʹ�ܶ�
sbit dula=P2^6;			   //��ѡ
sbit wela=P2^7;			   //λѡ
sbit beep=P2^3;			   //������

sbit s1=P3^0;			   //��Ϊ����ѡ���
sbit s2=P3^2;  			   //��Ϊ��ֵ�����
sbit s3=P3^3;			   //��Ϊ��ֵ��С��

uchar shuzi[10]="0123456789";
uchar table[14]="A SIMPLE CLOCK";

char hour=12 ,min=0,sec=0;//����ʱ����λ

uchar time=0,addr;  //addr:���ѡ��λ

void init();				   //��ʱ����1602��ʾ����ʼ���趨����
void write_com(uchar);		   //Һ������д��
void write_data(uchar);		   //Һ������д��
void display(uchar,uchar);	   //Һ����ʾ����
void keyscan();				   //�������������޸ĺ���
void beeper();				   //����������
void delayms(uchar);				   //һ����ʱ����

void main ()
{
	uchar i;
	init ();
	write_com (0x80+0x01);
	for (i=0;i<14;i++)
	{
		write_data(table[i]);	
	}
	display(0x80+0x44,hour);
	write_data(':');
	display(0x80+0x47,min);
	write_data(':');
	display(0x80+0x4a,sec);
	while (1)
	{
		keyscan ();	
	}
}

/*�жϷ���*/
void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;		  //50ms,��װ��ֵ
	TL0=(65536-45872)%256;
	time++;
	if (time==20)				  //1s
	{
		time=0;					  //��ʱ����������
		sec++;					  //��������һλ
		if (sec==60)			  //1min
		{
			sec=0;
			min++;
			if (min==60)		  //1h
				{
					min=0;
					hour++;
					if (hour==24)
						{hour=0;}
					display(0x80+0x44,hour);//ʱ�仯������д��ʱ
				}
			display(0x80+0x47,min);//�ֱ仯������д���
		}
		display(0x80+0x4a,sec);//��仯������д����
	}
}

/*��ʱ����1602��ʾ����ʼ���趨����*/
void init ()
{
	dula=0;		 	
	wela=0;			//�رն�ѡ��λѡ,��ֹ����ܵ���
	lcden=0;		//�ر�Һ��ʹ�ܶ�

	TMOD=0X01;					  //���ö�ʱ��0������ʽ1
	TH0=(65536-45872)/256;		  //50ms
	TL0=(65536-45872)%256;
	EA=1;						  //�����ж�
	ET0=1;						  //����ʱ��0�ж�
	TR0=1;						  //������ʱ��0
	
	write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
}

/*Һ������д��*/
void write_com (uchar com)
{
	lcdrw=0;		   //ѡ��д
	lcdrs=0;		   //ѡ������
	P0=com;
	delayms(5);		   //��ʱʹҺ�������ȶ�
	lcden=1;		   //�����彫��������Һ��������
	delayms(5);
	lcden=0;
}

/*Һ������д��*/
void write_data(uchar date)
{
	lcdrw=0;		  //ѡ��д
	lcdrs=1;		  //ѡ������
	P0=date;		  
	delayms (5);	  //��ʱʹҺ�������ȶ�
	lcden=1;		  //�����彫��������Һ��������
	delayms (5);
	lcden=0;
}

/*һ����ʱ����*/
void delayms(uchar xms)
{
	uchar i,j;
	for (i=xms;i>0;i--)
		for (j=114;j>0;j--);

}

/*�������������޸ĺ���*/
void keyscan ()
{
	if (s1==0)
		{
			delayms (10);		   //��ʱ����
			if (s1==0)			   //�ظ��ж�,����
				{
					addr++;		   //Һ��ѡ��λ�仯
					beeper();	   //��������ʱ��������0.1s,��Ϊ����
					while (!s1);   //�ȴ������ָ�
										
					if (addr==1)
						{
							TR0=0;
							write_com(0x80+0x4b);	 //����Ƶ���λ��(�ڶ���12λ)
							write_com(0x0f);		 //�����˸
						}
					if (addr==2)
						{
							write_com(0x80+0x48);	 //����Ƶ���λ��(�ڶ���9λ)
							write_com(0x0f);		 //�����˸
						}
					if (addr==3)					 
						{
							write_com(0x80+0x45);	  //����Ƶ�ʱλ��(�ڶ���6λ)
							write_com(0x0f);		  //�����˸
						}
					if (addr==4)
						{
							addr=0;				  	  //���ѡ��λ����(ȡ��ѡ��)
							write_com(0x0c);		  //����ʾ,��ȡ�������˸
							TR0=1;					  //����ʱ��,��ʼ��
						}
				}
		}
	if (s2==0&&addr!=0)//ֻ�й��ܼ�������֮����ֵ����������
		{
			delayms (10);			//��ʱ����
			if (s2==0)				//�ظ��ж�,����
				{	
					beeper ();		//��������ʱ��������,��Ϊ����
					while (!s2);	//�ȴ������ָ�
					if (addr==1)	//���ѡ��λ����λ
 						{
							sec++;			 
							if (sec==60)
								sec=0;
							display(0x80+0x4a,sec);//��仯ʱ����д��
							write_com(0x80+0x4b);//д��󽫹���ƻز���˸����ʾΪ����״̬
						}
					if (addr==2) 	 //���ѡ��λ�ڷ���λ
						{
							min++;
							if (min==60)
								min=0;	
							display(0x80+0x47,min);//�ֱ仯ʱ����д��
							write_com(0x80+0x48);//д��󽫹���ƻز���˸����ʾ����״̬
						}
					if (addr==3) 	 //���ѡ��λ��Сʱλ
						{
							hour++;
							if (hour==24)
								hour=0;
							display(0x80+0x44,hour);//ʱ�仯ʱ����д��	
							write_com(0x80+0x45);//д��󽫹���ƻز���˸����ʾ����״̬
						}
				}
		}

	if (s3==0&&addr!=0)//ֻ�й��ܼ�������֮����ֵ����������
		{
			delayms (10);		   //��ʱ����
			if (s3==0)			   //�ظ��ж�,����
				{	
					beeper ();	   //��������ʱ��������,��Ϊ����
					while (!s3);   //�ȴ������ָ�
					if (addr==1)
 						{
							sec--;
							if (sec==-1)
								sec=59;
							display(0x80+0x4a,sec);//��仯ʱ����д��
							write_com(0x80+0x4b);//д��󽫹���ƻز���˸����ʾ����״̬
						}
					if (addr==2) 
						{
							min--;
							if (min==-1)
									min=59;
							display(0x80+0x47,min);//�ֱ仯ʱ����д��
							write_com(0x80+0x48);//д��󽫹���ƻز���˸����ʾ����״̬
						}
					if (addr==3) 
						{
							hour--;
							if (hour==-1)
								hour=23;
							display(0x80+0x44,hour);//ʱ�仯ʱ����д��
							write_com(0x80+0x45);//д��󽫹���ƻز���˸����ʾ����״̬
						}
				}
		}
}

/*����������*/
void beeper()
{
	beep=0;				//����������
	delayms(100);		//0.1s��ʱ
	beep=1;				//�رշ�����
}

/*Һ����ʾ����*/
void display(uchar add,uchar date)//��ʾʱ���뺯����addʲôλ�� date��ʾʲô
{
	uchar shi,ge;
	shi=date/10;	
	ge=date%10;

	write_com(add);
	write_data(shuzi[shi]);
	write_data(shuzi[ge]);
}


