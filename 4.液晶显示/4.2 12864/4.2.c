#include <reg52.h>
#include <intrins.h>
#include <stdlib.h>	
#define uchar unsigned char
#define uint  unsigned int
/*****************�˿ڶ���****************************/
/*Һ��*/
#define LCD_data  P0             //���ݿ�
sbit lcden=P3^4;           //LCDENҺ��ʹ�ܶ�
sbit lcdrs=P3^5;           //RSҺ����������ѡ���
sbit lcdwr=P3^6;		   //WRҺ����дѡ���
/*�����*/
sbit dula=P2^6;
sbit wela=P2^7;
uchar dis1[10]; 
uchar code dis1[]={"���µ��ա�"};
uchar code dis2[]={"԰����Լ��Գ�У�"};
uchar code dis3[]={"С��������������"};
uchar code dis4[]={"����һ���գ�"};
	
	
/******************************************************
******************������*******************************
******************************************************/
void main()
{
    uchar i;
    init();                 //��ʼ��LCD             

    lcd_pos(0,0);             //������ʾλ��Ϊ�ڶ��еĵ�1���ַ�
    i=0;
    while(dis1[i] != '\0')
    {
       write_data(dis1[i]);      //��ʾ�ַ�
       i++;
    }

 	lcd_pos(1,0);             //������ʾλ��Ϊ�����еĵ�1���ַ�
    i = 0;
    while(dis2[i] != '\0')
    {
       write_data(dis2[i]);      //��ʾ�ַ�
       i++;
    }

	lcd_pos(2,0);             //������ʾλ��Ϊ�����еĵ�1���ַ�
    i = 0;
    while(dis3[i] != '\0')
    {
       write_dat(dis3[i]);      //��ʾ�ַ�
       i++;
    }

	lcd_pos(3,0);             //������ʾλ��Ϊ�����еĵ�1���ַ�
    i = 0;
    while(dis3[i] != '\0')
    {
       write_dat(dis3[i]);      //��ʾ�ַ�
       i++;
    }

    while(1);
}


/******************************************************
******************��ʼ��*******************************
******************************************************/                                                                */
/*��ʼ������*/
void init()
{ 
    wela=0;
	dula=0;
	lcden=0;		//�ر�Һ��ʹ�ܶ�
	
	write_com(0x30);      //����ָ�����
    delayms(5);
    write_com(0x0C);      //��ʾ�����ع��
    delayms(5);
    write_com(0x01);      //���LCD����ʾ����
    delayms(5);
}


/******************************************************
******************Һ����ʾ*****************************
******************************************************/
/*Һ������д��*/
void write_com(uchar com)
{
	lcdwr=0;
	lcdrs=0;
	lcden=0;
	P0=com;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}

/*Һ������д��*/
void write_data(uchar date)
{
	lcdwr=0;
	lcdrs=1;
	lcden=0;
	P0=date;
	delayms(5);
	lcden=1;
	delayms(5);
	lcden=0;
}

/*Һ����ʾλ������*/
void lcd_pos(uchar x,uchar y)
{                          
    uchar pos;
    switch(x)
    {
   		case 0: x=0x80; break;	 //line1
		case 1: x=0x90; break;	 //line2
		case 2: x=0x88; break;	 //line3
		csae 3: x=0x98; break;	 //line4
    }
    pos=x+y;  
    write_com(pos);     //��ʾ��ַ
}


/******************************************************
******************��ʱ����*****************************
******************************************************/
/*ms����ʱ����*/
void delay_1ms(uint x) 
{ 
uint i,j; 
for(j=0;j<x;j++)  
	for(i=0;i<110;i++); 
}

