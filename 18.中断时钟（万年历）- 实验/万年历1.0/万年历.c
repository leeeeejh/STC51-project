/********************************************************************************
�����ö�ʱ����д����ʱ�ӳ����趨�������ܼ���һ�����ܼ�s1��һ����ֵ�����s2��һ����
ֵ��С��s3������ʱ�䡣������s4��Ϊ����ģʽ��ʱ��ģʽ�л���,ͬʱ����ʹ���������ܼ�����
��ʱ����е���.ͬʱ���д���ͨ�Ź���,�ܹ�ͨ������ͨ�Ŷ�ʱ������޸�(�Ҷ�����޸�ģʽΪ
����n��ȴ������˸,�ڶ�nian�����޸�;ͬ������y,r,w,h,m,s�ȴ�1602Һ�����϶�Ӧλ�õĹ��
��˸���ٶ�������޸�).
���ұ������ܽ����ݴ��뵽AT2404�У��ٴ�������Դ�����ӱ�Ӷϵ�ʱ�̿�ʼ������ʱ
**********************************************************************************/
#include <reg52.h>
#include<intrins.h> 		//����NOP��ָ���_nop_();

#define uchar unsigned char 
#define uint unsigned int 
#define AT24C02_ADD 0xa0            //AT24C02��ַ

sbit lcdrs=P3^5;		   //RSҺ����������ѡ���
sbit lcdrw=P3^6;		   //WRҺ����дѡ���
sbit lcden=P3^4;		   //LCDENҺ��ʹ�ܶ�
sbit dula=P2^6;			   //��ѡ
sbit wela=P2^7;			   //λѡ

sbit beep=P2^3;			   //������
sbit led1=P1^0;			   

sbit s1=P3^0;			   //��Ϊ����ѡ���
sbit s2=P3^1;  			   //��Ϊ��ֵ�����
sbit s3=P3^2;			   //��Ϊ��ֵ��С��
sbit s4=P3^3;			   //ģʽ�л���

sbit SDA=P2^0;						//ʱ������
sbit SCL=P2^1;						//��������

bit write=0;							//д24CO2�ı�־

uchar shuzi[10]="0123456789";
uchar table[14]="ALARM MODE SET";
uchar chuankou;

char tou=20,nian=16,yue=1,ri=31,week=1,hour=12,min=0,sec=0;//����������ʱ����λ
char ialarm,ihour=12,imin=0,isec=0;//��������ģʽʱ����,ialarmΪ���ӿ���

uchar time=0,addr,alarm=0,setting=0,flag;  //addr:���ѡ��λ,alarm:���ӿ��ر�־

void init();				   //��ʱ����1602��ʾ����ʼ���趨����
void write_com(uchar);		   //Һ������д��
void write_data(uchar);		   //Һ������д��
void display(uchar,uchar);	   //Һ����ʾ����
void keyscan();				   //�������������޸ĺ���
void beeper();				   //����������
void delayms(uchar);		   //һ����ʱ����
void delay();					//4us��ʱ
void screen();				   //���ӽ�����ʾ����
void weekdisplay(uchar);		//������ʾ
void chosealarm(uchar);			//���ӿ�����ʾ
void chuankoutongxin();		//����ͨ��
void i2c_init();				//I2C��ʼ��
void i2c_stop();			//i2cֹͣ����
void i2c_respons();			//Ӧ��
bit i2c_waitACK();							  //i2cӦ������ж��ź�
void i2c_sendbyte(uchar bt);					  //����һ�ֽ�����
uchar i2c_recbyte();							  //����һ�ֽ�����
char i2c_readbyte(uchar);	  //��ȡһ�ֽ�����
void i2c_writebyte(uchar,uchar);	  //��ȡһ�ֽ�����
void naozhongpanduan();			//�����ж�


void main ()
{
	init ();				  //��ʼ��
	i2c_init();				  //I2C��ʼ��
	
	nian=16,yue=1,ri=31,week=1,hour=12,min=0,sec=0;//����������ʱ����λ
	
	i2c_writebyte(1,nian);	//��24C02�ĵ�ַ1��д������nian
	i2c_writebyte(2,yue);	//��24C02�ĵ�ַ2��д������yue
	i2c_writebyte(3,ri);	//��24C02�ĵ�ַ3��д������ri
	i2c_writebyte(4,week);	//��24C02�ĵ�ַ4��д������week
	i2c_writebyte(5,hour);	//��24C02�ĵ�ַ5��д������hour
	i2c_writebyte(6,min);	//��24C02�ĵ�ַ6��д������min
	i2c_writebyte(7,sec);	//��24C02�ĵ�ַ7��д������sec
	i2c_writebyte(11,ialarm);	//��24C02�ĵ�ַ11��д������ialarm
	i2c_writebyte(12,ihour);	//��24C02�ĵ�ַ12��д������ihour
	i2c_writebyte(13,imin);	//��24C02�ĵ�ַ13��д������imin
  	
	nian=i2c_readbyte(1);	//��24C02�ĵ�ַ1�ж�������nian
	yue=i2c_readbyte(2);	//��24C02�ĵ�ַ2�ж�������yue
	ri=i2c_readbyte(3);	//��24C02�ĵ�ַ3�ж�������ri
	week=i2c_readbyte(4);	//��24C02�ĵ�ַ4�ж�������week
	hour=i2c_readbyte(5);	//��24C02�ĵ�ַ5�ж�������hour
	min=i2c_readbyte(6);	//��24C02�ĵ�ַ6�ж�������min
	sec=i2c_readbyte(7);	//��24C02�ĵ�ַ7�ж�������sec
	ialarm=i2c_readbyte(11);	//��24C02�ĵ�ַ11�ж�������ialarm
	ihour=i2c_readbyte(12);	//��24C02�ĵ�ַ12�ж�������ihour
	imin=i2c_readbyte(13);	//��24C02�ĵ�ַ13�ж�������imin

	display(0x80,tou);
	display(0x80+2,nian);
	write_data('-');
	display(0x80+5,yue);
	write_data('-');
	display(0x80+8,ri);
	weekdisplay(week);
	display(0x80+0x44,hour);
	write_data(':');
	display(0x80+0x47,min);
	write_data(':');
	display(0x80+0x4a,sec);
	
	flag=0;				  //����ͨ�Ž��ձ�־λ����,
	while (1)			  //д����������Ϊmain����
	{					  //ǰ����ֵ��Ч
		if(flag==1)
		{
			TR0=0;
			chuankoutongxin();
		}

		keyscan ();
		
		naozhongpanduan();
		
		if(1==write)
		{
			write=0;
			
			i2c_writebyte(1,nian);	//��24C02�ĵ�ַ1��д������nian
			i2c_writebyte(2,yue);	//��24C02�ĵ�ַ2��д������yue
			i2c_writebyte(3,ri);	//��24C02�ĵ�ַ3��д������ri
			i2c_writebyte(4,week);	//��24C02�ĵ�ַ4��д������week
			i2c_writebyte(5,hour);	//��24C02�ĵ�ַ5��д������hour
			i2c_writebyte(6,min);	//��24C02�ĵ�ַ6��д������min
			i2c_writebyte(7,sec);	//��24C02�ĵ�ַ7��д������sec
			i2c_writebyte(11,ialarm);	//��24C02�ĵ�ַ11��д������ialarm
			i2c_writebyte(12,ihour);	//��24C02�ĵ�ַ12��д������ihour
			i2c_writebyte(13,imin);	//��24C02�ĵ�ַ13��д������imin	
		}		
	}
}

/*�����ж�*/
void naozhongpanduan()
{
	uint i;
	if(ialarm==0&&alarm==1)
	{
		if((hour==ihour)&&(min==imin))
		{
			beep=0;			//��������
			for(i=0;i<600;i++)	//1min
				delayms(100);	//0.1s
			beep=1;				//�ط�����
			alarm=0;			//һ���Ӻ��Զ��ر�����
		}
	}
}

/*����ͨ��*/
void chuankoutongxin()
{
	if(chuankou=='n')
	{
		write_com(0x80+3);	 //����Ƶ���λ��(��һ��4λ)
		write_com(0x0f);	 //�����˸
		
		while(!RI);
		RI=0;
		nian=SBUF;	
		display(0x80+2,nian);//��仯ʱ����д��	
		
		write_com(0x0c);		//����ʾ,��ȡ�������˸
		flag=0;					
		TR0=1;					//�����־λ
	}
	if(chuankou=='y')
	{
		write_com(0x80+6);	 //����Ƶ���λ��(��һ��7λ)
		write_com(0x0f);		 //�����˸
	
		while(!RI);
		RI=0;
		yue=SBUF;
		display(0x80+5,yue);//�±仯ʱ����д��	
	
		write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;
	}
	if(chuankou=='r')
	{
		write_com(0x80+9);	 //����Ƶ���λ��(��һ��10λ)
		write_com(0x0f);		 //�����˸
				
		while(!RI);
		RI=0;
		ri=SBUF;
		display(0x80+8,ri);//�ձ仯ʱ����д��	

		write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;
	}
	if(chuankou=='w')
	{
		write_com(0x80+15);	 //����Ƶ�����λ��(��һ��16λ)
		write_com(0x0f);		 //�����˸
			
		while(!RI);
		RI=0;
		week=SBUF;
		weekdisplay(week);	

		write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;
	}
	if(chuankou=='h')
	{
		write_com(0x80+0x45);	 //����Ƶ�ʱλ��(��һ��16λ)
		write_com(0x0f);		 //�����˸
			
		while(!RI);
		RI=0;
		hour=SBUF;
		display(0x80+0x44,hour);//���ڱ仯ʱ����д��	
	
		write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;
	}
	if(chuankou=='m')
	{
		write_com(0x80+0x48);	 //����Ƶ�����λ��(��һ��16λ)
		write_com(0x0f);		 //�����˸
			
		while(!RI);
		RI=0;
		min=SBUF;
		display(0x80+0x47,min);//���ڱ仯ʱ����д��	
	
		write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;
	}		   
	if(chuankou=='s')
	{
		write_com(0x80+0x4b);	 //����Ƶ�����λ��(��һ��16λ)
		write_com(0x0f);		 //�����˸		
						
		while(!RI);
		RI=0;
		sec=SBUF;
		display(0x80+0x4a,sec);//���ڱ仯ʱ����д��	
	
		write_com(0x0c);		  //����ʾ,��ȡ�������˸
		flag=0;
		TR0=1;
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
		write=1;				  //һ����д��һ��AT24C02
		if (sec==60)			  //1min
		{
			sec=0;
			min++;
			if (min==60)		  //1h
			{
				min=0;
				hour++;
				if (hour==24)
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
							display(0x80+2,nian);//��仯������д����
						}
						display(0x80+5,yue);//�±仯������д��
					}
					display(0x80+0x8,ri);//�ձ仯������д����
					weekdisplay(week);//�ܱ仯������д����
				}
				display(0x80+0x44,hour);//ʱ�仯������д��ʱ
			}
			display(0x80+0x47,min);//�ֱ仯������д���
		}
		display(0x80+0x4a,sec);//��仯������д����
	}
}

/*�����жϷ���*/
void ser() interrupt 4
{
	RI=0;					//�����������ж�
	chuankou=SBUF;		//�����ݴ��뵽��������
	flag=1;					//��־�Ƿ����յ�����
} 

/*��ʱ����1602��ʾ����ʼ���趨����*/
void init ()
{
	dula=0;		 	
	wela=0;			//�رն�ѡ��λѡ,��ֹ����ܵ���
	lcden=0;		//�ر�Һ��ʹ�ܶ�

	TMOD=0X21;					  //���ö�ʱ��1�Ĺ�����ʽΪ2,��ʱ��0������ʽ1
	TH0=(65536-45872)/256;		  //50ms
	TL0=(65536-45872)%256;
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
		for (j=114;j>0;j--);   //1ms

}

/*4us��ʱ*/
void delay()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

/*�������������޸ĺ���*/
void keyscan ()
{
	if(s4!=0)
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
					write_com(0x80+15);	 	//����Ƶ�����λ��(�ڶ���16λ)
					write_com(0x0f);		 //�����˸
				}
				if (addr==5)
				{
					write_com(0x80+9);	 //����Ƶ���λ��(�ڶ���10λ)
					write_com(0x0f);		 //�����˸
				}
				if (addr==6)
				{
					write_com(0x80+6);	 //����Ƶ���λ��(�ڶ���7λ)
					write_com(0x0f);		 //�����˸
				}
				if (addr==7)
				{
					write_com(0x80+3);	 //����Ƶ���λ��(�ڶ���4λ)
					write_com(0x0f);		 //�����˸
				}
				if (addr==8)
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
					i2c_writebyte(7,sec);	//��24C02�ĵ�ַ7��д������sec	
				}
				if (addr==2) 	 //���ѡ��λ�ڷ���λ
				{
					min++;
					if (min==60)
						min=0;	
					display(0x80+0x47,min);//�ֱ仯ʱ����д��
					write_com(0x80+0x48);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(6,min);	//��24C02�ĵ�ַ6��д������min	
				}
				if (addr==3) 	 //���ѡ��λ��Сʱλ
				{
					hour++;
					if (hour==24)
						hour=0;
					display(0x80+0x44,hour);//ʱ�仯ʱ����д��	
					write_com(0x80+0x45);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(5,hour);	//��24C02�ĵ�ַ5��д������hour	
				}
				if (addr==4) 	 //���ѡ��λ������λ
				{
					week++;
					if (week==8)
						week=1;
					weekdisplay(week);	
					write_com(0x80+15);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(4,week);	//��24C02�ĵ�ַ4��д������week	
				}
				if (addr==5) 	 //���ѡ��λ����λ
				{
					ri++;
					if (ri==32)
						ri=1;
					display(0x80+8,ri);//ʱ�仯ʱ����д��	
					write_com(0x80+9);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(3,ri);	//��24C02�ĵ�ַ3��д������ri	
				}
				if (addr==6) 	 //���ѡ��λ��Сʱλ
				{
					yue++;
					if (yue==13)
						yue=1;
					display(0x80+5,yue);//ʱ�仯ʱ����д��	
					write_com(0x80+6);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(2,yue);	//��24C02�ĵ�ַ2��д������yue	
				}
				if (addr==7) 	 //���ѡ��λ��Сʱλ
				{
					nian++;
					if (nian==100)
						hour=0;
					display(0x80+2,nian);//ʱ�仯ʱ����д��	
					write_com(0x80+3);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(1,nian);	//��24C02�ĵ�ַ1��д������nian	
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
					i2c_writebyte(7,sec);	//��24C02�ĵ�ַ7��д������sec	
				}
				if (addr==2) 
				{
					min--;
					if (min==-1)
						min=59;
					display(0x80+0x47,min);//�ֱ仯ʱ����д��
					write_com(0x80+0x48);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(6,min);	//��24C02�ĵ�ַ6��д������min	
				}
				if (addr==3) 
				{
					hour--;
					if (hour==-1)
						hour=23;
					display(0x80+0x44,hour);//ʱ�仯ʱ����д��
					write_com(0x80+0x45);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(5,hour);	//��24C02�ĵ�ַ5��д������hour	
				}
				if (addr==4) 
				{
					week--;
					if (week==0)
						week=7;
					weekdisplay(week);
					write_com(0x80+15);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(4,week);	//��24C02�ĵ�ַ4��д������week	
				}
				if (addr==5) 
				{
					ri--;
					if (ri==0)
						ri=31;
					display(0x80+8,ri);//�ձ仯ʱ����д��
					write_com(0x80+9);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(3,ri);	//��24C02�ĵ�ַ3��д������ri	
				}
				if (addr==6) 
				{
					yue--;
					if (yue==0)
						yue=12;
					display(0x80+5,yue);//�±仯ʱ����д��
					write_com(0x80+6);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(2,yue);	//��24C02�ĵ�ַ2��д������yue	
				}
				if (addr==7) 
				{
					nian--;
					if (nian==-1)
						nian=99;
					display(0x80+2,nian);//��仯ʱ����д��
					write_com(0x80+3);//д��󽫹���ƻز���˸����ʾ����״̬
					i2c_writebyte(1,nian);	//��24C02�ĵ�ַ1��д������nian	
				}
			}
		}
	}
	/*�ж��Ƿ��������ģʽ*/
	if(s4==0)
	{
		delayms (10);		   //��ʱ����
		if(s4==0)			   //�ظ��ж�,����
		{
			alarm=1;
			setting=1;
			beeper();		   //��������ʱ��������,��Ϊ����
			led1=0;			   //��������ģʽ,���Ʒ���
			delayms(200);	   //��ʱ
			led1=1;
			TR0=0;
			screen();		   //��������ģʽ����
			
			do
			{
				if (s1==0)
				{
					delayms (10);		   //��ʱ����
					if (s1==0)			   //�ظ��ж�,����
					{
												addr++;		   //Һ��ѡ��λ�仯
						beeper();	   //��������ʱ��������0.1s,��Ϊ����
						while (!s1);   //�ȴ������ָ�
						
						if(addr==1)
						{
							write_com(0x80+0x40+2);	 //����Ƶ�ʱλ��(�ڶ���7λ)
							write_com(0x0f);		 //�����˸	
						}
						if(addr==2)
						{
							write_com(0x80+0x40+7);	 //����Ƶ�ʱλ��(�ڶ���7λ)
							write_com(0x0f);		 //�����˸	
						}						
						if (addr==3)					 
						{
							write_com(0x80+0x40+10);  //����Ƶ���λ��(�ڶ���11λ)
							write_com(0x0f);		  //�����˸
						}
						if (addr==4)
						{
							addr=0;				  	  //���ѡ��λ����(ȡ��ѡ��)
							write_com(0x0c);		  //����ʾ,��ȡ�������˸
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
						if (addr==1)	//���ѡ��λ�����ӿ���ѡ��λ
						{
							ialarm++;			 
							if (ialarm==2)
								ialarm=0;
							chosealarm(ialarm);
							write_com(0x80+0x40+2);//д��󽫹���ƻز���˸����ʾΪ����״̬
							i2c_writebyte(11,ialarm);	//��24C02�ĵ�ַ1��д������ialarm	
						}
						if (addr==2)	//���ѡ��λ��ʱλ
						{
							ihour++;			 
							if (ihour==24)
								ihour=0;
							display(0x80+0x40+6,ihour);//ʱ�仯ʱ����д��
							write_com(0x80+0x40+7);//д��󽫹���ƻز���˸����ʾΪ����״̬
							i2c_writebyte(12,ihour);	//��24C02�ĵ�ַ12��д������ihour	
						}
						if (addr==3) 	 //���ѡ��λ�ڷ���λ
						{
							imin++;
							if (imin==60)
								imin=0;	
							display(0x80+0x40+9,imin);//�ֱ仯ʱ����д��
							write_com(0x80+0x40+10);//д��󽫹���ƻز���˸����ʾ����״̬
							i2c_writebyte(13,imin);	//��24C02�ĵ�ַ1��д������imin	
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
							ialarm--;
							if (ialarm==-1)
								ialarm=1;
							chosealarm(ialarm);
							write_com(0x80+0x40+2);//д��󽫹���ƻز���˸����ʾ����״̬
							i2c_writebyte(11,ialarm);	//��24C02�ĵ�ַ1��д������ialarm	
						}
						if (addr==2)
 						{
							ihour--;
							if (ihour==-1)
								ihour=23;
							display(0x80+0x40+6,ihour);//ʱ�仯ʱ����д��
							write_com(0x80+0x40+7);//д��󽫹���ƻز���˸����ʾ����״̬
							i2c_writebyte(12,ihour);	//��24C02�ĵ�ַ12��д������ihour	
						}
						if (addr==3) 
						{
							imin--;
							if (imin==-1)
								imin=59;
							display(0x80+0x40+9,imin);//�ֱ仯ʱ����д��
							write_com(0x80+0x40+10);//д��󽫹���ƻز���˸����ʾ����״̬
							i2c_writebyte(13,imin);	//��24C02�ĵ�ַ1��д������imin	
						}
					}
				}
				
				if(s4==0)
				{
					delayms (10);		   //��ʱ����
					if(s4==0)			   //�ظ��ж�,����
					{
						beeper();		   //��������ʱ��������,��Ϊ����
						led1=0;			   //��������ģʽ,���Ʒ���
						delayms(200);	   //��ʱ
						led1=1;
						TR0=1;
						setting=0;

						write_com(0x01);		 //00000001B,��ʾ��0������ָ����0

						display(0x80,tou);
						display(0x80+2,nian);
						write_data('-');
						display(0x80+5,yue);
   						write_data('-');
						display(0x80+8,ri);
						weekdisplay(week);
						display(0x80+0x44,hour);
						write_data(':');
						display(0x80+0x47,min);
						write_data(':');
						display(0x80+0x4a,sec);
					}
				}
			}while(setting);
		}
	}
}

/*����ģʽ��ʾ*/
void screen()
{
	uchar i;

	write_com(0x01);		 //00000001B,��ʾ��0������ָ����0

	write_com (0x80+0x01);
	for (i=0;i<14;i++)
	{
		write_data(table[i]);	
	}
	write_com(0x80+0x40+1);
	chosealarm(ialarm);
	display(0x80+0x40+6,ihour);
	write_data(':');
	display(0x80+0x40+9,imin);	
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

/*���ڼ���ʾ����*/
void weekdisplay(uchar a)
{
	switch(a)
	{
		case 1:	write_com(0x80+13);
				write_data('M');
				write_data('0');
				write_data('N');
				break;
		case 2:	write_com(0x80+13);
				write_data('T');
				write_data('U');
				write_data('E');
				break;
		case 3:	write_com(0x80+13);
				write_data('W');
				write_data('E');
				write_data('D');
				break;
		case 4:	write_com(0x80+13);
				write_data('T');
				write_data('H');
				write_data('U');
				break;
		case 5:	write_com(0x80+13);
				write_data('F');
				write_data('R');
				write_data('I');
				break;
		case 6:	write_com(0x80+13);
				write_data('S');
				write_data('A');
				write_data('T');
				break;
		case 7:	write_com(0x80+13);
				write_data('S');
				write_data('U');
				write_data('N');
				break;
	}
}

/*����ģʽѡ����*/
void chosealarm(uchar b)
{
	if(0==b)
	{
		write_com(0x80+0x40+1);
		write_data('O');
		write_data('N');
		write_data(' ');
	}
	if(1==b)
	{
		write_com(0x80+0x40+1);
		write_data('O');
		write_data('F');
		write_data('F');
	}
}		

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
char i2c_readbyte(uchar address)
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
void i2c_writebyte(uchar address, char dat)
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