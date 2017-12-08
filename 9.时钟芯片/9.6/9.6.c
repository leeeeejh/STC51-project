#include<reg52.h>
#include<string.h>
#define uint unsigned int
#define uchar unsigned char

sbit s1=P3^0;		//S1��Ϊ����ѡ���
sbit s2=P3^1;		//S2��Ϊ��ֵ�����
sbit s3=P3^2;		//S3��Ϊ���ּ�С��
sbit s4=P3^3;		//S4��Ϊʱ��/�����л���
//�ܽŶ���
sbit SCLK=P1^0;    	//DS1302ʱ���ź�  7��       
sbit DIO=P1^1;      //DS1302�����ź�  6��     
sbit CE=P1^2;  		//DS1302Ƭѡ	  5��
//λѰַ�Ĵ�������              
sbit ACC0=ACC^0;	//ACC�ۼ���(ACC:8λ�Ĵ���)��D0λ
sbit ACC7=ACC^7;	//ACC�ۼ�����D7λ

sbit dula=P2^6;		//��ѡ
sbit wela=P2^7;		//λѡ
sbit beep=P2^4;		//������

sbit lcdrw=P3^6;	//1602Һ����дѡ���
sbit lcdrs=P3^5;	//1602��������ѡ���
sbit lcden=P3^4;	//1602ʹ�ܶ�

uchar hang1[16]={"2016-01-31      "};
uchar hang2[16]={"TIME 00:00:00   "};


void write_com(uchar);		//Һ������д��
void write_data(uchar);		//Һ������д��
void display(void);				//Һ����ʾ
void delayms(uint);			//ms����ʱ����
void init(void);				//1602Һ����ʼ��
void write_1302(uchar,uchar);//DS1302:д������(���͵�ַ����д����)
uchar read_1302(uchar);		//DS1302:��ȡ����(���͵�ַ���ٶ�����)
void write_byte(uchar); 	//S1302��д�����(������)
uchar read_byte(void);		//DS1302����ȡ����(�½���)
void huoqu(uchar);			//���ڻ�ȡ����
void week(uchar);			//���ڼ���ȡ����
void keyscan(void);				//�������������޸ĺ���
void beeper(void);				//����������
void setdisplay(uchar,uchar);//�޸�״̬�¸�λλҺ��������ʾ

void main()
{
	uchar i,temp;
	uchar clock[7];//��,��,ʱ,��,��,��,��Ĵ�����ʼֵ
	init();	
	temp=0x80;			    //д�Ĵ����ĵ�ַΪ0x80
	write_1302(0x8e,0x00);			//WP=0д����
	for(i=0;i<7;i++)
	{
	 	write_1302(temp,clock[i]);
		temp+=2;					//ż��λ
	}
	write_1302(0x8e,0x80);			//д����
	while(1)
	{
		huoqu(clock);
		display();
		temp=0x81;					//���Ĵ����ĵ�ַ
		for(i=0;i<7;i++)
		{
			clock[i]=read_1302(temp);
			temp+=2;
		}							//����λ
	 }
}

void huoqu(uchar *poi)
{
	/*���ȡ*/
	hang2[11]=poi[0]/16+'0';
	hang2[12]=poi[0]%16+'0';
	/*�ֻ�ȡ*/
	hang2[8]=poi[1]/16+'0';
	hang2[9]=poi[1]%16+'0';
	/*Сʱ��ȡ*/
	hang2[5]=poi[2]/16+'0';
	hang2[6]=poi[2]%16+'0';
	/*�ջ�ȡ*/
	hang1[8]=poi[3]/16+'0';
	hang1[9]=poi[3]%16+'0';
	/*�»�ȡ*/
	hang1[5]=poi[4]/16+'0';
	hang1[6]=poi[4]%16+'0';
	/*���ȡ*/
	hang1[2]=poi[6]/16+'0';
	hang1[3]=poi[6]%16+'0';
		
	week(poi[5]);	//�ܻ�ȡ
}

/*�ܻ�ȡ*/
void week(uchar day)
{
	switch(day)
	{
		case 1:	hang1[13]='S';	hang1[14]='U';	
				hang1[15]='N';	break;
		case 2:	hang1[13]='M';	hang1[14]='O';	
				hang1[15]='N';	break;
		case 3:	hang1[13]='T';	hang1[14]='U';	
				hang1[15]='S';	break;
		case 4:	hang1[13]='W';	hang1[14]='E';	
				hang1[15]='N';	break;
		case 5:	hang1[13]='T';	hang1[14]='H';	
				hang1[15]='U';	break;
		case 6:	hang1[13]='F';	hang1[14]='R';	
				hang1[15]='I';	break;
		case 7:	hang1[13]='S';	hang1[14]='A';	
				hang1[15]='T';	break;

	}
}

/*Һ��������ʾ*/
void display(void)
{
	uchar i;
	/*1602��һ����ʾ*/
	write_com(0x80);
	for(i=0;i<16;i++)
	{
		write_data(hang1[i]);
	}
	/*1602�ڶ�����ʾ*/
	write_com(0x80+0x40);
	for(i=0;i<16;i++)
	{
		write_data(hang2[i]);
	}	
}

/*Һ������д��*/
void write_com(uchar com)
{
	lcdrw=0;		//ѡ��д
	lcdrs=0;		//ѡ������
	P0=com;
	delayms(5);		//��ʱʹҺ�������ȶ�
	lcden=1;		//�����彫��������Һ��������
	delayms(5);
	lcden=0;	
}

/*Һ������д��*/
void write_data(uchar date)
{
	lcdrw=0;		//ѡ��д
	lcdrs=1;		//ѡ������
	P0=date;
	delayms(5);		//��ʱʹҺ�������ȶ�
	lcden=1;		//�����彫��������Һ��������
	delayms(5);
	lcden=0;
}

/*ms����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}

/*1602Һ����ʼ��*/
void init(void)
{
	dula=0;
	wela=0;		//�رն�ѡ��λѡ,��ֹ����ܵ���
	lcden=0;	//�ر�Һ��ʹ�ܶ�

	write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
}

/*DS1302��д�����(������)*/ 
void write_byte(uchar date)
{
   uchar i;
   ACC=date;			 //��Ҫд������ݷ���ACC
   for(i=8;i>0;i--)		 //�ɵ͵���д��8λ����
   { 
      DIO=ACC0;			 //ÿ�δ�����ֽ�
	  SCLK=0;     		 
      SCLK=1;			 //��ʱ�ӵ�������д��һλ����
      ACC=ACC>>1;		 //����һλ,����һλ����ACC0
   }
}

/*DS1302����ȡ����(�½���)*/
uchar read_byte(void)
{
   uchar i;
   for(i=0;i<8;i++)		 //�ɵ͵��߶���8λ����
   {
      ACC=ACC>>1;		 //��ǰһ�½��ض�������������1λ,�Ӷ�ʹ�ôζ��������ݷ���ACC7
	  SCLK = 1;
	  SCLK = 0;			 //��ʱ�ӵ��½��ض���1λ����
      ACC7 = DIO;		 
   }
   return(ACC);
}

/*DS1302:д������(���͵�ַ����д����)*/ 
void write_1302(uchar addr,uchar date)
{
   	CE=0;    			//CE����Ϊ��,���ݴ�����ֹ,ֹͣ����
	SCLK=0;              //����ʱ������                   
    CE=1;  			    //CE����Ϊ��,�߼�������Ч,���¹���
    write_byte(addr);    //д���ַ
    write_byte(date);	//д������
    CE=0;				//CE����Ϊ��,���ݴ�����ֹ,ֹͣ����
    SCLK=1;
}

/*DS1302:��ȡ����(���͵�ַ���ٶ�����)*/
uchar read_1302(uchar addr)
{
    uchar temp;
    CE=0;                 //CE����Ϊ��,���ݴ�����ֹ,ֹͣ����    
    SCLK=0;  			 //����ʱ������ 
    CE=1;                 //CE����Ϊ��,�߼�������Ч,���¹���
    write_byte(addr);     //д���ַ
    temp=read_byte();	 //��ȡ����
    CE=0;				 //CE����Ϊ��,���ݴ�����ֹ,ֹͣ����
    SCLK=1;               
    return(temp);
}

/*�������������޸ĺ���*/
/*
void keyscan (void)
{
	uchar addr;	//���λ
	if(s4!=0)	//û�н������ӽ���
	{
		if (s1==0)		 //s1����,ѡ����Ҫ���õ�λ
		{
			delayms (10);		//��ʱ����
			if (s1==0)			//�ظ��ж�,����
			{
				addr++;		   //Һ��ѡ��λ�仯
				beeper();	   //��������ʱ��������0.1s,��Ϊ����
				while (!s1);   //�ȴ������ָ�
									
				if (addr==1)
				{
					TR0=0;
					write_com(0x80+0x40+12);	 //����Ƶ���λ��(�ڶ���13λ)
					write_com(0x0f);	 //�����˸
				}
				if (addr==2)
				{
					write_com(0x80+0x40+9);	 //����Ƶ���λ��(�ڶ���10λ)
					write_com(0x0f);	 //�����˸
				}
				if (addr==3)					 
				{
					write_com(0x80+0x40+6);	  //����Ƶ�ʱλ��(�ڶ���7λ)
					write_com(0x0f);	  //�����˸
				}
				if (addr==4)					 
				{
					write_com(0x80+13);	  //����Ƶ�����λ��(��һ��14λ)
					write_com(0x0f);	  //�����˸
				}
				if(addr==5)
				{
					write_com(0x80+9);	  //����Ƶ���λ��(��һ��10λ)
					write_com(0x0f);	  //�����˸
				}
				if(addr==6)
				{
					write_com(0x80+6);	  //����Ƶ���λ��(��һ��7λ)
					write_com(0xf0);	  //�����˸
				}
				if(addr==7)
				{
					write_com(0x80+3);	  //����Ƶ���λ��(��һ��4λ)
					write_com(0xf0);	  //�����˸
				}
				if (addr==8)
				{
					addr=0;				  	  //���ѡ��λ����(ȡ��ѡ��)
					write_com(0x0c);		  //����ʾ,��ȡ�������˸
					TR0=1;					  //����ʱ��,��ʼ��
				}
			}	
		}
		if(s2==0&&addr!=0)//ֻ�й��ܼ�������֮����ֵ����������
		{
			delayms (10);			//��ʱ����
			if (s2==0)				//�ظ��ж�,����
			{	
				beeper ();		//��������ʱ��������,��Ϊ����
				while (!s2);	//�ȴ������ָ�
				if (addr==1)	//���ѡ��λ����λ
 				{
					clock[0]++;			 
					if (clock[0]==60)
						clock[0]=0;
					setdisplay(0x80+0x40+12,clock[0]);//��仯ʱ����д��
					write_com(0x80+0x40+12);//д��󽫹���ƻز���˸����ʾΪ����״̬
				}
			
				if (addr==2) 	 //���ѡ��λ�ڷ���λ
				{
					clock[1]++;
					if (clock[1]==60)
						clock[1]=0;	
					setdisplay(0x80+0x40+9,clock[1]);//�ֱ仯ʱ����д��
					write_com(0x80+0x40+9);//д��󽫹���ƻز���˸����ʾ����״̬
				}

				if (addr==3) 	 //���ѡ��λ��Сʱλ
				{
					clock[2]++;
					if (clock[2]==24)
						clock[2]=0;
					setdisplay(0x80+0x40+6,clock[2]);//ʱ�仯ʱ����д��	
					write_com(0x80+0x40+6);//д��󽫹���ƻز���˸����ʾ����״̬
				}
				
				if(addr==4)		  //���ѡ��λ������λ
				{
					clock[5]++;
					if(clock[5]==8)
						clock[5]=1;
					display();			  //���ڱ仯������д��
					write_com(0x80+13);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}

				if(addr==5)		 //���ѡ��λ����λ
				{
					clock[3]++;
					if(clock[3]==32)
						clock[3]=1;
					setdisplay(0x80+9,clock[3]);   //�ձ仯������д��
					write_com(0x80+9);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}

				if(addr==6)		 //���ѡ��λ����λ
				{
					clock[4]++;
					if(clock[4]==13)
						clock[4]=1;
					setdisplay(0x80+6,clock[4]);  //�±仯������д��
					write_com(0x80+6);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}

				if(addr==7)
				{
					clock[6]++;
					if(clock[6]==100)
						clock[6]=0;
					setdisplay(0x80+3,clock[6]); //��仯ʱ��������
					write_com(0x80+3);	  //д��󽫹���ƻز���˸,��ʾ����״̬
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
				if (addr==1)	//���ѡ��λ����λ
 				{
					if (clock[0]==0)
						clock[0]=60;
					clock[0]--;			 
					setdisplay(0x80+0x40+12,clock[0]);//��仯ʱ����д��
					write_com(0x80+0x40+12);//д��󽫹���ƻز���˸����ʾΪ����״̬
				}
				
				if (addr==2) 	 //���ѡ��λ�ڷ���λ
				{
					if (clock[1]==0)
						clock[1]=60;
					clock[1]--;
					setdisplay(0x80+0x40+9,clock[1]);//�ֱ仯ʱ����д��
					write_com(0x80+0x40+9);//д��󽫹���ƻز���˸����ʾ����״̬
				}

				if (addr==3) 	 //���ѡ��λ��Сʱλ
				{
					if (clock[2]==0)
						clock[2]=24;
					clock[2]--;
					setdisplay(0x80+0x40+6,clock[2]);//ʱ�仯ʱ����д��	
					write_com(0x80+0x40+6);//д��󽫹���ƻز���˸����ʾ����״̬
				}
				
				if(addr==4)		  //���ѡ��λ������λ
				{
					if(clock[5]==1)
						clock[5]=8;
					clock[5]--;
					display();			  //���ڱ仯������д��
					write_com(0x80+13);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}

				if(addr==5)		 //���ѡ��λ����λ
				{
					if(clock[3]==1)
						clock[3]=32;
					clock[3]--;
					setdisplay(0x80+9,clock[3]);   //�ձ仯������д��
					write_com(0x80+9);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}

				if(addr==6)		 //���ѡ��λ����λ
				{
					if(clock[4]==1)
						clock[4]=13;
					clock[4]--;
					setdisplay(0x80+6,clock[4]);  //�±仯������д��
					write_com(0x80+6);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}

				if(addr==7)
				{
					if(clock[6]==0)
						clock[6]=100;
					clock[6]--;
					setdisplay(0x80+3,clock[6]); //��仯ʱ��������
					write_com(0x80+3);	  //д��󽫹���ƻز���˸,��ʾ����״̬
				}
			}
		}
	}	
}
*/

/*����������*/
void beeper(void)
{
	beep=0;				//����������
	delayms(100);		//0.1s��ʱ
	beep=1;				//�رշ�����
}

/*Һ���޸�״̬����ʾ����*/
void setdisplay(uchar add,uchar date)
{
	uchar shi,ge;
	shi=date/16+'0';	
	ge=date%16+'0';

	write_com(add);
	write_data(shi);
	write_data(ge);
}

