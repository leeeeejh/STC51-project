/********************************************************************************
�����ö�ʱ����д����ʱ�ӳ����趨�������ܼ���һ�����ܼ�s1��һ����ֵ�����s2��һ����
ֵ��С��s3������ʱ�䡣������s4��Ϊ����ģʽ��ʱ��ģʽ�л���,ͬʱ����ʹ���������ܼ�����
��ʱ����е���.ͬʱ���д���ͨ�Ź���,�ܹ�ͨ������ͨ�Ŷ�ʱ������޸�(�Ҷ�����޸�ģʽΪ
����n��ȴ������˸,�ڶ�nian�����޸�;ͬ������y,r,w,h,m,s�ȴ�1602Һ�����϶�Ӧλ�õĹ��
��˸���ٶ�������޸�).
���ұ������ܽ����ݴ��뵽AT2404�У��ٴ�������Դ�����ӱ�Ӷϵ�ʱ�̿�ʼ������ʱ
**********************************************************************************/

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
sbit s4=P3^3;	//ģʽ�л���

sbit beep=P2^3; //�����������

void delayms(uint xms);	//ms����ʱ
void delay();			//4us��ʱ

void i2c_init();	 //i2c��ʼ��
void i2c_start();	 //����i2c
void i2c_stop();	 //ֹͣi2c
void i2c_sendbyte(uchar bt);	//�������һ�ֽ�����bt����
uchar i2c_recbyte(); //�������Ͻ���1�ֽ�����
void i2c_respons();  //i2cӦ���ź�  
void i2c_write(uchar address, char dat);//��ָ����ַд��һ�ֽ�����
char i2c_read(uchar address);//��ָ����ַ����һ�ֽ�����

void yj_display(uchar add,uchar date);//Һ����ʾ����
void yj_weekdisplay(uchar a);//������ʾ����
void yj_write_com (uchar com);//Һ������д��
void yj_write_data(uchar date);//Һ������д��

void nz_clock();	 //����ģʽ��ʾ����
void nz_ONorOFF(uchar b);//���ӿ�������
void nz_time();		//�ж��Ƿ񵽴��������õ�ʱ��

void ck_input();	//��ȡ��������
	
void keyscan();		//����ɨ�躯��
void T_init();		//��ʱ����ʼ��
void init();//��ʼ������
void beeper();//����������


uchar table[14]="ALARM MODE SET";

uchar count,s1num; //count:��¼�жϴ���
				   //s1num:��¼s1�������µĴ���
uchar alarm,setting;//alarm:��¼�Ƿ����ù�����
					//setting:��־�Ƿ��������
uchar chuankou,flag;//chuankou:���洮��ͨ�Ž��յ�����
					//flag:����Ƿ���յ���������
char tou,nian,yue,ri,week,hour,min,sec;//����������ʱ����λ
char ialarm,ihour=12,imin=0,isec=0;//��������ģʽʱ����,ialarmΪ���ӿ���


/*************************************************
**************************************************
*************************************************/
/*������*/
void main()
{
	init();//���ȳ�ʼ��������
	flag=0;	//����ͨ�Ž��ձ�־λ����,
	while(1)//�����������ѭ��
	{
		if(flag==1)
		{
			ck_input();
		}

		keyscan();//��ͣ�ļ�ⰴ���Ƿ񱻰���
		nz_time(); //�ж��Ƿ������������
	}	
}

/*��ʼ������*/
void init()
{
	dula=0;
	wela=0;			//�ر�������������ˣ���ֹ����Һ��ʱ����ܻ������
	lcden=0;		//�ر�Һ��ʹ�ܶ�
	
	/*��ʼ�����ֱ���ֵ*/
	tou=20;
	nian=16;
	yue=1;
	ri=31;
	week=1;
	hour=12;
	min=0;
	sec=0;	  //ʱ��,ģʽ�µı���
	ialarm=0;
	ihour=12;
	imin=0;	  //����ģʽ�µı���
	count=0;  //�жϴ�������
	s1num=0;  //s1������������
	/*��ʼ��i2c*/
	i2c_init();	
	/*��ʼ��1602Һ��*/
	yj_write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	yj_write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	yj_write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0

	nian=i2c_read(1);
	yue=i2c_read(2);
	ri=i2c_read(3);
	week=i2c_read(4);
	hour=i2c_read(5);
	min=i2c_read(6);
	sec=i2c_read(7);
	ialarm=i2c_read(11);
	ihour=i2c_read(12);
	imin=i2c_read(13);

	yj_display(0x80,tou);		 //(1,1)��ʾ��ݵĿ�ͷ20
	yj_display(0x80+2,nian);	 //(1,3)��ʾ��ݵĺ���λ
	yj_write_data('-');
	yj_display(0x80+5,yue);	 //(1,6)��ʾ�·�
	yj_write_data('-');
	yj_display(0x80+8,ri);		 //(1,9)��ʾ����
	yj_weekdisplay(week);		 //(1,14)��ʾ����
	yj_display(0x80+0x44,hour); //(2,5)��ʾСʱ
	yj_write_data(':');
	yj_display(0x80+0x47,min);	 //(2,8)��ʾ����
	yj_write_data(':');
	yj_display(0x80+0x4a,sec);	 //(2,11)��ʾ��

	nian=i2c_read(1);
	yue=i2c_read(2);
	ri=i2c_read(3);
	week=i2c_read(4);
	hour=i2c_read(5);
	min=i2c_read(6);
	sec=i2c_read(7);
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
	if(s4!=0)
	{
		if(s1==0)	
		{
			delayms(5);
			if(s1==0)//ȷ�Ϲ��ܼ�������
			{	
				s1num++;//���ܼ����´�����¼
				while(!s1);//�ͷ�ȷ��
				beeper();//ÿ���а����ͷŷ�������������
				if(s1num==1)//��һ�α�����ʱ
				{
					TR0=0;  //�رն�ʱ��
					yj_write_com(0x80+0x40+11);//��λ(2,12)
					yj_write_com(0x0f); //��꿪ʼ��˸
				}
				if(s1num==2)//�ڶ��ΰ��¹����˸��λ����λ
					yj_write_com(0x80+0x40+8);//(2,8)
				if(s1num==3)//�����ΰ��¹����˸��λ��Сʱλ
					yj_write_com(0x80+0x40+5);//(2,6)
				if(s1num==4)//���Ĵΰ��¹����˸��λ������λ
					yj_write_com(0x80+15);//(1,16)
				if(s1num==5)//����ΰ��¹����˸��λ����λ
					yj_write_com(0x80+9);//(1,10)
				if(s1num==6)//�����ΰ��¹����˸��λ����λ
					yj_write_com(0x80+6);//(1,7)
				if(s1num==7)//���ߴΰ��¹����˸��λ����λ
					yj_write_com(0x80+3);//(1,4)
				if(s1num==8)//�ڰ˴ΰ���
				{
					s1num=0;//��¼����������
					yj_write_com(0x0c);//ȡ�������˸
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
						sec++; //��������1
						if(sec==60)//����60������
							sec=0;
						yj_display(0x80+0x40+10,sec);//ÿ����һ����Һ����ʾһ��
						yj_write_com(0x80+0x40+11);//��ʾλ�����»ص����ڴ�
						i2c_write(7,sec);	//���ݸı���������24C02
					}
					if(s1num==2)//�����ܼ��ڶ��ΰ���
					{
						min++;//��������Ӽ�1
						if(min==60)//����60������
							min=0;
						yj_display(0x80+0x40+7,min);//ÿ����һ����Һ����ʾһ��
						yj_write_com(0x80+0x40+8);//��ʾλ�����»ص����ڴ�
						i2c_write(6,min);//���ݸı���������24C02
					}
					if(s1num==3)//�����ܼ������ΰ���
					{
						hour++;//�����Сʱ��1
						if(hour==24)//����24������
							hour=0;
						yj_display(0x80+0x40+4,hour);//ÿ����һ����Һ����ʾһ��
						yj_write_com(0x80+0x40+5);//��ʾλ�����»ص����ڴ�
						i2c_write(5,hour);//���ݸı���������24C02
					}
					if(s1num==4)//�����ܼ����Ĵΰ���
					{
						week++;//�����Сʱ��1
						if(week==8)//����24������
							week=1;
						yj_weekdisplay(week);
						yj_write_com(0x80+15);//��ʾλ�����»ص����ڴ�
						i2c_write(4,week);//���ݸı���������24C02
					}
					if(s1num==5)//�����ܼ�����ΰ���
					{
						ri++;//�����Сʱ��1
						if(ri==32)//����24������
							ri=1;
						yj_display(0x80+8,ri);//ÿ����һ����Һ����ʾһ��
						yj_write_com(0x80+9);//��ʾλ�����»ص����ڴ�
						i2c_write(3,ri);//���ݸı���������24C02
					}
					if(s1num==6)//�����ܼ������ΰ���
					{
						yue++;//�����Сʱ��1
						if(yue==32)//����24������
							yue=1;
						yj_display(0x80+5,yue);//ÿ����һ����Һ����ʾһ��
						yj_write_com(0x80+6);//��ʾλ�����»ص����ڴ�
						i2c_write(2,yue);//���ݸı���������24C02
					}	
					if(s1num==7)//�����ܼ����ߴΰ���
					{
						nian++;//�����Сʱ��1
						if(nian==100)//����24������
							nian=0;
						yj_display(0x80+2,nian);//ÿ����һ����Һ����ʾһ��
						yj_write_com(0x80+3);//��ʾλ�����»ص����ڴ�
						i2c_write(1,nian);//���ݸı���������24C02
					}	
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
					sec--;//��������1
					if(sec==-1)//����������������������Ϊ59
						sec=59;
					yj_display(0x80+0x40+10,sec);//ÿ����һ����Һ����ʾһ��
					yj_write_com(0x80+0x40+11);//��ʾλ�����»ص����ڴ�
					i2c_write(7,sec);//���ݸı���������24C02
				}
				if(s1num==2)//�����ܼ��ڶ��ΰ���
				{
					min--;//��������Ӽ�1
					if(min==-1)//����������������������Ϊ59
						min=59;
					yj_display(0x80+0x40+7,min);//ÿ����һ����Һ����ʾһ��
					yj_write_com(0x80+0x40+8);//��ʾλ�����»ص����ڴ�
					i2c_write(6,min);//���ݸı���������24C02
				}
				if(s1num==3)//�����ܼ��ڶ��ΰ���
				{
					hour--;//�����Сʱ��1
					if(hour==-1)//����������������������Ϊ23
						hour=23;
					yj_display(0x80+0x40+4,hour);//ÿ����һ����Һ����ʾһ��
					yj_write_com(0x80+0x40+5);//��ʾλ�����»ص����ڴ�
					i2c_write(5,hour);//���ݸı���������24C02
				}
				if(s1num==4)//�����ܼ��ڶ��ΰ���
				{
					week--;//�����Сʱ��1
					if(week==0)//����������������������Ϊ23
						week=7;
					yj_weekdisplay(week);
					yj_write_com(0x80+15);//��ʾλ�����»ص����ڴ�
					i2c_write(4,week);//���ݸı���������24C02
				}
				if(s1num==5)//�����ܼ��ڶ��ΰ���
				{
					ri--;//�����Сʱ��1
					if(ri==0)//����������������������Ϊ23
						ri=31;
					yj_display(0x80+8,ri);//ÿ����һ����Һ����ʾһ��
					yj_write_com(0x80+9);//��ʾλ�����»ص����ڴ�
					i2c_write(3,ri);//���ݸı���������24C02
				}
				if(s1num==6)//�����ܼ��ڶ��ΰ���
				{
					yue--;//�����Сʱ��1
					if(yue==0)//����������������������Ϊ23
						yue=12;
					yj_display(0x80+5,yue);//ÿ����һ����Һ����ʾһ��
					yj_write_com(0x80+6);//��ʾλ�����»ص����ڴ�
					i2c_write(2,yue);//���ݸı���������24C02
				}
				if(s1num==7)//�����ܼ��ڶ��ΰ���
				{
					nian--;//�����Сʱ��1
					if(nian==-1)//����������������������Ϊ23
						nian=99;
					yj_display(0x80+2,nian);//ÿ����һ����Һ����ʾһ��
					yj_write_com(0x80+3);//��ʾλ�����»ص����ڴ�
					i2c_write(1,nian);//���ݸı���������24C02
				}
			}
		}
	}

	if(0==s4)
	{
		delayms(5);
		if(0==s4)//ȷ�Ϲ��ܼ�������
		{
			while(!s4);//�ͷ�ȷ��
			beeper();//ÿ���а����ͷŷ�������������	
			P1=0;	 //��������ģʽ,���Ʒ���
			delayms(200); //��ʱ0.2s
			P1=0xff;
			TR0=0;		//�رն�ʱ���ж�
			alarm=1;	 //������
			setting=1;	 //������������״̬
			nz_clock();	//�������ӽ���
			
			do
			{
				if (s1==0)
				{
					delayms(5);		   
					if (s1==0)//ȷ�Ϲ��ܼ�������
					{
						s1num++;//���ܼ����´�����¼					
						while(!s1);//�ͷ�ȷ��
						beeper();//ÿ���а����ͷŷ�������������	   
						if(s1num==1)//��һ�α�����ʱ
						{
							TR0=0;  //�رն�ʱ��
							yj_write_com(0x80+0x40+2);//��궨λ�����ӿ���ѡ��λλ��(2,3)
							yj_write_com(0x0f); //��꿪ʼ��˸
						}
						if(s1num==2)//�ڶ��ΰ��¹����˸��λ������λ��
							yj_write_com(0x80+0x40+7);//(2,8)
						if(s1num==3)//�����ΰ��¹����˸��λ��Сʱλ��
							yj_write_com(0x80+0x40+10);//(2,11)
						if(s1num==4)//���Ĵΰ���
						{
							s1num=0;//��¼����������
							yj_write_com(0x0c);//ȡ�������˸
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
								ialarm++; //��������1
								if(ialarm==2)//����60������
									ialarm=0;
								nz_ONorOFF(ialarm);
								yj_write_com(0x80+0x40+2);//��ʾλ�����»ص����ڴ�
								i2c_write(11,ialarm);	//���ݸı���������24C02
							}
							if(s1num==2)//�����ܼ��ڶ��ΰ���
							{
								ihour++;//��������Ӽ�1
								if(ihour==24)//����60������
									ihour=0;
								yj_display(0x80+0x40+6,ihour);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+0x40+7);//��ʾλ�����»ص����ڴ�
								i2c_write(12,ihour);//���ݸı���������24C02
							}
							if(s1num==3)//�����ܼ������ΰ���
							{
								imin++;//�����Сʱ��1
								if(imin==60)//����24������
									imin=0;
								yj_display(0x80+0x40+9,imin);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+0x40+10);//��ʾλ�����»ص����ڴ�
								i2c_write(13,imin);//���ݸı���������24C02
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
								ialarm--;//��������1
								if(ialarm==-1)//����������������������Ϊ59
									ialarm=1;
								 nz_ONorOFF(ialarm);
							   	 yj_write_com(0x80+0x40+2);//��ʾλ�����»ص����ڴ�
							  	 i2c_write(11,ialarm);//���ݸı���������24C02
							}
							if(s1num==2)//�����ܼ��ڶ��ΰ���
							{
								ihour--;//��������Ӽ�1
								if(ihour==-1)//����������������������Ϊ59
									ihour=23;
								yj_display(0x80+0x40+6,ihour);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+0x40+7);//��ʾλ�����»ص����ڴ�
								i2c_write(12,ihour);//���ݸı���������24C02
							}
							if(s1num==3)//�����ܼ��ڶ��ΰ���
							{
								imin--;//�����Сʱ��1
								if(imin==-1)//����������������������Ϊ23
									imin=59;
								yj_display(0x80+0x40+9,imin);//ÿ����һ����Һ����ʾһ��
								yj_write_com(0x80+0x40+10);//��ʾλ�����»ص����ڴ�
								i2c_write(13,imin);//���ݸı���������24C02
							}
						}
					}
				}
				if(s4==0)
				{
					delayms(5);		   //��ʱ����
					if(s4==0)			   //�ظ��ж�,����
					{
						while(!s4);
						beeper();		   //��������ʱ��������,��Ϊ����
						P1=0;			   //��������ģʽ,���Ʒ���
						delayms(200);	   //��ʱ
						P1=0xff;
						TR0=1;
						setting=0;		   //�������

						yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0

						yj_display(0x80,tou);
						yj_display(0x80+2,nian);
						yj_write_data('-');
						yj_display(0x80+5,yue);
   						yj_write_data('-');
						yj_display(0x80+8,ri);
						yj_weekdisplay(week);
						yj_display(0x80+0x44,hour);
						yj_write_data(':');
						yj_display(0x80+0x47,min);
						yj_write_data(':');
						yj_display(0x80+0x4a,sec);
					}
				}
			}while(setting); //�ȴ��������
		}
	}
}


/*************************************************
********************��������**********************
**************************************************/
/*����ģʽ��ʾ*/
void nz_clock()
{
	uchar i;

	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
	yj_write_com (0x80+0x01);
	for (i=0;i<14;i++)
	{
		yj_write_data(table[i]);	
	}
	yj_write_com(0x80+0x40+1);
	nz_ONorOFF(ialarm);
	yj_display(0x80+0x40+6,ihour);
	yj_write_data(':');
	yj_display(0x80+0x40+9,imin);		
}

/*����ģʽѡ����*/
void nz_ONorOFF(uchar b)
{
	if(0==b)
	{
		yj_write_com(0x80+0x40+1);
		yj_write_data('O');
		yj_write_data('N');
		yj_write_data(' ');
	}
	if(1==b)
	{
		yj_write_com(0x80+0x40+1);
		yj_write_data('O');
		yj_write_data('F');
		yj_write_data('F');
	}
}

/*����ʱ���ж�*/
void nz_time()
{
	uint i;
	if(0==ialarm&&1==alarm)	//���ù�����������������
	{
		if((hour==ihour)&&(min==imin))
		{
			beep=0;				 
			for(i=0;i<60;i++)
				delayms(1000);
			beep=1;				//��������1s
			alarm=0;
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
	TMOD=0x21;//���ö�ʱ��1������ʽ2,��ʱ��0����ģʽ1
	TH0=(65536-50000)/256;//��ʱ��װ��ֵ
	TL0=(65536-50000)%256;
	TH1=253;					  //T1��ʱ������ֵ,������9600
	TL1=253;	

	ET0=1;						  //����ʱ��0�ж�
	TR0=1;						  //������ʱ��0
	TR1=1;						  //������ʱ��1

	REN=1;						  //�����ڽ�������
	SM0=0;						  //�趨���ڹ�����ʽ1
	SM1=1;

	EA=1;						  //�����ж�
	ES=1;						  //�������ж�	
}

/*��ʱ��0��������*/
void T0_time0() interrupt 1//��ʱ��0�жϷ������
{
	TH0=(65536-50000)/256;//�ٴ�װ��ʱ����ֵ
	TL0=(65536-50000)%256;
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
				yj_display(0x80+0x40+4,hour);//Сʱ���仯������д��
				i2c_write(5,hour);//���ݸı���������24C02
			}
			yj_display(0x80+0x40+7,min);//�������仯������д��
			i2c_write(6,min);//���ݸı���������24C02
		}
		yj_display(0x80+0x40+10,sec);//�����仯������д��	
		i2c_write(7,sec);//���ݸı���������24C02
	}	
}


/*************************************************
******************����ͨ������********************
**************************************************/
/*�����жϷ���*/
void ck_ser() interrupt 4
{
	RI=0;					//�����������ж�
	chuankou=SBUF;		//�����ݴ��뵽��������
	flag=1;					//��־�Ƿ����յ�����
} 

/*��ȡ��������*/
void ck_input()
{
	if(chuankou=='n')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+3);	 //����Ƶ���λ��(��һ��4λ)
		yj_write_com(0x0f);	 //�����˸
		
		while(!RI);
			RI=0;
		nian=SBUF;	
		i2c_write(1,nian);//���ݸı���������24C02
		yj_display(0x80+2,nian);//��仯ʱ����д��	
		yj_write_com(0x0c);		//����ʾ,��ȡ�������˸
		flag=0;					
		TR0=1;					//��ʱ����ʱ
	}
	if(chuankou=='y')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+6);	 //����Ƶ���λ��(��һ��7λ)
		yj_write_com(0x0f);		 //�����˸
	
		while(!RI);
		RI=0;
		yue=SBUF;
		i2c_write(2,yue);//���ݸı���������24C02
		yj_display(0x80+5,yue);//�±仯ʱ����д��	
		yj_write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;					//��ʱ����ʱ
	}
	if(chuankou=='r')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+9);	 //����Ƶ���λ��(��һ��10λ)
		yj_write_com(0x0f);		 //�����˸
				
		while(!RI);
			RI=0;
		ri=SBUF;
		i2c_write(3,ri);//���ݸı���������24C02
		yj_display(0x80+8,ri);//�ձ仯ʱ����д��	
		yj_write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;					//��ʱ����ʱ
	}
	if(chuankou=='w')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+15);	 //����Ƶ�����λ��(��һ��16λ)
		yj_write_com(0x0f);		 //�����˸
			
		while(!RI);
			RI=0;
		week=SBUF;
		i2c_write(4,week);//���ݸı���������24C02
		yj_weekdisplay(week);	
		yj_write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;					//��ʱ����ʱ
	}
	if(chuankou=='h')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+0x45);	 //����Ƶ�ʱλ��(��һ��16λ)
		yj_write_com(0x0f);		 //�����˸
			
		while(!RI);
			RI=0;
		hour=SBUF;
		i2c_write(5,hour);//���ݸı���������24C02
		yj_display(0x80+0x44,hour);//���ڱ仯ʱ����д��	
		yj_write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;					//��ʱ����ʱ
	}
	if(chuankou=='m')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+0x48);	 //����Ƶ�����λ��(��һ��16λ)
		yj_write_com(0x0f);		 //�����˸
			
		while(!RI);
			RI=0;
		min=SBUF;
		i2c_write(6,min);//���ݸı���������24C02
		yj_display(0x80+0x47,min);//���ڱ仯ʱ����д��	
		yj_write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;					//��ʱ����ʱ
	}		   
	if(chuankou=='s')
	{
		TR0=0;				  //�ر�ʱ����ʱ
		yj_write_com(0x80+0x4b);	 //����Ƶ�����λ��(��һ��16λ)
		yj_write_com(0x0f);		 //�����˸		
						
		while(!RI);
			RI=0;
		sec=SBUF;
		i2c_write(7,sec);//���ݸı���������24C02
		yj_display(0x80+0x4a,sec);//���ڱ仯ʱ����д��	
		yj_write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;					//��ʱ����ʱ
	}	 	  	
}

/*************************************************
******************��ʱ��������********************
**************************************************/
/*ms����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}

/*4us��ʱ*/
void delay()
{
;;
}



