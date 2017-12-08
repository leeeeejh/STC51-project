#include <reg52.h>
#include <intrins.h>
#include <string.h>
#define uint unsigned int
#define uchar unsigned char
sbit dula=P2^6;
sbit wela=P2^7;
//����DS1302��MCU�ӿ�
sbit SCLK=P1^0;      //DS1302ʱ���ź�  7��       
sbit IO=P1^1;        //DS1302�����ź�  6��     
sbit CE=P1^2;  		 //DS1302Ƭѡ	   5��
//λѰַ�Ĵ�������              
sbit ACC0=ACC^0;		  //ACC�ۼ���(ACC:8λ�Ĵ���)��D0λ
sbit ACC7=ACC^7;		  //ACC�ۼ�����D7λ
//����1602�뵥Ƭ���Ľӿ�
sbit lcden=P3^4;	//LCDENҺ��ʹ�ܶ�
sbit lcdrs=P3^5;	//RSҺ����������ѡ���
sbit lcdwr=P3^6;	//WRҺ����дѡ���

uchar hang1[]={"2000-00-00       "};//1602��һ��
uchar hang2[]={"TIME 00:00:00    "};//1602�ڶ���
uchar *week[]={"SUN","MON","TUS","WEN","THU","FRI","SAT"};
uchar datatime[7];

void delayms(uint xms);		  //ms����ʱ����
void write_byte(uchar date);  //DS1302��д�����(������)
uchar read_byte(void);		  //DS1302����ȡ����(�½���)
void write_1302(uchar addr,uchar date);	   //DS1302:д������(���͵�ַ����д����)
uchar read_1302(uchar addr);			   //DS1302:��ȡ����(���͵�ַ���ٶ�����)
void write_com (uchar com);				   //Һ������д��
void write_data(uchar date);			   //Һ������д��
void lcd_init();						   //lcd��ʼ������
void gettime();							   //��ȡ��ǰʱ��
void display(uchar add,uchar date);        //Һ����ʾ����
void format_datatime(uchar *add,uchar date);//��ʽ�����ں���


/*������*/
void main()
{
	lcd_init();		//��ʼ��Һ��
	while(1)
	{
	 	uchar i;
		gettime();//��õ�ǰʱ��

		/*1602��һ������д��*/
		format_datatime(hang1+2,datatime[6]);	 //��
		format_datatime(hang1+5,datatime[4]);//��		//��
		format_datatime(hang1+8,datatime[3]);//��		//��
		
		strcpy(hang1+13,week[datatime[5]]);	 //����
				
		/*1602�ڶ�������д��*/
		format_datatime(hang2+5,datatime[2]); //ʱ		
		format_datatime(hang2+8,datatime[1]); //��
		format_datatime(hang2+11,datatime[0]);//��

		write_com(0x80);
		for(i=0;i<16;i++)
		{
			write_data(hang1[i]);
			delayms(1);
		}
		write_com(0x80+0x40);
		for(i=0;i<16;i++)
		{
			write_data(hang2[i]);
			delayms(1);
		}
		
		/*1602Һ����ʾ*/
//		display(0x80,hang1);
//		display(0x80+0x40,hang2);
	}
}

/*lcd��ʼ������*/
void lcd_init()
{
 	dula=0;			
	wela=0;			//�رն�ѡ��λѡ,��ֹ����ܵ���
	lcden=0;		//�ر�Һ��ʹ�ܶ�
	
	write_com(0x38);//8λ���ݽӿڣ�2����ʾ��5*7�����ַ�
	write_com(0x0c);//����ʾ���ع�����˸
	write_com(0x06);//���ݶ�д�������治����AC�Զ���1
	write_com(0x01);//��DDRAM��ACֵ
}

/*��ʽ�����ں���*/
void format_datatime(uchar *add,uchar date)
{
	add[0]=date/10+'0';
	add[1]=date%10+'0';
}

/*Һ������д��*/
void write_com (uchar com)
{
	lcdwr=0;		   //ѡ��д
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
	lcdwr=0;		  //ѡ��д
	lcdrs=1;		  //ѡ������
	P0=date;		  
	delayms (5);	  //��ʱʹҺ�������ȶ�
	lcden=1;		  //�����彫��������Һ��������
	delayms (5);
	lcden=0;
}

/*Һ����ʾ����*/
void display(uchar add,uchar *hang)//��ʾʱ���뺯����addʲôλ�� date��ʾʲô
{
	uchar i;
	write_com(add);
	for(i=0;i<16;i++)
	{
		write_data(hang[i]);
		delayms(1);
	}
}

/*��ȡ��ǰʱ��*/
void gettime()
{
 	uchar i;
	uchar temp=0x81;						//���Ĵ�����ַ0x81
	for(i=0;i<7;i++)
	{
	 	datatime[i]=read_1302(temp);
		temp+=2;					//����λ��ȡ
	}
}

/*ms����ʱ����*/
void delayms(uint xms)
{
 	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}

/*DS1302��д�����(������)*/ 
void write_byte(uchar date)
{
   uchar i;
   ACC=date;			 //��Ҫд������ݷ���ACC
   for(i=8;i>0;i--)		 //�ɵ͵���д��8λ����
   { 
      IO=ACC0;			 //ÿ�δ�����ֽ�
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
      ACC7 = IO;		 
   }
   return(ACC/16*10+ACC%16);	//2����ת10����
}

/*DS1302:д������(���͵�ַ����д����)*/ 
void write_1302(uchar addr,uchar date)
{
  	CE=0;    			//CE����Ϊ��,���ݴ�����ֹ,ֹͣ����
	SCLK=0;             //����ʱ������                   
    CE=1;  			    //CE����Ϊ��,�߼�������Ч,���¹���
    write_byte(addr);   //д���ַ
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








