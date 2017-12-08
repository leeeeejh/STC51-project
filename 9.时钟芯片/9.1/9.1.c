#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

/*DS1302��at89s52��������*/
//�ܽŶ���
sbit SCLK=P3^5;           //DS1302ʱ���ź�  7��       
sbit DIO=P3^6;            //DS1302�����ź�  6��     
sbit CE=P3^7;  		  	  //DS1302Ƭѡ		5��
//λѰַ�Ĵ�������              
sbit ACC0=ACC^0;		  //ACC�ۼ���(ACC:8λ�Ĵ���)��D0λ
sbit ACC7=ACC^7;		  //ACC�ۼ�����D7λ

sbit dula=P2^6;			  //��ѡ
sbit wela=P2^7;			  //λѡ

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

/*һ����ʱ����*/
void delayms(uchar xms)
{
   	uchar i,j;
   	for(i=xms;i>0;i--)
   		for(j=114;j>0;j--);
}


/*��ʾ����*/
/* ��̬ɨ������������LED��1���ڣ���  count >=50��   //��������               */
/*                                   time  >=2ms    //����ʱ��               */
/* DS1302�룬�֣�ʱ�Ĵ�����BCD����ʽ����16���̺������"��4λ"��"��4λ"���� */
/****************************************************************************/           
void display(uchar *poi)
{  
   dula=1;
   P0=dutable[*poi % 16];	   //��ʾ���λ	
   dula=0;
   P0=0xff;					   //��Ӱ
   wela=1;
   P0=wetable[7];			   //��8�������
   wela=0;
   delayms(3);                 //��ʱ
                    
   dula=1;
   P0=dutable[*poi /16];       //��ʾ��ʮλ        
   dula=0;
   P0=0xff;					   //��Ӱ
   wela=1;
   P0=wetable[6];			   //��7�������
   wela=0;
   delayms(3);				   //��ʱ
   poi++;					   //��ַ��һ

   dula=1;
   P0=0x40;            		   //"-"�ָ���
   dula=0;
   P0=0xff;					   
   wela=1;
   P0=wetable[5];			   //��6�������
   wela=0;
   delayms(3);				   

   dula=1;
   P0=dutable[*poi % 16];      //��ʾ�ָ�λ         
   dula=0;
   P0=0xff;					   //��Ӱ
   wela=1;
   P0=wetable[4];			   //��5�������
   wela=0;
   delayms(3);

   dula=1;
   P0=dutable[*poi /16];       //��ʾ��ʮλ        
   dula=0;
   P0=0xff;
   wela=1;
   P0=wetable[3];			   //��4�������
   wela=0;
   delayms(3);
   poi++;

   dula=1;
   P0=0x40;            		   //"-"�ָ���
   dula=0;
   P0=0xff;					   
   wela=1;
   P0=wetable[2];			   //��3�������
   wela=0;
   delayms(3);	

   dula=1;
   P0=dutable[*poi %16];       //��ʾʱ��λ        
   dula=0;
   P0=0xff;
   wela=1;
   P0=wetable[1];			   //��2�������
   wela=0;
   delayms(3);
  
   dula=1;
   P0=dutable[*poi /16];       //��ʾʱʮλ        
   dula=0;
   P0=0xff;
   wela=1;
   P0=wetable[0];			   //��1�������
   wela=0;
   delayms(3);
}



void main(void)
{
  	 uchar clock[3]={0x00,0x00,0x12};   //�룬�֣�ʱ�Ĵ�����ʼֵ
  	 uchar temp=0x80;          			//д�Ĵ����ĵ�ַΪ0x80,ż��λ
  	 uchar i;
   	 write_1302(0x8e,0x00);             //WP=0 д����
  	 for(i=0;i<3;i++)					//д��ʱ���ֵ
  	 {
   	 	write_1302(temp,clock[i]);		
  		temp+=2;
  	 }
  	 write_1302(0x8e,0x80);             //WP=1 д����
  	 while(1)
  	 {  
   	 	display(clock);				    //�������ʾʱ��
		temp=0x81;                    	//���Ĵ����ĵ�ַΪ0x81,����λ
		for(i=0;i<3;i++)
 	 	{
	    	clock[i]=read_1302(temp);	//��ȡʱ��
		 	temp+=2;
	  	}
    }
}

// ds1302_writeData(0x90,0x5c); //��ֹ���,���͹���,��Բ��ɳ����  
//ds1302_writeData(0x90, 0xa6);//�������,��һ��������,��4k���� 
