#include <reg52.h>
#include <intrins.h>
#include <string.h>
#define uint unsigned int
#define uchar unsigned char
//����DS1302��MCU�ӿ�
sbit SCLK = P1^0;//ʱ��������
sbit IO = P1^1;//IO�ӿ�
sbit RST = P1^2;//��λ������
//����1602�뵥Ƭ���Ľӿ�
sbit RS = P3^5;//ָ������ѡ��
sbit RW = P3^6;//�ӵأ�ִ��д����
sbit EN = P3^4;//�ź�ʹ��

uchar *WEEK[]=
{
 	"SUN","MON","TUS","WEN","THU","FRI","SAT"
};
uchar LCD_DSY_BUFFER1[]={"DATE 00-00-00    "};
uchar LCD_DSY_BUFFER2[]={"TIME 00:00:00    "};
uchar DateTime[7];

/********��ʱi���뺯��************/
void DelayMS(uint ms)
{
 	uchar i;
	while(ms--)
	{
	 	for(i=0;i<120;i++);
	}
}
/*****��DS1302д��1�ֽ�����**********/
void Write_A_Byte_TO_DS1302(uchar x)
{
 	uchar i;
	for(i=0;i<8;i++)
	{
	 	IO=x&0x01;SCLK=1;SCLK=0;
                x>>=1;//x����1λ����λ��0
	}
}
/*****��DS1302�ж���1�ֽ�����**********/
uchar Get_A_Byte_FROM_DS1302()
{
 	uchar i,b=0x00;
	for(i=0;i<8;i++)
	{
	 	b=b|_crol_((uchar)IO,i);
		SCLK=1;SCLK=0;
	}
	return b/16*10+b%16;
}
/***********��ȡDS1302ĳ��ַ�����ݣ���д�����֣��������************/
uchar Read_Data(uchar addr)
{
 	uchar dat;
	RST = 0;//��λ
	SCLK=0;//ʱ��������0
        RST=1;//�������ݴ���
   	Write_A_Byte_TO_DS1302(addr);//д���ַ������
	dat = Get_A_Byte_FROM_DS1302();//����һ���ֽڵ�����
   	SCLK=1;RST=0;
	return dat;
}

void GetTime()
{
 	uchar i,addr=0x81;
	for(i=0;i<7;i++)
	{
	 	DateTime[i]=Read_Data(addr);addr+=2;
	}
}
/*************����LCD״̬*************/
uchar Read_LCD_State()
{
 	uchar state;
	RS=0;RW=1;EN=1;DelayMS(1);
	state=P0;
	EN = 0;DelayMS(1);
	return state;
}

/***********LCDæ���*****************/
void LCD_Busy_Wait()
{
 	while((Read_LCD_State()&0x80)==0x80);//��ȡæ��־λBF,BF=1��һֱ�ȴ�
	DelayMS(5);
}
/**************��LCDд����*******************/
void Write_LCD_Data(uchar dat)
{
 	LCD_Busy_Wait();//æ���ȷ����һָ����ɣ�Ҳ�����ʵ�����ʱ�滻����
	RS=1;RW=0;EN=0;P0=dat;EN=1;DelayMS(1);EN=0;	
}

void Write_LCD_Command(uchar cmd)
{
 	LCD_Busy_Wait();
	RS=0;RW=0;EN=0;P0=cmd;EN=1;DelayMS(1);EN=0;	
}
/*****************��ʼ��LCD����*****************/
void Init_LCD()
{
 	Write_LCD_Command(0x38);//8λ���ݽӿڣ�2����ʾ��5*7�����ַ�
	DelayMS(1);	//��ʱ��֤��һָ�����
	Write_LCD_Command(0x01);//��DDRAM��ACֵ
	DelayMS(1);	
	Write_LCD_Command(0x06);//���ݶ�д�������治����AC�Զ���1
	DelayMS(1);	
	Write_LCD_Command(0x0c);//����ʾ���ع�����˸
	DelayMS(1);	
}

void Set_LCD_POS(uchar p)
{
 	Write_LCD_Command(p|0x80);	
}

void Display_LCD_String(uchar p,uchar *s)
{
 	uchar i;
	Set_LCD_POS(p);
	for(i=0;i<16;i++)
	{
		Write_LCD_Data(s[i]);
		DelayMS(1); 	
	}
}
/******************��ʽ������ʱ�亯��**********/
void Format_DateTime(uchar d,uchar *a)
{
 	a[0]=d/10+'0';
	a[1]=d%10+'0';
}
/***************������***************/
void main()
{
 	Init_LCD();//��ʼ��Һ��
	while(1)
	{
	 	GetTime();//��õ�ǰʱ��
		Format_DateTime(DateTime[6],LCD_DSY_BUFFER1+5);//ͨ������ʾ
		Format_DateTime(DateTime[4],LCD_DSY_BUFFER1+8);
		Format_DateTime(DateTime[3],LCD_DSY_BUFFER1+11);

		strcpy(LCD_DSY_BUFFER1+13,WEEK[DateTime[5]]);

		Format_DateTime(DateTime[2],LCD_DSY_BUFFER2+5);
		Format_DateTime(DateTime[1],LCD_DSY_BUFFER2+8);
		Format_DateTime(DateTime[0],LCD_DSY_BUFFER2+11);

		Display_LCD_String(0x00,LCD_DSY_BUFFER1);//Һ����ʾ
		Display_LCD_String(0x40,LCD_DSY_BUFFER2);
	}
}
