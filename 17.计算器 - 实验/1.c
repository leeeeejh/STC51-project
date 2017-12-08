#include<reg52.h>
#include<intrins.h>
#include<stdio.h>
#include<math.h>
#define uchar unsigned char
#define uint unsigned int

uchar code dutable[]={		//����������ܶ�ѡ��
//0   1    2	3 
0x3f,0x06,0x5b,0x4f,
//4   5	   6 	7
0x66,0x6d,0x7d,0x07,
//8   9	   .   ����ʾ
0x7f,0x6f,0x80,0x00};
 
/*led*/
sbit led1=P1^0;
/*��������*/
sbit s1=P3^0;
sbit s2=P3^1;
sbit s3=P3^2;
sbit s4=P3^3;
/*�����*/
sbit dula=P2^6;	 //��ѡ
sbit wela=P2^7;	 //λѡ
/*Һ��*/
sbit lcden=P3^4;//LCDENҺ��ʹ�ܶ�
sbit lcdrs=P3^5;//RSҺ����������ѡ���
sbit lcdrw=P3^6;//WRҺ����дѡ���
/*������*/
sbit beep=P2^3;

/*******************�Ӻ���*****************************
******************************************************/
/*Һ��*/
void yj_fh_display(uchar a);//Һ��������ʾ
void yj_write_com (uchar com);//Һ������д��
void yj_write_data(uchar date);//Һ������д��
/*����*/
void matrixkeyscan();//���󰴼�ɨ��
void keyscan();//��������ɨ��
/*��ʼ��*/
void init();//��ʼ������
/*��ʱ*/
void delayms(uint xms);//ms����ʱ

void calculate();//����


uchar code table[]={0x00,0x07,0x04,0x04,0x04,0x14,0x0c,0x04};   //�̡�

double su1,su2,su3;
unsigned long shu1,shu2,shu3; //shu1,shu2������������,��Һ������������,�����岻�ܴ���6λ����,shu3Ϊ���
uchar chang1,chang2,chang3;//chang1,chang2������¼�������ֵĳ���,chang3��¼����ܳ�
uchar digit1,digit2,digit3;//digit1,digit2��¼shu1,shu2������λ����,digit3��¼�����������
	
uchar mark,fuhao=0; //mark:�Ƿ��·���λ�ı��,��������ʾ1,û����ʾ0 ;fuhao:��¼����ķ��Ŷ�Ӧ�Ĵ���;
bit point,equal,shift,fu;//equal:���ڰ���,����Ϊ1;shift:�Ƿ�����չ��;fu:��¼�����õ������Ƿ�Ϊ����

/******************************************************
*******************������******************************
******************************************************/
void main()
{
	init();
	while(1)
	{
		
		matrixkeyscan();//�������ɨ��
		keyscan();		//��������ɨ��
	}
}

/*��ʼ������*/
void init()
{
	dula=0;
	wela=0;			 //�ر�������������ˣ���ֹ����Һ��ʱ����ܻ������
	lcden=0;		//�ر�Һ��ʹ�ܶ�		
	/*��ʼ��������*/
	shu1=0;
	shu2=0;
	shu3=0;
	chang1=0;
	chang2=0;
	chang3=0;
	digit1=0;
	digit2=0;
	digit3=0;
	mark=0;
	point=0;
	equal=0;
	shift=0;
	fu=0;

	/*��ʼ��1602Һ��*/
	yj_write_com(0x38);		 //00111000B,����Ϊ16*2��ʾ��5*7����8λ���ݽӿ�
	yj_write_com(0x0c);		 //00001100B,���ÿ���ʾ������ʾ���
	yj_write_com(0x06);		 //00000110B,дһ���ַ����ַָ���1
	yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
	
	yj_write_com(0x80);
	yj_write_com(0x0f);//�����˸
}

/******************************************************
*******************����ɨ��****************************
******************************************************/
/*�������ɨ�躯��*/
void matrixkeyscan()
{
	uchar temp,key,input=0;	//key:��¼���������;input:����Ƿ�����������

	P3=0xfe;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)	//�ظ��ж�,ȷ�ϰ�������
		{
			temp=P3;
			switch(temp)
			{
				case 0xee:	key=1;	input=1;	break;	//����1
				case 0xde:	key=2; 	input=1;	break;	//����2
				case 0xbe:	key=3;	input=1;	break;	//����3
				case 0x7e:	if(chang1!=0&&mark==0)	//����Ѿ�����������1,��û���������
							{
								if(0==shift) //û�а�����չ��
								{
									mark=1;		//��Ƿ���λ
									fuhao=11;	//�Ӻ�
								}
								if(1==shift)
								{	
									mark=1;
									fuhao=15;   //��Ȼ����
								}
								break;
							}
			}
			while(temp!=0xf0) //�ȴ������ͷ�
			{
				temp=P3;
				temp=temp&0xf0;
			}
			/*λ���������*/
			if(input==1&&0==mark&&chang1>=6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1); 
				chang1=6; 
			}
			if(input==1&&2==mark&&chang2>=6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  
				chang2=6;
			}
			/*������ʾ*/
			if(1==input&&0==mark&&chang1<6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1);  
				yj_write_data(key+'0');  //Һ����ʾ����
				shu1=shu1*10+key;	  //��1��λ
				chang1++;			  //��1��λ������
			}
			if(1==input&&2==mark&&chang2<6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);   
				yj_write_data(key+'0');  //Һ����ʾ����
				shu2=shu2*10+key;
				chang2++;
			}
			if(1==mark&&0==chang2)	 //��ʾ����
			{
				yj_fh_display(fuhao);
				mark=2;				 //��Ƿ���λ���������
			}
		}
	}
	
	P3=0xfd;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;	 //�ظ��ж�,ȷ�ϰ�������
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xed:	key=4;	input=1;	break;	//����4
				case 0xdd:	key=5;	input=1;	break;	//����5
				case 0xbd:	key=6;	input=1;	break;	//����6
				case 0x7d:	if(chang1!=0&&mark==0)	//����Ѿ�����������1,��û���������
							{
								if(0==shift) //û�а�����չ��
								{
									mark=1;		//��Ƿ���λ
									fuhao=12;	//����
								}
								if(1==shift)
								{	
									mark=1;
									fuhao=16;   //һ�����
								}
								break;
							}
			}
			while(temp!=0xf0) //�ȴ������ͷ�
			{
				temp=P3;
				temp=temp&0xf0;
			}
			/*λ���������*/
			if(input==1&&0==mark&&chang1>=6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1); 
				chang1=6; 
			}
			if(input==1&&2==mark&&chang2>=6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  
				chang2=6;
			}
			/*������ʾ*/
			if(1==input&&0==mark&&chang1<6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1);  
				yj_write_data(key+'0');  //Һ����ʾ����
				shu1=shu1*10+key;	  //��1��λ
				chang1++;			  //��1��λ������
			}
			if(1==input&&2==mark&&chang2<6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  
				yj_write_data(key+'0');  //Һ����ʾ����
				shu2=shu2*10+key;
				chang2++;
			}
			if(1==mark&&0==chang2)	 //��ʾ����
			{
				yj_fh_display(fuhao);
				mark=2;				 //��Ƿ���λ���������
			}
		}
	}

	P3=0xfb;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;	 //�ظ��ж�,ȷ�ϰ�������
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
				case 0xeb:	key=7;	input=1;	break;	//����7
				case 0xdb:	key=8;	input=1;	break;	//����8
				case 0xbb:	key=9;	input=1;	break;	//����9
				case 0x7b:	if(chang1!=0&&mark==0)	//����Ѿ�����������1,��û���������
							{
								if(0==shift) //û�а�����չ��
								{
									mark=1;		//��Ƿ���λ
									fuhao=13;	//�˺�
								}
								if(1==shift)
								{	
									mark=1;
									fuhao=17;   //�η�
								}
								break;
							}
			}
			while(temp!=0xf0) //�ȴ������ͷ�
			{
				temp=P3;
				temp=temp&0xf0;
			}
			/*λ���������*/
			if(input==1&&0==mark&&chang1>=6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1); 
				chang1=6; 
			}
			if(input==1&&2==mark&&chang2>=6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  
				chang2=6;
			}
			/*������ʾ*/
			if(input==1&&0==mark&&chang1<6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1);  
				yj_write_data(key+'0');  //Һ����ʾ����
				shu1=shu1*10+key;	  //��1��λ
				chang1++;			  //��1��λ������
			}
			if(input==1&&2==mark&&chang2<6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  
				yj_write_data(key+'0');  //Һ����ʾ����
				shu2=shu2*10+key;
				chang2++;
			}
			if(1==mark&&0==chang2)	 //��ʾ����
			{
				yj_fh_display(fuhao);
				mark=2;				 //��Ƿ���λ���������
			}
		}
	}

	P3=0xf7;
	temp=P3;
	temp=temp&0xf0;
	if(temp!=0xf0)
	{
		delayms(10);
		temp=P3;
		temp=temp&0xf0;	  //�ظ��ж�,ȷ�ϰ�������
		if(temp!=0xf0)
		{
			temp=P3;
			switch(temp)
			{
			/*	case 0xe7:	point=1;		//С�����
							if(0==mark)
							{
								digit1=shu1; //��1������Ϊ��1Ŀǰ�ĸ���
							}
							if(2==mark)
							{
								digit2=shu2;	//��2������Ϊ��2Ŀǰ�ĸ���	
							}
							yj_write_data(0x80);
							break;	*/						
				case 0xd7:	key=0;	input=1;	break;	//����0
				case 0xb7:	equal=1;	
							calculate();	   //����������
							break;
				case 0x77: 	if(chang1!=0&&mark==0)	//����Ѿ�����������1,��û���������
							{
								if(0==shift) //û�а�����չ��
								{
									mark=1;		//��Ƿ���λ
									fuhao=14;	//����
								}
								if(1==shift)
								{	
									mark=1;
									fuhao=18;   //����
								}
								break;
							} 	
			}
			while(temp!=0xf0) //�ȴ������ͷ�
			{
				temp=P3;
				temp=temp&0xf0;
			}
			/*λ���������*/
			if(input==1&&0==mark&&chang1>=6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1); 
				chang1=6; 
			}
			if(input==1&&2==mark&&chang2>=6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  
				chang2=6;
			}
			/*������ʾ*/
			if(input==1&&0==mark&&chang1<6)	//���û�а�����λȡ����1�ļ�������,����1�ĳ���С��7
			{
				yj_write_com(0x80+chang1);  
				yj_write_data(key+'0');  //Һ����ʾ����
				shu1=shu1*10+key;	  //��1��λ
				chang1++;			  //��1��λ������
			}
			if(input==1&&2==mark&&chang2<6)
			{
				if(16==fuhao)
					yj_write_com(0x80+chang1+3+chang2);
				else yj_write_com(0x80+chang1+1+chang2);  				yj_write_data(key+'0');  //Һ����ʾ����
				shu2=shu2*10+key;
				chang2++;
			}
			if(1==mark&&0==chang2)	 //��ʾ����
			{
				yj_fh_display(fuhao);
				mark=2;				 //��Ƿ���λ���������
			}
		}
	}
}

/*��������ɨ�����*/
void keyscan()
{
	/*��չ��s1+s4*/
	if((0==s4)&&(0==s1))
	{
		delayms(100);	//��ʱ500ms���ж�s1�Ƿ��԰���
		if((0==s4)&&(0==s1))		//s1�����Ƿ�պ�
		{
			if(0==shift)
			{
				shift=1;
				led1=0;
				delayms(500);
			}
			else if(1==shift)
			{
				shift=0;
				led1=1;
				delayms(500);
			}
		}
	}

	/*��λ��s1+s3*/
	if(0==s3&&0==s1)
	{
		delayms(100);
		if(0==s3&&0==s1)
		{
			if(0==mark&&chang1>0) //ֻ������shu1
			{
				chang1--;
				yj_write_com(0x80+chang1);	 //����Ƶ�ǰһλ��������ֵ�λ�ô�
				yj_write_data(' ');			 //�����λ����ʾ
				yj_write_com(0x80+chang1);
				shu1=shu1/10;				 //����ղ������ĩλ����	
			}
			else
			{
				if(2==mark&&chang2==0)	//�����˷���λȴû������shu2
				{
					fuhao=0;		 //�������λ
					mark=0;			 //������ű��λ
					yj_write_com(0x80+chang1);	  //����Ƶ�ǰһλ��������ֵ�λ�ô�
					yj_write_data(' ');			  //�����λ����ʾ
					yj_write_data(' ');			  //�����λ����ʾ
					yj_write_data(' ');			  //�����λ����ʾ
					yj_write_com(0x80+chang1);	  //����Ƶ�ǰһλ��������ֵ�λ�ô�	 
				}
				else
				{
					if(2==mark&&chang2!=0)	//�����˷���λȴû������shu2
					{
						chang2--;
						if(16==fuhao)						 //����Ƶ�ǰһλ��������ֵ�λ�ô�
							yj_write_com(0x80+chang1+3+chang2);
						else yj_write_com(0x80+chang1+1+chang2); 
						yj_write_data(' ');			 		 //�����λ����ʾ
						if(16==fuhao)						 //����Ƶ�ǰһλ��������ֵ�λ�ô�
							yj_write_com(0x80+chang1+3+chang2);
						else yj_write_com(0x80+chang1+1+chang2); 
						shu2=shu2/10;				 //����ղ������ĩλ����	
					}	
				}
			}
		}		
	}

	/*������s2+s1*/
	if((0==s2)&&(0==s1))
	{
		delayms(100);
		if((0==s2)&&(0==s1))
		{
			/*��ʼ��������*/
			shu1=0;
			shu2=0;
			shu3=0;
			chang1=0;
			chang2=0;
			chang3=0;
			digit1=0;
			digit2=0;
			digit3=0;
			mark=0;
			point=0;
			equal=0;
			shift=0;
			fu=0;		
		}

		yj_write_com(0x01);		 //00000001B,��ʾ��0������ָ����0
		yj_write_com(0x80);
		yj_write_com(0x0f);//�����˸
	}
}


/******************************************************
*******************�������****************************
******************************************************/
/*�������*/
void calculate()
{
	unsigned long a=1,b=0,c=0;
	uchar i;

	equal=0;	//����Ⱥű��λ

	if(chang1==0||(mark==2&&chang2==0))		   //û�������κ���������Ѿ���ʾ�˷���λ,Ȼ��û��������2
	{
		if(fuhao==0)					  //û�������κ�����
			yj_write_com(0x80);
		else
		{
			if(fuhao==16)				   //�ַ�Ϊlog
				yj_write_com(0x80+chang1+3);
			else yj_write_com(0x80+chang1+1);	  //�������ַ�
		}
	}
	else
	{
		if(chang2==0)					  //ֻ����������1
		{
			shu3=shu1;
			chang3=chang1;
		}
		else 
		{
			switch(fuhao)
			{
				case 11: shu3=shu1+shu2;
				/*�ӷ�*/ b=shu3;	  //��shu3��ֵ�ݴ���b��
						 for(i=0;i<7;i++)
						 {
						 	if(b>0)			//���shu3��λ����������λ����1
								chang3++;
							b=b/10;
						 }
						 if(shu3==0)
							chang3=1;
						yj_write_com(0x80+0x40+15-chang3);
						yj_write_data('=');
						for(i=chang3;i>0;i--)
							a=a*10;
						for(i=0;i<chang3;i++)
						{
							a=a/10;
							b=shu3/a;	//����,ȡ��һλ����
							shu3=shu3%a;
							yj_write_data(b+'0');
						}	
						 break;

				case 12: if(shu1>shu2)	 
							shu3=shu1-shu2;
						 if(shu2>shu1)
						 {
						 	fu=1;
							shu3=shu2-shu1;
						 }
				/*����*/ b=shu3;	  //��shu3��ֵ�ݴ���b��
						 for(i=0;i<7;i++)
						 {
						 	if(b>0)			//���shu3��λ����������λ����1
								chang3++;
							b=b/10;
						 }
						 if(shu3==0)
							chang3=1;
						 if(0==fu)
						 {
						 	yj_write_com(0x80+0x40+15-chang3);
						 	yj_write_data('=');
						 }
						 else
						 {
						 	yj_write_com(0x80+0x40+15-chang3-1);
						 	yj_write_data('=');
							yj_write_data('-');
						 }
						 for(i=chang3;i>0;i--)
							a=a*10;
						 for(i=0;i<chang3;i++)
						 {
							a=a/10;
							b=shu3/a;	//����,ȡ��һλ����
							shu3=shu3%a;
							yj_write_data(b+'0');
						 }
						 fu=0; //����������λ	
						 break;

				case 13: shu3=shu1 * shu2;
				/*�˷�*/ b=shu3;	  //��shu3��ֵ�ݴ���b��
						 for(i=0;i<8;i++)
						 {
						 	if(b>0)			//���shu3��λ����������λ����1
								chang3++;
							b=b/10;
						 }
						 if(shu3==0)
							chang3=1;
					 	 yj_write_com(0x80+0x40+15-chang3);
		   				 yj_write_data('=');
				 		 for(i=chang3;i>0;i--)
							a=a*10;
						 for(i=0;i<chang3;i++)
					 	 {
							a=a/10;
							b=shu3/a;	//����,ȡ��һλ����
							shu3=shu3%a;
							yj_write_data(b+'0');
						 }	
						 break;

				case 14: shu3=shu1 * 1000 /shu2;
				/*����*/ if(shu3%10>=5)		//4��5��
						 	shu3=shu3+10;
						 shu3=shu3/10; 
						 		
						 b=shu3;
						 if(b<10)
						 {						  
						 	yj_write_com(0x80+0x40+15-4);
							yj_write_data('=');
							yj_write_data('0');
							yj_write_data('.');	  //��ʾС����
							yj_write_data('0');
							yj_write_data(b+'0');
						 }
						 else
						 {
						 	if(b>=10&&b<100)
						 	{
						 		yj_write_com(0x80+0x40+15-4);
								yj_write_data('=');
								yj_write_data('0');
								yj_write_data('.');	  //��ʾС����
								yj_write_data(b/10+'0');
								yj_write_data(b%10+'0');
							 }
							 else
							 {
							 	if(b>100)
								{
									for(i=0;i<8;i++)
						 			{
									 	if(b>0)			//���shu3��λ����������λ����1
											chang3++;
										b=b/10;
									 }
									 yj_write_com(0x80+0x40+15-chang3-1);
		   							 yj_write_data('=');
				 					 for(i=chang3-2;i>0;i--)
										a=a*10;
							 		 c=shu3/100;   //c�Ĵ�shu3��ֵ
									 for(i=0;i<chang3-2;i++)
								 	 {
										a=a/10;
										b=c/a;	//����,ȡ��һλ����
										c=c%a;
										yj_write_data(b+'0');
						 			 }
									 yj_write_com(0x80+0x40+15-2);
									 yj_write_data('.');
									 yj_write_data((shu3%100)/10+'0');
									 yj_write_data(shu3%10+'0');			
								 } 
						 	 }
						 }
						 break;

				case 15: shu3=shu1 % shu2;
				/*ȡ��*/ b=shu3;
						 for(i=0;i<8;i++)
						 {
						 	if(b>0)			//���shu3��λ����������λ����1
								chang3++;
							b=b/10;
						 }
						 if(shu3==0)
							chang3=1;
					 	 yj_write_com(0x80+0x40+15-chang3);
		   				 yj_write_data('=');
				 		 for(i=chang3;i>0;i--)
							a=a*10;
						 for(i=0;i<chang3;i++)
					 	 {
							a=a/10;
							b=shu3/a;	//����,ȡ��һλ����
							shu3=shu3%a;
							yj_write_data(b+'0');
						 }	
						 break;

				case 16: su3=(double)(log(shu2)) /(log(shu1));
				/*����*/ shu3=su3*1000;
						 if(shu3%10>=5)		//4��5��
						 	shu3=shu3+10;
						 shu3=shu3/10; 
						 		
						 b=shu3;
						 if(b<10)
						 {						  
						 	yj_write_com(0x80+0x40+15-4);
							yj_write_data('=');
							yj_write_data('0');
							yj_write_data('.');	  //��ʾС����
							yj_write_data('0');
							yj_write_data(b+'0');
						 }
						 else
						 {
						 	if(b>=10&&b<100)
						 	{
						 		yj_write_com(0x80+0x40+15-4);
								yj_write_data('=');
								yj_write_data('0');
								yj_write_data('.');	  //��ʾС����
								yj_write_data(b/10+'0');
								yj_write_data(b%10+'0');
							 }
							 else
							 {
							 	if(b>100)
								{
									for(i=0;i<8;i++)
						 			{
									 	if(b>0)			//���shu3��λ����������λ����1
											chang3++;
										b=b/10;
									 }
									 yj_write_com(0x80+0x40+15-chang3-1);
		   							 yj_write_data('=');
				 					 for(i=chang3-2;i>0;i--)
										a=a*10;
							 		 c=shu3/100;   //c�Ĵ�shu3��ֵ
									 for(i=0;i<chang3-2;i++)
								 	 {
										a=a/10;
										b=c/a;	//����,ȡ��һλ����
										c=c%a;
										yj_write_data(b+'0');
						 			 }
									 yj_write_com(0x80+0x40+15-2);
									 yj_write_data('.');
									 yj_write_data((shu3%100)/10+'0');
									 yj_write_data(shu3%10+'0');			
								 } 
						 	 }
						 }
						 break;

				case 17: shu3=1;
				/*�η�*/ for(i=0;i<shu2;i++)
						 	shu3 *= shu1;
						 b=shu3;	  //��shu3��ֵ�ݴ���b��
						 for(i=0;i<9;i++)
						 {
						 	if(b>0)			//���shu3��λ����������λ����1
								chang3++;
							b=b/10;
						 }
						 if(shu3==0)
							chang3=1;
						 yj_write_com(0x80+0x40+15-chang3);
						 yj_write_data('=');
						 for(i=chang3;i>0;i--)
							a=a*10;
						 for(i=0;i<chang3;i++)
						 {
							a=a/10;
							b=shu3/a;	//����,ȡ��һλ����
							shu3=shu3%a;
							yj_write_data(b+'0');
						 }	
						 break;
							
				case 18: su1=shu1;
				/*����*/ su2=shu2;
						 su3=pow(su1,1/su2);
			
						 shu3=su3*1000;
						 if(shu3%10>=5)		//4��5��
						 	shu3=shu3+10;
						 shu3=shu3/10; 
						 		
						 b=shu3;
						 if(b<10)
						 {						  
						 	yj_write_com(0x80+0x40+15-4);
							yj_write_data('=');
							yj_write_data('0');
							yj_write_data('.');	  //��ʾС����
							yj_write_data('0');
							yj_write_data(b+'0');
						 }
						 else
						 {
						 	if(b>=10&&b<100)
						 	{
						 		yj_write_com(0x80+0x40+15-4);
								yj_write_data('=');
								yj_write_data('0');
								yj_write_data('.');	  //��ʾС����
								yj_write_data(b/10+'0');
								yj_write_data(b%10+'0');
							 }
							 else
							 {
							 	if(b>100)
								{
									for(i=0;i<8;i++)
						 			{
									 	if(b>0)			//���shu3��λ����������λ����1
											chang3++;
										b=b/10;
									 }
									 yj_write_com(0x80+0x40+15-chang3-1);
		   							 yj_write_data('=');
				 					 for(i=chang3-2;i>0;i--)
										a=a*10;
							 		 c=shu3/100;   //c�Ĵ�shu3��ֵ
									 for(i=0;i<chang3-2;i++)
								 	 {
										a=a/10;
										b=c/a;	//����,ȡ��һλ����
										c=c%a;
										yj_write_data(b+'0');
						 			 }
									 yj_write_com(0x80+0x40+15-2);
									 yj_write_data('.');
									 yj_write_data((shu3%100)/10+'0');
									 yj_write_data(shu3%10+'0');			
								 } 
						 	 }
						 }
						 break;
						 		 
			}
		}
		
	}
}


/******************************************************
*******************Һ����ʾ****************************
******************************************************/
/*Һ����ʾ����*/
void yj_fh_display(uchar a)
{
	uchar m;
	switch(a)
	{
		case 11: yj_write_com(0x80+chang1);
				 yj_write_data('+');	break;
		case 12: yj_write_com(0x80+chang1);
				 yj_write_data('-');	break;
		case 13: yj_write_com(0x80+chang1);
				 yj_write_data('*');	break;
		case 14: yj_write_com(0x80+chang1);
				 yj_write_data('/');	break;
		case 15: yj_write_com(0x80+chang1);
				 yj_write_data('%');	break;
		case 16: yj_write_com(0x80+chang1);
				 yj_write_data('l');
				 yj_write_data('o');
				 yj_write_data('g');
				 break;				 
		case 17: yj_write_com(0x80+chang1);
				 yj_write_data('^');	break;
		case 18: yj_write_com(0x40);
				 for(m=0;m<8;m++)
				 {
				 	yj_write_com(0x40+m);	 //�����Զ����ַ��� CGRAM ��ַ 
				 	yj_write_data(table[m]); //��CGRAMд���Զ����ַ��������
				 }
				 yj_write_com(0x80+chang1);
				 yj_write_data(0);
				 yj_write_com(0x80+chang1+1);
				 break;
	}
}


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