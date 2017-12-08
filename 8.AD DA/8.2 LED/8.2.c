#include<reg52.h>    		//����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h> 		//����NOP��ָ���_nop_();

#define uchar unsigned char 
#define uint unsigned int
#define PCF8591_ADD 0x90   //PCF8591��ַ 

sbit SDA=P2^0;     	  	  //i2c��������
sbit SCL=P2^1;		      //i2cʱ������

void delayms(uchar xms);  				//һ����ʱ����
void init();							//�����ʼ��
void i2c_start();						//i2c��������
void i2c_stop();						//i2cֹͣ����
void DAC_ACK();						    //DAC��i2cӦ���ź�
void i2c_sendbyte(uint bt);				//����һ�ֽ�����
void DAC(uchar);						//DA��ģת������


void main()
{
	uchar num;            //DA��ģ�������
    init();				  //������ʼ��
    while(1)
    { 
       DAC(num);          //DA�������LEDģ���ѹ�仯
       num++;             //�ۼӣ���256�������Ϊ0������ѭ������ʾ��LED�������𽥱仯
       delayms(20);       //��ʱ�������������仯
    }
}

/*һ����ʱ*/
void delayms(uchar xms)
{
  uint i,j;
  for(i=xms;i>0;i--)
     for(j=114;j>0;j--);
}

/*��ʼ��*/
void init()
{
    TMOD = 0x20;        // ��ʱ��1�����ڷ�ʽ2���Զ���װ��
    TH1 = 253;
    TL1 = 253;			//9600������
	TR1 = 1;			//����T1��ʱ��

	SM0=0;
	SM1=1;				//���ڹ�����ʽ1, 10λ�첽
}

/*����i2c,��ʼ�ź�*/
void i2c_start()		//SCL�ڸߵ�ƽ�ڼ�,SDAһ���½��������ź�	  
{
	SCL=1;
	_nop_();
	SDA=1;
	_nop_();
    SDA=0;
	_nop_();
	SCL=0;
	_nop_();
}

/*ֹͣi2c*/
void i2c_stop()			//SCL�ڸߵ�ƽ��ʱ��,SDAһ��������ֹͣ�ź�
{
	SCL=0;
	_nop_();
	SDA=0;
	_nop_();						
	SCL=1;
	_nop_();
	SDA=1;
	_nop_();	
}

/*i2cӦ���ź�*/
void DAC_ACK()		   //SCL�ڸߵ�ƽ�ڼ�,SDA�����豸��Ϊ�͵�ƽ��ʾӦ��
{
	SDA=1;
	_nop_();
	SCL=1;
	_nop_();
	SCL=0;		
}

/*�������һ�ֽ�����bt����*/
void i2c_sendbyte(uint bt)
{
	uint i;
	for(i=0;i<8;i++)
	{
		if(bt&0x80)
			SDA=1;
		else 
			SDA=0;
		_nop_();
		SCL=1;						//SCLΪ�ߵ�ƽ,�ôӻ���SDA�а����������ݶ���
		bt<<=1;						//��bt����һλ
		_nop_();
		SCL=0;						//����SCL,ռ�����߼���ͨ��
	}
}

/*д��DA��ģת��ֵ*/
void DAC(uchar date)
{
	i2c_start();					//����
	i2c_sendbyte(PCF8591_ADD+0);	//������,д��оƬ��ַ   
    DAC_ACK();		   	    		//ACKӦ��
	i2c_sendbyte(0x40);	     		//д�����λ��ʹ��DAC���
    DAC_ACK();		   	 			//ACKӦ��             
    i2c_sendbyte(date);  			//д����
    DAC_ACK();               		//ACKӦ��
    i2c_stop();                     //ֹͣ           
}