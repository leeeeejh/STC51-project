#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned  char
#define MAIN_Fosc	11059200UL	//�궨����ʱ��HZ

sbit dula=P2^6;	  //����ܶ�ѡ
sbit wela=P2^7;	  //�����λѡ
sbit DS=P2^2;	  //DS18B20������(�ź���)

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

uchar code wetable[]={		//�����λѡ��
//��1λ 2λ	 3λ   4λ       
0xfe, 0xfd, 0xfb, 0xf7, 
//5λ  6λ	 7λ   8λ
0xef, 0xdf, 0xbf, 0x7f};

bit DS_init();	  //�����߳�ʼ������
uchar DS_read_byte();	//DS18B20������
void DS_write_byte(uchar date);	//DS18B20д����
void yj_display(uint num);	//Һ����ʾ����
void delayms(uint xms);		//ms����ʾ����
void delayus(uchar xus);	//us����ʾ����

/*******************************************************
********************************************************
*******************************************************/
void main()
{
	uint i;
	uchar L,M;	 //L:�¶�ֵ��λ,M:�¶�ֵ��λ
	while(1)
	{
		DS_init();	 //��ʼ��DS18B20
		DS_write_byte(0xcc);//����ROM
		DS_write_byte(0x44);//�¶�ת��
		
		DS_init();	 //��ʼ��DS18B20
		DS_write_byte(0xcc);//����ROM
		DS_write_byte(0xbe);//���ݴ���
		L=DS_read_byte();
		M=DS_read_byte();
		i = M;		 //д���¶�ֵ��λ
		i <<= 8;	 //�Ƶ���λֵ����λ
		i |= L;		 //�����λֵ
		i = i* 0.0625;//ת��Ϊʮ����
		i=i*10+0.5;	//��С�����һλ�����Ƶ�С����ǰ,����������С�����ڶ�λ
		yj_display(i);
	}

}


/*******************************************************
*********************DS18B20����************************
*******************************************************/
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


/*******************************************************
********************Һ����ʾ����************************
*******************************************************/
/*Һ����ʾ*/
void yj_display(uint num)
{
	dula=0;						 
	P0=dutable[num/100];		 //ʮλ��
	dula=1;
	dula=0;
	wela=0;
	P0=0xfe;
	wela=1;
	wela=0;
	delayms(5);

	dula=0;
	P0=dutable[num%100/10]|0x80; //��λ����ʾ��С����
	dula=1;						 //0x80��ʾС����
	dula=0;
	wela=0;
	P0=0xfd;
	wela=1;
	wela=0;
	delayms(5);

	dula=0;
	P0=dutable[num%10];			 //С��λ
	dula=1;
	dula=0;
	wela=0;
	P0=0xfb;
	wela=1;
	wela=0;
	delayms(5);
}


/*******************************************************
*********************��ʱ����***************************
*******************************************************/
/*ms����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}

/*us����ʱ����,*/
void delayus(uchar xus)	//ִ��һ��6.5us,����һ��11.95us
{
	while(xus--);
}