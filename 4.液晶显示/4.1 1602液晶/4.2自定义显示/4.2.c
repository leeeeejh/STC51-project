#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
uchar code table1[]={0x03,0x07,0x0f,0x1f,0x1f,0x1f,0x1f,0x1f,
               		 0x18,0x1e,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,
                	 0x03,0x0f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,
               		 0x18,0x1c,0x1e,0x1f,0x1f,0x1f,0x1f,0x1f,
               		 0x0f,0x07,0x03,0x01,0x00,0x00,0x00,0x00,
               		 0x1f,0x1f,0x1f,0x1f,0x1f,0x0f,0x07,0x01,
               		 0x1f,0x1f,0x1f,0x1f,0x1f,0x1e,0x1c,0x10,
               		 0x1e,0x1c,0x18,0x10,0x00,0x00,0x00,0x00};//��ͼ��
uchar code table[]={0x00,0x07,0x04,0x04,0x04,0x14,0x0c,0x04};//��
uchar code hang1[16]={" HDU DIANZI     "};
uchar code hang2[16]={" FANGNAN        "};


/**************�ӿڶ���************************/
/*�����*/
sbit dula=P2^6;
sbit wela=P2^7;
/*Һ��*/
sbit lcden=P3^4;//LCDENҺ��ʹ�ܶ�
sbit lcdrs=P3^5;//RSҺ����������ѡ���
sbit lcdrw=P3^6;//WRҺ����дѡ��� 

/**************�Ӻ�������*****************************/
/*Һ��*/
void yj_write_com(uchar com);//д��ָ���
void yj_write_data(uchar date);//д�����ݺ���
void init(void);//��ʼ������
void delayms(uint xms);//ms����ʱ����



/******************************************************
*******************������******************************
******************************************************/
void main()
{
	uchar i,j,k,tmp;  //tmp:�������
	init();
	tmp=0x40;//����CGRAM��ַ�ĸ�ʽ��
	k=0;
	for(j=0;j<8;j++)
    {
    	for(i=0;i<8;i++)			 //��һ������
        {
        	yj_write_com(tmp+i); 	 //�����Զ����ַ��� CGRAM ��ַ 
         	yj_write_data(table1[k]);//��CGRAMд���Զ����ַ��������
         	k++;					 //k��ʾtable�����еĵڼ�����
        }
      	tmp=tmp+8;	//�Զ����5X8������ַ�����Ҫ8���ֽ������棨����λ���ã�����˾�������Ķ�Ӧ��ϵ�ˡ�
    }
	/*��ʾӢ����ĸ*/
	yj_write_com(0x80);	//��һ��
	for(i=0;i<16;i++)
	{
		yj_write_data(hang1[i]);	
	}
	yj_write_com(0x80+0x40);//�ڶ���
	for(i=0;i<16;i++)
	{
		yj_write_data(hang2[i]);	
	}
	/*��ʾ����*/
    for(i=0;i<4;i++)//�ڵ�һ�е�12��λ����ʾ��ͼ�����ϰ벿
    {
    	yj_write_com(0x80+12+i);
		yj_write_data(i);   	
    }
	for(i=4;i<8;i++)//�ڵڶ��е�12��λ����ʾ��ͼ�����°벿
    {
    	yj_write_com(0x80+0x40+12+i-4);
		yj_write_data(i);   	
    }
  	
    while (1);
}


/*��ʼ��*/
void init()
{
	dula=0;		 //�رն�ѡ
	wela=0;		 //�ر�λѡ
	lcden=0;	 //�ر�Һ��ʹ�ܶ�

	/*��ʼ��1602Һ��*/
	yj_write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	yj_write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	yj_write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0	
}


/******************************************************
*******************Һ����ʾ****************************
******************************************************/
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


/******************************************************
*******************��ʱ����****************************
******************************************************/
/*ms����ʱ����*/
void delayms(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=114;j>0;j--);
}