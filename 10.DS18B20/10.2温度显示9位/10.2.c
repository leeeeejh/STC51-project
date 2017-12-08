#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define MAIN_Fosc		11059200UL	//�궨����ʱ��HZ

sbit dula=P2^6;
sbit wela=P2^7;
sbit DS=P2^2;

uchar code dutable[]={		//����������ܶ�ѡ��
//0   1    2	3 
0x3f,0x06,0x5b,0x4f,
//4   5	   6 	7
0x66,0x6d,0x7d,0x07,
//8   9	   A	b
0x7f,0x6f,0x77,0x7c,
//C   d	   E	F
0x39,0x5e,0x79,0x71,
//-   .  ����ʾ
0x40,0x80,0x00};

bit DS_init();	  //�����߳�ʼ������
uchar DS_read_byte();	//DS18B20������
void DS_write_byte(uchar date);	//DS18B20д����
void yj_display(uint num);//Һ����ʾ����
void delayms(uint xms);	  //ms����ʱ����
void delayus(uchar xus);  //us����ʱ����


/*******************************************************
********************************************************
*******************************************************/
void main()
{
	uint i;
	uchar L,M;
	DS_init();//��ʼ��DS18B20
	DS_write_byte(0xcc);//����ROM
	DS_write_byte(0x4e);//д�Ĵ���
	DS_write_byte(0x30);//���ϵ���д�������¶���������
	DS_write_byte(0x40);//�����¶���������
	DS_write_byte(0x1f);//���ù�����9λģʽ��(0001 1111)
	DS_init();//��ʼ��DS18B20
	DS_write_byte(0xcc);//����ROM 
	DS_write_byte(0x48);//�����ĺ��2,3�ֽ��е����ݸ��Ƶ�E2PROM
	while(1)
	{
		DS_init();//��ʼ��DS18B20
		DS_write_byte(0xcc);//����ROM
		DS_write_byte(0x44);//�¶�ת��

		DS_init();//��ʼ��DS18B20
		DS_write_byte(0xcc);//����ROM
		DS_write_byte(0xbe);//���ݴ���
		L=DS_read_byte();
		M=DS_read_byte();
		i = M;			//д���¶�ֵ��λ
		i <<= 8;		//�Ƶ���λֵ����λ
		i |= L;			//�����λֵ
		i = i *0.0625;  //ת��Ϊʮ����
		i = i *	10+0.5;	//��С�����һλ�����Ƶ�С����ǰ,����������С�����ڶ�λ
		
		yj_display(i);
	}	
}


/******************************************************
*****************DS18B20******************************
******************************************************/
/*�����߳�ʼ������*/
bit DS_init()
{
	 bit i;
	DS=1;
	_nop_();
	DS=0;
	delayus(100);	//��������661.95us,�ҽ��������ϵ�DS18B20����ȫ����λ
	DS=1;			//�ͷ�����
	delayus(5);		//��ʱ44.45us,�ȴ�DS18B20���ش����ź�
	i=DS;
	delayus(65);	//��ʱ434.35us
	DS=1;
	_nop_();
	return(i);
}

/*DS18B20д����*/
void DS_write_byte(uchar date)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		DS=0;
		_nop_();	//����дʱ��
		DS=date&0x01;//���ӵ�λ����λ��˳��������
		delayus(10);//��ʱ76.95us
		DS=1;//�ͷ�����׼����һ������д��
		_nop_();
		date >>= 1;//����׼�����뵹���ڶ�λ
	}
}

/*DS18B20������*/
uchar DS_read_byte()
{
	uchar i,j,date;
	for(i=0;i<8;i++)
	{
		DS=0;	
		_nop_();	//������ʱ��
		DS=1;
		_nop_();	//�ͷ�����
		j=DS;	//�õ����λ
		delayus(10);//��ʱ76.95us
		DS=1;
		_nop_();
		date=(j<<7)|(date>>1);//���»�õ�λ�Ƶ����λ,��ԭ�������λ����һλ
	}
	return (date);
}


/******************************************************
*****************Һ����ʾ����**************************
******************************************************/
void yj_display(uint num)
{
	dula=0;
	P0=dutable[num/100];
	dula=1;
	dula=0;
	wela=1;
	P0=0xfe;
	wela=1;
	wela=0;
	delayms(5);

	dula=0;
	P0=dutable[num%100/10]|0x80;
	dula=1;
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delayms(5);

	dula=0;
	P0=dutable[num%10];
	dula=1;
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delayms(5);
}


/******************************************************
*****************��ʱ����******************************
******************************************************/
/*ms����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}

/*us����ʱ����*/
void delayus(uchar xus)
{
	while(xus--);
}