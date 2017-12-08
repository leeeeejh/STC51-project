#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int  
#define ADD 0x02          //�ӻ���ַ
#define BN 12
uchar code table[12]={"HDU FangNan"};		
uchar rable[12];			//code����ı�����д��ROM��,���ܹ��޸�,���ﲻ�ܼ�code

void init();				//���пڳ�ʼ��
uchar master(uchar,uchar);	//����ͨ��
void error();				//���͸�λ�ź�

void main()
{
	init();				  //���пڳ�ʼ��
	master(ADD,0x01);	  
	master(ADD,0x02);
	while(1);
}

/*���пڳ�ʼ��*/
void init()
{
	TMOD=0x20;		//���ö�ʱ��1�Ĺ���ģʽ2,8λ�Զ���װ
	TH1=253;		//9600������
	TL1=253;
	PCON=0x00;		//
	TR1=1;			//������ʱ��1

	SCON=0xf8;		//SCON: SM0 SM1 SM2 REN TB8 RB8 TI  RI 
					//50h =  1   1   1   1   1   0  0   0 b
					//���п�Ϊ��ʽ3
}

/*����ͨ��*/
uchar master(uchar addr,uchar command)
{
	uchar a,temp,i;
	while(1)
	{
		SBUF=addr;					  //���ͺ��е�ַ
		while(!TI);					  //�ȴ��������
		TI=0;
		while(!RI);					  //�ȴ��ӻ��ش�
		RI=0;
		if(SBUF!=addr)				  //�ж�Ӧ���ַ�Ƿ����
			error();				  //����ַ��,����λ�ź�
		else				   		  //��ַ���
		{
		 	TB8=0;					  //���ַ��־
			SBUF=command;	 		  //������
			while(!TI);				  //�ȴ������������
				TI=0;
			while(!RI);		 		  //�ȵ��ӻ�Ӧ��
				RI=0;
			a=SBUF;			  		  //����״̬
			if((a&0x80)==0x80)		  //������δ������,���͸�λ�ź�
			{
				TB8=1;
				error();
			}
			else 
			{
				if(command==0x01)		//�Ƿ�������
				{
					if((a&0x01)==0x01)	//�ӻ�׼���ý���
					{
						do
						{
							temp=0;		//��У���
							for(i=0;i<BN;i++)
							{
								SBUF=table[i];	 //����һ����
								temp+=table[i];
								while(!TI);
									TI=0;
							}
							SBUF=temp;			 //����У���
							while(!TI);
								TI=0;
							while(!RI);
								RI=0;
						}while(SBUF!=0);		//���ղ���ȷ,�ȴ����·���(������ȷʱ������ȷ�ź�0x00,����ӻ���λ)
						TB8=1;				//�õ�ַ��־,��Ϊ������־
						return 0;
					}
				}
				else 
				{
					if((a&0x02)==0x02)	   //��������,�ӻ�׼���÷���
					{
						while(1)
						{
							temp=0;			//��У���
							for(i=0;i<BN;i++)
							{
								while(!RI);	 //�ȴ��������
									RI=0;
								rable[i]=SBUF;	   //����һλ����
								temp+=rable[i];
							}
							while(!RI);
								RI=0;
							if(SBUF==temp)		  //У���У��
							{
								SBUF=0x00;		  //У�����ͬ����00
								while(!TI);
									TI=0;
								break;
							}
							else
							{
								SBUF=0xff;		  //У��Ͳ�ͬ��ff,���½���
								while(!TI);
									TI=0;
							}
						}
						TB8=1;			//�õ�ַ��־,��Ϊ������־
						return 0;	
					}
				}
			}
		}
	}
}

/*���͸�λ�ź�*/
void error()
{
	SBUF=0xff;				  
	while(!TI);
	TI=0;
}