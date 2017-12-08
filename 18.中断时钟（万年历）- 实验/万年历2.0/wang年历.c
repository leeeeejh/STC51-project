//����TX-1Cʵ�����ʵ�ʾ���Ϊ11.0592M��
//����Ϊ�˷������ʱ��,���Ǽ��辧��Ϊ12M.
#include<reg52.h>//����52��Ƭ��ͷ�ļ�
#define uchar unsigned char 
#define uint unsigned int
#define AT24C02_ADD 0xa0            //AT24C02��ַ

bit  write=0;           //д24C02�ı�־��

sbit SDA=P2^0;						//ʱ������
sbit SCL=P2^1;						//��������

sbit dula=P2^6; //���������������
sbit wela=P2^7;

sbit lcdrs=P3^5;		   //RSҺ����������ѡ���
sbit lcdrw=P3^6;		   //WRҺ����дѡ���
sbit lcden=P3^4;		   //LCDENҺ��ʹ�ܶ�

sbit s1=P3^0;   //���尴��--���ܼ�
sbit s2=P3^1;	//���尴��--���Ӽ�
sbit s3=P3^2;	//���尴��--��С��
sbit s4=P3^3;			   //ģʽ�л���

sbit beep=P2^3; //�����������

uchar count,s1num;
char miao,shi,fen;
uchar code table[]="  WORK CLOCK";//�����ʼ�ϵ�ʱҺ��Ĭ����ʾ״̬

void delay()
{ ;; }

/*����i2c,��ʼ�ź�*/
void i2c_start()		//SCL�ڸߵ�ƽ�ڼ�,SDAһ���½��������ź�	  
{
	SCL=1;
	delay();
	SDA=1;
	delay();
    SDA=0;
	delay();
	SCL=0;
	delay();
}

/*ֹͣi2c*/
void i2c_stop()			//SCL�ڸߵ�ƽ��ʱ��,SDAһ��������ֹͣ�ź�
{
	SCL=0;
	delay();
	SDA=0;
	delay();						
	SCL=1;
	delay();
	SDA=1;
	delay();	
}

/*i2cӦ���ź�*/
void i2c_respons()  				//SCL�ڸߵ�ƽ�ڼ�,SDA�����豸��Ϊ�͵�ƽ��ʾӦ��
{
	uchar i;
	SDA=0;
	delay();
	SCL=1;
	delay();
	while((SDA==1)&&(i<250))
		i++;
	SCL=0;
	delay();
	SDA=1;
	delay();
}

/*�������һ�ֽ�����bt����*/
void i2c_sendbyte(uchar bt)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		if(bt&0x80)
			SDA=1;
		else 
			SDA=0;
		delay();
		SCL=1;						//SCLΪ�ߵ�ƽ,�ôӻ���SDA�а����������ݶ���
		bt<<=1;						//��bt����һλ
		delay();
		SCL=0;						//����SCL,ռ�����߼���ͨ��
	}
}

/*�������Ͻ���1�ֽ�����*/
uchar i2c_recbyte()
{
	uchar date,i;
	for(i=0;i<8;i++)
	{
		SCL=1;						//SCLΪ�ߵ�ƽ,�ôӻ���SDA�а����������ݶ���
		delay();					 
		date<<=1;					//���ƽ�λ,����һλ���ֽ��в���
		if(SDA)
		{
			date=date|0x01;		   	//��ֵ,���յ�Ϊ1,û��Ϊ0
		}
		SCL=0;						//����SCL,ռ�����߼���ͨ��
		delay();
	}
	return date;
}

/*ָ����ַ����һ�ֽ�����*/
char i2c_read(uchar address)
{
	uchar date;	

	i2c_start();					 //����
	i2c_sendbyte(AT24C02_ADD+0);	 //������ д
	i2c_respons();					 //Ӧ��
	i2c_sendbyte(address);			 //�洢��Ԫ��ַ
	i2c_respons();					 //Ӧ��
	i2c_start();					 //����
	i2c_sendbyte(AT24C02_ADD+1);	 //������ ��
	i2c_respons();					 //Ӧ��
	date=i2c_recbyte();				 //��������
	i2c_stop();						 //ֹͣ
	return date;
} 

/*ָ����ַд��һ�ֽ�����*/
void i2c_write(uchar address, char dat)
{
	i2c_start();					 //����
	i2c_sendbyte(AT24C02_ADD+0);	 //������ д
	i2c_respons();					 //Ӧ��
	i2c_sendbyte(address);			 //�洢��Ԫ��ַ
	i2c_respons();					 //Ӧ��
	i2c_sendbyte(dat);	 			 //����
	i2c_respons();					 //Ӧ��
	i2c_stop();						 //ֹͣ
} 

/*i2c��ʼ��*/
void i2c_init()			//�����߶�����,���ͷ�����	   
{
	SDA=1;
	delay();
	SCL=1;					   
	delay();
}

/*һ����ʱ����*/
void delayms(uchar xms)
{
	uchar i,j;
	for (i=xms;i>0;i--)
		for (j=114;j>0;j--);   //1ms
}

/*����������*/
void beeper()
{
	beep=0;				//����������
	delayms(100);		//0.1s��ʱ
	beep=1;				//�رշ�����
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

/*Һ����ʾ����*/
void display(uchar add,uchar date)//��ʾʱ���뺯����addʲôλ�� date��ʾʲô
{
	uchar shi,ge;
	shi=date/10;	
	ge=date%10;

	write_com(add);
	write_data(shi+'0');
	write_data(ge+'0');
}


void init()//��ʼ������
{
	uchar num;
	dula=0;//�ر�������������ˣ���ֹ����Һ��ʱ����ܻ������
	wela=0;
	lcden=0;
	fen=0; //��ʼ���ֱ���ֵ
	miao=0;
	shi=0;
	count=0;
	s1num=0;
	i2c_init();
	write_com(0x38);//��ʼ��1602Һ��
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);//������ʾ��ʼ����
	for(num=0;num<15;num++)//��ʾ����������
		{
			write_data(table[num]);
			delayms(5);
		}
		write_com(0x80+0x40+6);//д��ʱ����ʾ���ֵ�����ð��
		write_data(':');
		delayms(5);
		write_com(0x80+0x40+9);
		write_data(':');
		delayms(5);
	miao=i2c_read(1);//�״��ϵ��AT24C02�ж�ȡ���洢������
	fen=i2c_read(2);
	shi=i2c_read(3);
	display(0x80+0x40+10,miao);//�ֱ���ȥҺ����ʾ
	display(0x80+0x40+7,fen);
	display(0x80+0x40+4,shi);
	TMOD=0x01;//���ö�ʱ��0����ģʽ1
	TH0=(65536-50000)/256;//��ʱ��װ��ֵ
	TL0=(65536-50000)%256;
	EA=1;	//�����ж�
	ET0=1;	//����ʱ��0�ж�
	TR0=1;	//������ʱ��0
}
void keyscan()//����ɨ�躯��
{
	if(s1==0)	
	{
		delayms(5);
		if(s1==0)//ȷ�Ϲ��ܼ�������
		{	s1num++;//���ܼ����´�����¼
			while(!s1);//�ͷ�ȷ��
			beeper();//ÿ���а����ͷŷ�������������
			if(s1num==1)//��һ�α�����ʱ
			{
				TR0=0;  //�رն�ʱ��
				write_com(0x80+0x40+10);//��궨λ����λ��
				write_com(0x0f); //��꿪ʼ��˸
			}
			if(s1num==2)//�ڶ��ΰ��¹����˸��λ������λ��
			{
				write_com(0x80+0x40+7);
			}
			if(s1num==3)//�����ΰ��¹����˸��λ��Сʱλ��
			{
				write_com(0x80+0x40+4);
			}
			if(s1num==4)//���Ĵΰ���
			{
				s1num=0;//��¼����������
				write_com(0x0c);//ȡ�������˸
				TR0=1;	//������ʱ��ʹʱ�ӿ�ʼ��
			}		
		}
	}
	if(s1num!=0)//ֻ�й��ܼ������º����Ӻͼ�С������Ч
	{
		if(s2==0)
		{
			delayms(5);
			if(s2==0)//���Ӽ�ȷ�ϱ�����
			{
				while(!s2);//�����ͷ�
				beeper();//ÿ���а����ͷŷ�������������
				if(s1num==1)//�����ܼ���һ�ΰ���
				{
					miao++; //��������1
					if(miao==60)//����60������
						miao=0;
					display(0x80+0x40+10,miao);//ÿ����һ����Һ����ʾһ��
					i2c_write(0x80+0x40+10);//��ʾλ�����»ص����ڴ�
					i2c_write(1,miao);	//���ݸı���������24C02
				}
				if(s1num==2)//�����ܼ��ڶ��ΰ���
				{
					fen++;//��������Ӽ�1
					if(fen==60)//����60������
						fen=0;
					display(0x80+0x40+7,fen);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+7);//��ʾλ�����»ص����ڴ�
					i2c_write(2,fen);//���ݸı���������24C02
				}
				if(s1num==3)//�����ܼ������ΰ���
				{
					shi++;//�����Сʱ��1
					if(shi==24)//����24������
						shi=0;
					display(0x80+0x40+4,shi);;//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+4);//��ʾλ�����»ص����ڴ�
					i2c_write(3,shi);//���ݸı���������24C02
				}
			}
		}
		if(s3==0)
		{
			delayms(5);
			if(s3==0)//ȷ�ϼ�С��������
			{
				while(!s3);//�����ͷ�
				beeper();//ÿ���а����ͷŷ�������������
				if(s1num==1)//�����ܼ���һ�ΰ���
				{
					miao--;//��������1
					if(miao==-1)//����������������������Ϊ59
						miao=59;
					display(0x80+0x40+10,miao);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+10);//��ʾλ�����»ص����ڴ�
					i2c_write(1,miao);//���ݸı���������24C02
				}
				if(s1num==2)//�����ܼ��ڶ��ΰ���
				{
					fen--;//��������Ӽ�1
					if(fen==-1)//����������������������Ϊ59
						fen=59;
					display(0x80+0x40+7,fen);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+7);//��ʾλ�����»ص����ڴ�
					i2c_write(2,fen);//���ݸı���������24C02
				}
				if(s1num==3)//�����ܼ��ڶ��ΰ���
				{
					shi--;//�����Сʱ��1
					if(shi==-1)//����������������������Ϊ23
						shi=23;
					display(0x80+0x40+4,shi);//ÿ����һ����Һ����ʾһ��
					write_com(0x80+0x40+4);//��ʾλ�����»ص����ڴ�
					i2c_write(3,shi);//���ݸı���������24C02
				}
			}
		}
	}
}
void main()//������
{
	init();//���ȳ�ʼ��������
	while(1)//�����������ѭ��
	{
		keyscan();//��ͣ�ļ�ⰴ���Ƿ񱻰���
	}
}
void timer0() interrupt 1//��ʱ��0�жϷ������
{
	TH0=(65536-50000)/256;//�ٴ�װ��ʱ����ֵ
	TL0=(65536-50000)%256;
	count++;		//�жϴ����ۼ�
	if(count==20) //20��50����Ϊ1��
	{
		count=0;
		miao++;
		if(miao==60)//��ӵ�60���λ����
		{
			miao=0;//ͬʱ��������
			fen++;
			if(fen==60)//���Ӽӵ�60���λСʱ
			{
				fen=0;//ͬʱ����������
				shi++;
				if(shi==24)//Сʱ�ӵ�24��Сʱ����
				{
					shi=0;
				}
				display(0x80+0x40+4,shi);//Сʱ���仯������д��
				write_add(3,shi);//���ݸı���������24C02
			}
			display(0x80+0x40+7,fen);//�������仯������д��
			write_add(2,fen);//���ݸı���������24C02
		}
		display(0x80+0x40+10,miao);//�����仯������д��	
		write_add(1,miao);//���ݸı���������24C02
	}	
}
