#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned  char
#define MAIN_Fosc	11059200UL	//�궨����ʱ��HZ
#define AT24C02_ADD 0xa0            //AT24C02��ַ
/*�����,��ر�*/
sbit dula=P2^6;	  //����ܶ�ѡ
sbit wela=P2^7;	  //�����λѡ
/*Һ��*/
sbit lcdrs=P3^5;		   //RSҺ����������ѡ���
sbit lcdrw=P3^6;		   //WRҺ����дѡ���
sbit lcden=P3^4;		   //LCDENҺ��ʹ�ܶ�
/*��������*/
sbit s1=P3^0;   //��ֵģʽ���ý����л���
sbit s2=P3^1;	//��ֵѡ���л���
sbit s3=P3^2;	//��ֵ�����  \ ���淭ǽƽ���¶�
sbit s4=P3^3;	//��ֵ��С��  \ ������������
/*������*/
sbit beep=P2^3; //�����������
/*�¶ȴ�����*/
sbit DS=P2^2;	  //DS18B20������(�ź���)
/*E2PROM*/
sbit SDA=P2^0;						//ʱ������
sbit SCL=P2^1;						//��������


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
/*�¶ȴ�����*/
bit DS_init();	  //�����߳�ʼ������
void DS_set();			//�¶���ֵ���ý���
void DS_wendu();			//DS18B20��ȡ�¶�ֵ
uchar DS_read_byte();	//DS18B20������
void DS_write_byte(uchar date);	//DS18B20д����
void DS_display(uchar,uint);//�¶�Һ����ʾ����
/*E2PROM*/
void i2c_init();	 //i2c��ʼ��
void i2c_start();	 //����i2c
void i2c_stop();	 //ֹͣi2c
void i2c_sendbyte(uchar bt);	//�������һ�ֽ�����bt����
uchar i2c_recbyte(); //�������Ͻ���1�ֽ�����
void i2c_respons();  //i2cӦ���ź�  
void i2c_write(uchar address, char dat);//��ָ����ַд��һ�ֽ�����
char i2c_read(uchar address);//��ָ����ַ����һ�ֽ�����
/*Һ��*/
void yj_display(uchar add,uchar date);//Һ����ʾ����
void yj_weekdisplay(uchar a);//������ʾ����
void yj_write_com (uchar com);//Һ������д��
void yj_write_data(uchar date);//Һ������д��
/*��ʱ*/
void delayms(uint xms);		//ms����ʾ����
void delayus(uchar xus);	//us����ʾ����
void delay();			//4us��ʱ

void keyscan();		//����ɨ�躯��
void T_init();		//��ʱ����ʼ��
void init();//��ʼ������
void beeper();//����������

uchar temp;	//temp:ʵʱ�¶�,
uchar maximun,minimun;//maximun:�¶�����ֵ,minimun:�¶�����ֵ		
char tou,nian,yue,ri,week,hour,min,sec;
char setting,s2num,count;//count:��ʱ������,setting:��ֵ����ģʽ�����־,s2num:��������

/**************************************************
**************************************************/
void main()
{
	init();//���ȳ�ʼ��������
	while(1)
	{
		keyscan();
		DS_wendu();
	}
}

/*��ʼ������*/
void init()
{
	dula=0;
	wela=0;	 //�رն�ѡ��λѡ,��ֹ�����������ʾ
	lcden=0; //�ر�Һ��ʹ�ܶ�
	
	/*��ʼ�����ֱ���ֵ*/
	tou=20;
	nian=16;
	yue=1;
	ri=31;
	week=1;
	hour=12;
	min=0;
	sec=0;	  //ʱ��,ģʽ�µı���
	maximun=0;
	minimun=0;//�¶���ֵ��ʼ��
	/*��ʼ��i2c*/
	i2c_init();	
	/*��ʼ��1602Һ��*/
	yj_write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	yj_write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	yj_write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
	/*Һ����ʾ*/
	yj_display(0x80,tou);		 //(1,1)��ʾ��ݵĿ�ͷ20
	yj_display(0x80+2,nian);	 //(1,3)��ʾ��ݵĺ���λ
	yj_write_data('-');
	yj_display(0x80+5,yue);	 //(1,6)��ʾ�·�
	yj_write_data('-');
	yj_display(0x80+8,ri);		 //(1,9)��ʾ����
	yj_weekdisplay(week);		 //(1,14)��ʾ����
	yj_display(0x80+0x40,hour); //(2,5)��ʾСʱ
	yj_write_data(':');
	yj_display(0x80+0x40+3,min);	 //(2,8)��ʾ����
	yj_write_data(':');
	yj_display(0x80+0x40+6,sec);	 //(2,11)��ʾ��

	/*��ʼ����ʱ��*/
	T_init();
}

/*����������*/
void beeper()
{
	beep=0;
	delayms(100);
	beep=1;
}


/*************************************************
********************����ɨ��**********************
**************************************************/
/*����ɨ�躯��*/
void keyscan()
{
	if(0==s1)
	{
		delayms(5);
		if(0==s1)//ȷ����ֵ���ü��Ѿ�����
		{
			while(!s1);//ȷ���ͷ�
			beeper();//ÿ���а����ͷŷ�������������
			P1=0;	//��������ģʽ,������Ϊ����
			delayms(200);  //��ʱ0.2s
			P1=0xff;
			TR0=0;	//�رն�ʱ���ж�
			setting=1;	//�����¶���ֵ����ģʽ
			DS_set(); //�����¶���ֵ���ý���
			
			do
			{
				if(0==s2)
				{
					delayms(5);
					if(0==s2)//ȷ�Ϲ��ܼ�������
					{
						s2num++;//���ܼ����´�����¼
						while(!s2);//�ͷ�ȷ��
						beeper();//ÿ���а����ͷŷ�������������	   
						if(s2num==1)//��һ�α�����ʱ
						{
							yj_write_com(0x80+9);//��궨λ������¶���ֵλ��(1,10)	
							yj_write_com(0x0f); //��꿪ʼ��˸
						}
						if(s2num==2)//�ڶ��ΰ��¹����˸��λ����С��ֵλ��
							yj_write_com(0x80+0x40+9);//(2,10)
						if(s2num==3)//�����ΰ���
						{
							s2num=0;//��¼����������
							yj_write_com(0x0c);//ȡ�������˸
						}
					}
				}
				if(s2num!=0)//ֻ�й��ܼ������º����Ӻͼ�С������Ч
				{
					if(0==s3)
					{
						delayms(5);
						if(0==s3)//���Ӽ�ȷ�ϱ�����
						{
							while(!s3);//�����ͷ�
							beeper();//ÿ���а����ͷŷ�������������
							if(s2num==1)//�����ܼ���һ�ΰ���
							{
								maximun++;
								yj_display(0x80+8,maximun);//ÿ����һ����Һ����ʾһ��			
								yj_write_com(0x80+9);//��ʾλ�����»ص����ڴ�
							}
							if(s2num==2)//�����ܼ��ڶ��ΰ���
							{
								minimun++;
								yj_display(0x80+0x40+8,minimun);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+0x40+9);//��ʾλ�����»ص����ڴ�
							}
						}
					}
					if(0==s4)
					{
						delayms(5);
						if(0==s4)//ȷ�ϼ�С��������
						{
							while(!s4);//�����ͷ�
							beeper();//ÿ���а����ͷŷ�������������
							if(s2num==1)//�����ܼ���һ�ΰ���
							{
								maximun--;
								if(maximun<0)	 //��������
									maximun=0;
								yj_display(0x80+8,maximun);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+9);//��ʾλ�����»ص����ڴ�
							}
							if(s2num==2)
							{
								minimun--;
								if(minimun<0);
									minimun=0;
								yj_display(0x80+0x40+8,minimun);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+9);//��ʾλ�����»ص����ڴ�
							}
						}
					}
				}
				if(0==s1)
				{
					delayms(5);
					if(0==s1)//ȷ����ֵ���ü��Ѿ�����
					{
						while(!s1);//�����ͷ�
						beeper();//ÿ���а����ͷŷ�������������
						P1=0;	//��������ģʽ,������Ϊ����
						delayms(200);  //��ʱ0.2s
						P1=0xff;
						TR0=1;	//�رն�ʱ���ж�
						setting=0;

						yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0

						yj_display(0x80,tou);
						yj_display(0x80+2,nian);
						yj_write_data('-');
						yj_display(0x80+5,yue);
   						yj_write_data('-');
						yj_display(0x80+8,ri);
						yj_weekdisplay(week);
						yj_display(0x80+0x40,hour);
						yj_write_data(':');
						yj_display(0x80+0x40+3,min);
						yj_write_data(':');
						yj_display(0x80+0x40+6,sec);
						DS_wendu();
					}
				}
			}while(setting);	
		}
	}
	
	if(s1!=0)
	{
		if(0==s3)
		{
			delayms(5);
			if(0==s3)
			{
				while(!s3);//�����ͷ�
				beeper();//ÿ���а����ͷŷ�������������
				i2c_write(10,temp);//���浱ǰʱ�̵�ƽ���¶�
			}
		}
		if(0==s4)
		{
			delayms(5);
			if(0==s4)
			{
				while(!s4);//�����ͷ�
				beeper();//ÿ���а����ͷŷ�������������
			
				nian=i2c_read(1);
				yue=i2c_read(2);
				ri=i2c_read(3);
				week=i2c_read(4);
				hour=i2c_read(5);
				min=i2c_read(6);
				sec=i2c_read(7);
				temp=i2c_read(10);
				maximun=i2c_read(11);
				minimun=i2c_read(12);

				yj_display(0x80,tou);		 //(1,1)��ʾ��ݵĿ�ͷ20
				yj_display(0x80+2,nian);	 //(1,3)��ʾ��ݵĺ���λ
				yj_write_data('-');
				yj_display(0x80+5,yue);	 //(1,6)��ʾ�·�
				yj_write_data('-');
				yj_display(0x80+8,ri);		 //(1,9)��ʾ����
				yj_weekdisplay(week);		 //(1,14)��ʾ����
				yj_display(0x80+0x40,hour); //(2,1)��ʾСʱ
				yj_write_data(':');
				yj_display(0x80+0x40+3,min);	 //(2,4)��ʾ����
				yj_write_data(':');
				yj_display(0x80+0x40+6,sec);	 //(2,7)��ʾ��
				yj_display(0x80+0x40+12,temp);	  //(2,13)��ʾ�¶�
				yj_write_data(0xdf);
				yj_write_data('C');
			}
		}
	}	
}


/*************************************************
******************1602Һ����ʾ********************
**************************************************/
/*Һ������д��*/
void yj_write_com (uchar com)
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
void yj_write_data(uchar date)
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
void yj_display(uchar add,uchar date)//��ʾʱ���뺯����addʲôλ�� date��ʾʲô
{
	uchar shi,ge;
	shi=date/10;	
	ge=date%10;

	yj_write_com(add);
	yj_write_data(shi+'0');
	yj_write_data(ge+'0');
}

/*������ʾ*/
void yj_weekdisplay(uchar a)
{
	switch(a)
	{
		case 1:	yj_write_com(0x80+13);
				yj_write_data('M');
				yj_write_data('0');
				yj_write_data('N');
				break;
		case 2:	yj_write_com(0x80+13);
				yj_write_data('T');
				yj_write_data('U');
				yj_write_data('E');
				break;
		case 3:	yj_write_com(0x80+13);
				yj_write_data('W');
				yj_write_data('E');
				yj_write_data('D');
				break;
		case 4:	yj_write_com(0x80+13);
				yj_write_data('T');
				yj_write_data('H');
				yj_write_data('U');
				break;
		case 5:	yj_write_com(0x80+13);
				yj_write_data('F');
				yj_write_data('R');
				yj_write_data('I');
				break;
		case 6:	yj_write_com(0x80+13);
				yj_write_data('S');
				yj_write_data('A');
				yj_write_data('T');
				break;
		case 7:	yj_write_com(0x80+13);
				yj_write_data('S');
				yj_write_data('U');
				yj_write_data('N');
				break;
	}
}


/*******************************************************
*********************DS18B20����************************
*******************************************************/
/*�¶���ֵ���ý���*/
void DS_set()
{
	yj_write_com(0x01);	  //00000001B,��ʾ��0������ָ����0
	
	yj_write_com(0x80+4);
	yj_write_data('M');
	yj_write_data('A');
	yj_write_data('X');
	yj_display(0x80+8,maximun);
	yj_write_data(0xdf);
	yj_write_data('C');

	yj_write_com(0x80+0x40+4);
	yj_write_data('M');
	yj_write_data('I');
	yj_write_data('N');
	yj_display(0x80+0x40+8,minimun);
	yj_write_data(0xdf);
	yj_write_data('C');
}

/*DS18B20��ȡ�¶�ֵ*/
void DS_wendu()
{
	uchar i;
	uchar L,M;	 //L:�¶�ֵ��λ,M:�¶�ֵ��λ
	
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
	temp = i* 0.0625+0.5;//ת��Ϊʮ����,����������С��
	yj_display(0x80+0x40+12,temp);
	yj_write_data(0xdf);
	yj_write_data('C');
}

/*Һ����ʾ����
void DS_display(uchar add,uint date)//��ʾʱ���뺯����addʲôλ�� date��ʾʲô
{
	uint shi,ge;
	shi=date/10;	
	ge=date%10;

	yj_write_com(add);
	yj_write_data(shi+'0');
	yj_write_data(ge+'0');
} */

/*�����߳�ʼ������*/
bit DS_init()
{
	bit i;
	DS=1;
	_nop_();
	DS=0;
	delayus(75);	//��������661.95us,�ҽ��������ϵ�DS18B20����ȫ����λ
	DS=1;			//�ͷ�����
	delayus(4);		//��ʱ44.45us,�ȴ�DS18B20���ش����ź�
	i=DS;
	delayus(20);	//��ʱ434.35us
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


/*************************************************
******************i2c�ϵ籣��*********************
**************************************************/
/*i2c��ʼ��*/
void i2c_init()			//�����߶�����,���ͷ�����	   
{
	SDA=1;
	delay();
	SCL=1;					   
	delay();
}

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



/*************************************************
*******************��ʱ������*********************
**************************************************/
/*��ʱ����ʼ��*/
void T_init()
{
	TMOD=0x01;//���ö�ʱ��0����ģʽ1
	TH0=(65536-45872)/256;//��ʱ��װ��ֵ
	TL0=(65536-45872)%256;

	ET0=1;						  //����ʱ��0�ж�
	TR0=1;						  //������ʱ��0
	EA=1;						  //�����ж�
}

/*��ʱ��0��������*/
void T0_time0() interrupt 1//��ʱ��0�жϷ������
{
	TH0=(65536-45872)/256;//�ٴ�װ��ʱ����ֵ
	TL0=(65536-45872)%256;
	count++;		//�жϴ����ۼ�
	if(count==20) 	//20��50����Ϊ1��
	{
		count=0;
		sec++;
		if(sec==60)//��ӵ�60���λ����
		{
			sec=0;//ͬʱ��������
			min++;
			if(min==60)//���Ӽӵ�60���λСʱ
			{
				min=0;//ͬʱ����������
				hour++;
				if(hour==24)//Сʱ�ӵ�24��Сʱ����
				{
					hour=0;
					week++;
					if(week==8)
						week=1;
					ri++;
					if(ri==32)
					{
						ri=1;
						yue++;
						if(yue==13)
						{
							yue=1;
							nian++;
							yj_display(0x80+2,nian);//��仯������д����
							i2c_write(1,nian);//���ݸı���������24C02
						}
						yj_display(0x80+5,yue);//�±仯������д��
						i2c_write(2,yue);//���ݸı���������24C02
					}
					yj_display(0x80+8,ri);//�ձ仯������д����
					yj_weekdisplay(week);//�ܱ仯������д����
					i2c_write(3,ri);//���ݸı���������24C02
					i2c_write(4,week);//���ݸı���������24C02
				}
				yj_display(0x80+0x40,hour);//Сʱ���仯������д��
				i2c_write(5,hour);//���ݸı���������24C02
			}
			yj_display(0x80+0x40+3,min);//�������仯������д��
			i2c_write(6,min);//���ݸı���������24C02
		}
		yj_display(0x80+0x40+6,sec);//�����仯������д��	
		i2c_write(7,sec);//���ݸı���������24C02
	}	
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

/*4us��ʱ*/
void delay()
{
;;
}