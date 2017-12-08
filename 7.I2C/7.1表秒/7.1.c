#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define AT24C02_ADD 0xa0            //AT24C02��ַ

sbit dula=P2^6;						//��ѡ
sbit wela=P2^7;						//λѡ
sbit SDA=P2^0;						//ʱ������
sbit SCL=P2^1;						//��������

uchar code dutable[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

void I2C_init();
void init();
void delay();
void I2C_start();
void I2C_stop();
void I2C_response();
void write_byte(uchar date);
uchar read_byte();
void write_add(uchar address,uchar date);
uchar read_add(uchar address);
void display(uchar shi,uchar ge);
void delayms(uchar xms);

uchar sec,tcnt;
bit write=0;		//д24CO2�ı�־

void main()
{
	I2C_init();
	sec=read_add(2);				//������������ݸ���sec
	if(sec>100)						//��ֹ�״ζ�ȡ����������
		sec=0;
	init();
	while(1)
	{
		display(sec/10,sec%10);
		if(1==write)				//�ж�T0�Ƿ��ʱ��1��
		{
			write=0;				//����
			write_add(2,sec);		//��24C02�ĵ�ַ2��д������sec	
		}
	}	
}

/*T0��ʱ����ʱ�жϷ�����*/
void T0_time() interrupt 1
{
	TH0=(65536-45872)/256;
	TL0=(65536-46872)%256;			 //50ms
	tcnt++;
	if(20==tcnt)					 //1s
	{
		tcnt=0;
		sec++;						 
		write=1;					 //1��дһ��24C02
		if(100==sec)
			sec=0;		
	}
}

/*I2C��ʼ��*/
void I2C_init()			//�����߶�����,���ͷ�����	   
{
	SDA=1;
	delay();
	SCL=1;					   
	delay();
}

/*�ж��������Ĵ�����ʼ��*/
void init()
{
	TMOD=0x01;				   //��ʱ��0������ʽΪ1
	TH0=(65536-45872)/256;
	TL0=(65536-45872)%256;	   //50ms
	ET0=1;					   //����ʱ��0�ж�
	TR0=1;					   //������ʱ��0�ж�
	EA=1;					   //�����ж�
}

/*5us������ʱ*/
void delay()
{
	_nop_();				   //һ���������ڵ���ʱ
}

/*I2C��ʼ�ź�*/
void I2C_start()		 //SCL�ڸߵ�ƽ�ڼ�,SDAһ���½��������ź�	  
{
	SDA=1;
	delay();
	SCL=1;
	delay();
	SDA=0;
	delay();
}

/*I2Cֹͣ�ź�*/
void I2C_stop()		    //SCL�ڸߵ�ƽ�ڼ䣬SDAһ��������ֹͣ�ź�
{
	SDA=0;
	delay();
	SCL=1;
	delay();
	SDA=1;
	delay();
}

/*I2CӦ���ź�*/
void I2C_response()		//SCL�ڸߵ�ƽ�ڼ䣬SDA�����豸��Ϊ�͵�ƽ��ʾӦ��
{
	uchar i;
	SCL=1;
	delay();
	while((1==SDA)&&(i<255))
		i++;
	SCL=0;
	delay();
}

/*д�ֽ�*/
void write_byte(uchar date)
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;		  //��temp����һλ�����λ���Ƶ�PSW�Ĵ�����CYλ��
		SCL=0;				  //SCLΪ�͵�ƽʱSDA������仯
		delay();			  //�������ȶ�
		SDA=CY;				  //��CY��ֵ��SDA������SCL�Ŀ����·��ͳ�ȥ
		delay();			  //�������ȶ�
		SCL=1;				  //SCLΪ�ߵ�ƽ���ôӻ���SDA�а����������ݶ���
		delay();
	}
	SCL=0;					  //����SCL��ռ�����߼���ͨ��
	delay();
	SDA=1;					  //�ͷ�SDA
	delay();
}

/*���ֽ�*/
uchar read_byte()
{
	uchar i,k;
	SCL=0;
	delay();
	SDA=1;
	delay();
	for(i=0;i<8;i++)
	{
		SCL=1;
		delay();
		k=(k<<1)|SDA;		//��k����һλ����SDA���л����㣬���ΰ�8��������λ����һ���ֽ�������ɽ���
		SCL=0;
		delay();
	}
	return k;
}

/*д����*/
void write_add(uchar address,uchar date)
{
	I2C_start();				    //����
	write_byte(AT24C02_ADD);		//������ д	   
	I2C_response();					//Ӧ��
	write_byte(address);			//�洢��Ԫ��ַ
	I2C_response();					//Ӧ��
	write_byte(date);				//����
	I2C_response();					//Ӧ��
	I2C_stop();						//ֹͣ
}

/*������*/
uchar read_add(uchar address)
{
	uchar date;
	I2C_start();				   //����
	write_byte(AT24C02_ADD+0);	   //������ д
	I2C_response();				   //Ӧ��
	write_byte(address);		   //��Ԫ��ַ
	I2C_response();				   //Ӧ��
	I2C_start();				   //����
	write_byte(AT24C02_ADD+1);	   //������ ��
	I2C_response();				   //Ӧ��
	date=read_byte();			   //����
	I2C_stop();					   //ֹͣ
	return date;
}

/*�������ʾ*/
void display(uchar shi,uchar ge)
{
	dula=1;
	P0=dutable[shi];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfe;
	wela=0;
	delayms(5);

	dula=1;
	P0=dutable[ge];
	dula=0;
	P0=0xff;
	wela=1;
	P0=0xfd;
	wela=0;
	delayms(5);
}

/*��ʱ*/
void delayms(uchar xms)
{
	uchar i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}