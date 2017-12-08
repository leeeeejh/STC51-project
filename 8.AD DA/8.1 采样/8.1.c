#include<reg52.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define PCF8591_ADD 0x90	//PCF8591��ַ

sbit SDA=P2^0;			 	//i2c��������
sbit SCL=P2^1;				//i2cʱ������

void delayms(uint xms);						  //һ����ʱ����
void i2c_start();							  //i2c��������
void i2c_stop();							  //i2cֹͣ����
void i2c_ACK(bit ck);						  //i2cӦ���ź�
bit i2c_waitACK();							  //i2cӦ������ж��ź�
void i2c_sendbyte(uint bt);					  //����һ�ֽ�����
uint i2c_recbyte();							  //����һ�ֽ�����
bit i2c_readbyte(uint address, uint *dat);	  //��ȡһ�ֽ�����
void init(void);							  //�����ʼ��

void main()
{
	uint ans;								 //SBUF����
	init();									 //�����ʼ��
	while(1)
	{
		i2c_readbyte(0x43,&ans);			 //������������ݲ���ֵ��ans
		SBUF = ans;							 
        while (!TI);						 //�ж�SBUF�Ƿ��ͳɹ�
        TI = 0;								 //����
		delayms(1000);						 //1s��ʱ
	}
}

/*һ����ʱ*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}

/*����i2c,��ʼ�ź�*/
void i2c_start()							 //SCL�ڸߵ�ƽ�ڼ�,SDAһ���½��������ź�	  
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
void i2c_stop()								//SCL�ڸߵ�ƽ��ʱ��,SDAһ��������ֹͣ�ź�
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
/*ckΪ1ʱ����Ӧ���ź�,
  ckΪ0ʱ������*/
void i2c_ACK(bit ck)					   //SCL�ڸߵ�ƽ�ڼ�,SDA�����豸��Ϊ�͵�ƽ��ʾӦ��
{
	if(ck)
		SDA=0;
	else 
		SDA=1;
	_nop_();
	SCL=1;
	_nop_();
	SCL=0;
	_nop_();
	SDA=1;
	_nop_();
}

/*i2cӦ���źŽ��շ����ź�*/
/*����Ϊ1ʱ���յ�ACK
  ����Ϊ0ʱû�н��յ�ACK*/
bit i2c_waitACK()
{
	SDA=1;
	_nop_();
	SCL=1;
	_nop_();
	if(SDA)
	{
		SCL=0;
		i2c_stop();
		return 0;
	}
	else
	{
		SCL=0;
		return 1;
	}		
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

/*�������Ͻ���1�ֽ�����*/
uint i2c_recbyte()
{
	uint date,i;
	for(i=0;i<8;i++)
	{
		SCL=1;						//SCLΪ�ߵ�ƽ,�ôӻ���SDA�а����������ݶ���
		_nop_();					 
		date<<=1;					//���ƽ�λ,����һλ���ֽ��в���
		if(SDA)
		{
			date=date|0x01;		   	//��ֵ,���յ�Ϊ1,û��Ϊ0
		}
		SCL=0;						//����SCL,ռ�����߼���ͨ��
		_nop_();
	}
	return date;
}

/*����һ�ֽ�����*/
/*����1�ɹ�,  
  ����0ʧ��*/
bit i2c_readbyte(uint address, uint *dat)
{
	i2c_start();					 //����
	i2c_sendbyte(PCF8591_ADD+0);	 //������ д
	if(!i2c_waitACK())				 //ACKӦ��
		return 0;
	i2c_sendbyte(address);			 //�洢��Ԫ��ַ
	if(!i2c_waitACK())				 //ACKӦ��
		return 0;
	i2c_start();					 //����
	i2c_sendbyte(PCF8591_ADD+1);	 //������ ��
	if(!i2c_waitACK())				 //ACKӦ��
		return 0;
	*dat=i2c_recbyte();				 //��������
	i2c_ACK(0);						 //ֹͣӦ��
	i2c_stop();						 //ֹͣ
	return 1;
}

/*��ʼ��*/
void init(void)
{
    TMOD = 0x20;        // ��ʱ��1�����ڷ�ʽ2���Զ���װ��
    TH1 = 253;
    TL1 = 253;			//9600������
	TR1 = 1;			//����T1��ʱ��

	SM0=0;
	SM1=1;				//���ڹ�����ʽ1, 10λ�첽
}
