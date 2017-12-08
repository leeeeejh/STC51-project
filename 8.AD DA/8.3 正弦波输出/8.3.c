/******************************************************************** 
* �ļ���  �� PCF8591T��DAʵ��.c 
* ����    :  ���ǵ�Ƭ��ȥ����PCF8591Tȥʵ��DA���ܡ� 
             �������ҲҪ��ʾ�����ϲ��ܹ۲⵽��DA���һ�����Ҳ��� 
* ������ʾ�����⵽���Ҳ� 
***********************************************************************/  
  
#include<reg52.h>  
#include <I2C.H>  
  
#define  PCF8591 0x90    //PCF8591 ��ַ  
  
// �˱�Ϊ LED ����ģ            // 0    1    2    3    4    5    6   7    8    9    A    b    c    d    E    -    L   P    U    Hidden  _ (20)  
unsigned char Disp_Tab[] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x88,0x83,0xC6,0xA1,0x86,0xbf,0xc7,0x8c,0xc1, 0xff,  0xf7 };   
  
unsigned char AD_CHANNEL;  
unsigned long xdata  LedOut[8];  
unsigned int  idata D[32];  
// 128�����Ҳ�������   
unsigned char code auc_SinParam[128] = {   
64,67,70,73,76,79,82,85,88,91,94,96,99,102,104,106,109,111,113,115,117,118,120,121,   
123,124,125,126,126,127,127,127,127,127,127,127,126,126,125,124,123,121,120,118,   
117,115,113,111,109,106,104,102,99,96,94,91,88,85,82,79,76,73,70,67,64,60,57,54,51,48,   
45,42,39,36,33,31,28,25,23,21,18,16,14,12,10,9,7,6,4,3,2,1,1,0,0,0,0,0,0,0,1,1,2,3,4,6,   
7,9,10,12,14,16,18,21,23,25,28,31,33,36,39,42,45,48,51,54,57,60};          
  
/******************************************************************* 
DAC �任, ת������                
*******************************************************************/  
bit DACconversion(unsigned char sla,unsigned char c,  unsigned char Val)  
{  
   Start_I2c();              //��������  
   SendByte(sla);            //����������ַ  
   if(ack==0)return(0);  
   SendByte(c);              //���Ϳ����ֽ�  
   if(ack==0)return(0);  
   SendByte(Val);            //����DAC����ֵ    
   if(ack==0)return(0);  
   Stop_I2c();               //��������  
   return(1);  
}  
  
/******************************************************************* 
ADC�����ֽ�[����]���ݺ���                
*******************************************************************/  
bit ISendByte(unsigned char sla,unsigned char c)  
{  
   Start_I2c();              //��������  
   SendByte(sla);            //����������ַ  
   if(ack==0)return(0);  
   SendByte(c);              //��������  
   if(ack==0)return(0);  
   Stop_I2c();               //��������  
   return(1);  
}  
  
/******************************************************************* 
ADC���ֽ����ݺ���                
*******************************************************************/  
unsigned char IRcvByte(unsigned char sla)  
{  unsigned char c;  
  
   Start_I2c();          //��������  
   SendByte(sla+1);      //����������ַ  
   if(ack==0)return(0);  
   c=RcvByte();          //��ȡ����0  
  
   Ack_I2c(1);           //���ͷǾʹ�λ  
   Stop_I2c();           //��������  
   return(c);  
}  
/******************************************************************** 
* ���� : Delay_1ms() 
* ���� : ��ʱ�ӳ�����ʱʱ��Ϊ 1ms * x 
* ���� : x (��ʱһ����ĸ���) 
* ��� : �� 
***********************************************************************/  
void Delay_1ms(unsigned char i)//1ms��ʱ  
{  
    unsigned char x,j;  
    for(j=0;j<i;j++)  
    for(x=0;x<=148;x++);   
}  
//******************************************************************/  
main()  
{   
unsigned char i=0;  
 while(1)  
 {/********����AD-DA����*************/    
   switch(AD_CHANNEL)  
   {  
     case 0: ISendByte(PCF8591,0x41);  
             D[0]=IRcvByte(PCF8591)*2;  //ADC0 ģ��ת��1  
             break;    
   
     case 1: ISendByte(PCF8591,0x42);  
             D[1]=IRcvByte(PCF8591)*2;  //ADC1  ģ��ת��2  
             break;    
  
     case 2: ISendByte(PCF8591,0x43);  
             D[2]=IRcvByte(PCF8591)*2;  //ADC2  ģ��ת��3  
             break;    
  
     case 3: ISendByte(PCF8591,0x40);  
             D[3]=IRcvByte(PCF8591)*2;  //ADC3   ģ��ת��4  
             break;    
  
     case 4: DACconversion(PCF8591,0x40, D[4]/4); //DAC   ��ģת��  
             break;  
   }  
  
   if(++AD_CHANNEL>4) AD_CHANNEL=0;  
  
 /********���½�AD��ֵ�͵�LED�������ʾ*************/  
        D[4] = auc_SinParam[i++]*2;  
        Delay_1ms(5);  
        if(i==128) i=0;  
    }    
}  
 

view plaincopy to clipboardprint?
/*************************�˲���ΪI2C���ߵ���������*************************************/  
  
#include<reg52.h>  
#include <intrins.h>  
#include <I2C.H>  
  
#define  NOP()   _nop_()   /* �����ָ�� */  
#define  _Nop()  _nop_()   /*�����ָ��*/  
  
sbit     SCL=P1^5;       //I2C  ʱ��   
sbit     SDA=P3^6;       //I2C  ����   
bit ack;                 /*Ӧ���־λ*/  
     
  
/******************************************************************* 
                     �����ߺ���                
����ԭ��: void  Start_I2c();   
����:     ����I2C����,������I2C��ʼ����.   
********************************************************************/  
void Start_I2c()  
{  
  SDA=1;         /*������ʼ�����������ź�*/  
  _Nop();  
  SCL=1;  
  _Nop();        /*��ʼ��������ʱ�����4.7us,��ʱ*/  
  _Nop();  
  _Nop();  
  _Nop();  
  _Nop();      
  SDA=0;         /*������ʼ�ź�*/  
  _Nop();        /* ��ʼ��������ʱ�����4��s*/  
  _Nop();  
  _Nop();  
  _Nop();  
  _Nop();         
  SCL=0;       /*ǯסI2C���ߣ�׼�����ͻ�������� */  
  _Nop();  
  _Nop();  
}  
  
/******************************************************************* 
                      �������ߺ���                
����ԭ��: void  Stop_I2c();   
����:     ����I2C����,������I2C��������.   
********************************************************************/  
void Stop_I2c()  
{  
  SDA=0;      /*���ͽ��������������ź�*/  
  _Nop();       /*���ͽ���������ʱ���ź�*/  
  SCL=1;      /*������������ʱ�����4��s*/  
  _Nop();  
  _Nop();  
  _Nop();  
  _Nop();  
  _Nop();  
  SDA=1;      /*����I2C���߽����ź�*/  
  _Nop();  
  _Nop();  
  _Nop();  
  _Nop();  
}  
  
/******************************************************************* 
                 �ֽ����ݷ��ͺ���                
����ԭ��: void  SendByte(UCHAR c); 
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,���� 
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)      
           ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡� 
********************************************************************/  
void  SendByte(unsigned char  c)  
{  
 unsigned char  BitCnt;  
   
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*Ҫ���͵����ݳ���Ϊ8λ*/  
    {  
     if((c<<BitCnt)&0x80)SDA=1;   /*�жϷ���λ*/  
       else  SDA=0;                  
     _Nop();  
     SCL=1;               /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/  
      _Nop();   
      _Nop();             /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/  
      _Nop();  
      _Nop();  
      _Nop();           
     SCL=0;   
    }  
      
    _Nop();  
    _Nop();  
    SDA=1;                /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/  
    _Nop();  
    _Nop();     
    SCL=1;  
    _Nop();  
    _Nop();  
    _Nop();  
    if(SDA==1)ack=0;       
       else ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/  
    SCL=0;  
    _Nop();  
    _Nop();  
}  
  
/******************************************************************* 
                 �ֽ����ݽ��պ���                
����ԭ��: UCHAR  RcvByte(); 
����:        �������մ���������������,���ж����ߴ���(����Ӧ���ź�)�� 
          ���������Ӧ����Ӧ��ӻ���   
********************************************************************/      
unsigned char   RcvByte()  
{  
  unsigned char  retc;  
  unsigned char  BitCnt;  
    
  retc=0;   
  SDA=1;                     /*��������Ϊ���뷽ʽ*/  
  for(BitCnt=0;BitCnt<8;BitCnt++)  
      {  
        _Nop();             
        SCL=0;                  /*��ʱ����Ϊ�ͣ�׼����������λ*/  
        _Nop();  
        _Nop();                 /*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/  
        _Nop();  
        _Nop();  
        _Nop();  
        SCL=1;                  /*��ʱ����Ϊ��ʹ��������������Ч*/  
        _Nop();  
        _Nop();  
        retc=retc<<1;  
        if(SDA==1)retc=retc+1;  /*������λ,���յ�����λ����retc�� */  
        _Nop();  
        _Nop();   
      }  
  SCL=0;      
  _Nop();  
  _Nop();  
  return(retc);  
}  
  
/******************************************************************** 
                     Ӧ���Ӻ��� 
����ԭ��:  void Ack_I2c(bit a); 
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����) 
********************************************************************/  
void Ack_I2c(bit a)  
{  
    
  if(a==0)SDA=0;              /*�ڴ˷���Ӧ����Ӧ���ź� */  
  else SDA=1;  
  _Nop();  
  _Nop();  
  _Nop();        
  SCL=1;  
  _Nop();  
  _Nop();                    /*ʱ�ӵ͵�ƽ���ڴ���4��s*/  
  _Nop();  
  _Nop();  
  _Nop();    
  SCL=0;                     /*��ʱ���ߣ�ǯסI2C�����Ա��������*/  
  _Nop();  
  _Nop();      
}  
 

view plaincopy to clipboardprint?
extern bit ack;  
//�����ߺ���  
extern void Start_I2c();  
//�������ߺ���    
extern void Stop_I2c();  
//Ӧ���Ӻ���  
extern void Ack_I2c(bit a);  
//�ֽ����ݷ��ͺ���  
extern void  SendByte(unsigned char  c);  
//���ӵ�ַ���Ͷ��ֽ����ݺ���                 
extern bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no) ;  
//���ӵ�ַ���Ͷ��ֽ����ݺ���     
extern bit ISendStrExt(unsigned char sla,unsigned char *s,unsigned char no);  
//���ӵ�ַ���ֽ����ݺ���                 
extern unsigned char RcvByte();  
   
   
extern void HC595SendData(unsigned int SendVal);            
  
              
 

view plaincopy to clipboardprint?
/*************************�˲���Ϊ74HC595����������ʹ��SPI��������*************************************/  
  
#include<reg52.h>  
#include <intrins.h>  
  
#define  NOP()   _nop_()    /* �����ָ�� */  
#define  _Nop()  _nop_()    /*�����ָ��*/  
  
//SPI IO  
sbit    MOSIO =P1^5;  
sbit    R_CLK =P1^6;  
sbit    S_CLK =P1^7;  
sbit    OE    =P3^6;  
  
void HC595SendData(unsigned int SendVal);  
  
  
/********************************************************************************************************* 
** ��������: HC595SendData 
** ��������: ��SPI���߷������� 
** �ܽ���������ο���ص�оƬ����(ѧϰ����������) 
*********************************************************************************************************/  
void HC595SendData(unsigned int SendVal)  
{    
  unsigned char i;  
          
  for(i=0;i<16;i++)   
   {  
    if((SendVal<<i)&0x8000) MOSIO=1; // set dataline high  
    else MOSIO=0;  
   
    S_CLK=0;  
    NOP();  
    NOP();  
    S_CLK=1;      
   }  
     
      
  R_CLK=0; //set dataline low  
  NOP();  
  NOP();  
  R_CLK=1; //Ƭѡ  
  OE=0;  
  
}  
 