#include"CH452_bas.h"
#include"delay.h"
#include"myiic.h"

void delaus(){
	delay_us(10);
}

void CH452_init(void){
	IIC_Init();
	
	CH452_Write(CH452_SYSON2);
}


void CH452_I2c_Start_2(void) // ������ʼ�������Ʒ�ʽ��SDA�����ж������ʹ��������������
{
	//CH452_SDA_SET;
	IIC_SDA = 1;
	//CH452_SDA_D_IN;  /* ����SDAΪ���뷽�� */
	SDA_IN();
	//CH452_SCL_CLR;   //֪ͨCH452����Ҫ����������˶�ʱ�䲻Ҫ�ٲ��������ж�
	IIC_SCL = 0;
	do
	{
		delaus(); // �����㹻��ʱ���CH452�����ж�
		delaus();
	} while (READ_SDA == 0); // CH452���������ж�?
	delaus();
	//DISABLE_KEY_INTERRUPT; //��ֹ�����ж�,��ֹ��ʼʱ��CH452�ж϶������жϷ�������е�START
	//CH452_SDA_SET;		   /*������ʼ�����������ź�*/
	IIC_SDA = 1;
	/*CH452_SDA_D_OUT;	   // ����SDAΪ������� 
	CH452_SCL_SET;
	delaus();
	CH452_SDA_CLR; //������ʼ�ź�
	delaus();
	CH452_SCL_CLR; //ǯסI2C���ߣ�׼�����ͻ�������� 
	delaus();*/
	IIC_Start();
	
}

void CH452_I2c_Stop_2(void) // ��������
{
	/*CH452_SDA_CLR;
	SDA_OUT();  //����SDAΪ������� 
	delaus();
	CH452_SCL_SET;
	delaus();
	CH452_SDA_SET; //����I2C���߽����ź�
	delaus();
	CH452_SDA_D_IN;  //����SDAΪ���뷽�� 
	delaus();*/
	IIC_Stop();
	//CLEAR_KEY_INTER_FLAG; // ����жϱ�־,��Ϊͨ��SDA����������Ի����ж�
	//ENABLE_KEY_INTERRUPT;
}

void CH452_I2c_WrByte(unsigned char dat) //дһ���ֽ�����
{
	/*unsigned char i;
	CH452_SDA_D_OUT;		  //����SDAΪ������� 
	for (i = 0; i != 8; i++) // ���8λ����
	{
		if (dat & 0x80)
		{
			CH452_SDA_SET;
		}
		else
		{
			CH452_SDA_CLR;
		}
		delaus();
		CH452_SCL_SET;
		dat <<= 1;
		delaus();
		delaus();
		CH452_SCL_CLR;
		delaus();
	}*/
	IIC_Send_Byte(dat);
	IIC_Wait_Ack();
	/*CH452_SDA_D_IN;  //����SDAΪ���뷽�� 
	CH452_SDA_SET;
	delaus();
	CH452_SCL_SET; // ����Ӧ��
	delaus();
	delaus();
	CH452_SCL_CLR;
	delaus();*/
}


unsigned char CH452_I2c_RdByte(void) //��һ���ֽ�����
{
/*	unsigned char dat, i;
	CH452_SDA_SET;
	CH452_SDA_D_IN; // ����SDAΪ���뷽�� 
	dat = 0;
	for (i = 0; i != 8; i++) // ����8λ����
	{
		CH452_SCL_SET;
		delaus();
		delaus();
		dat <<= 1;
		if (CH452_SDA_IN)
			dat++; // ����1λ
		CH452_SCL_CLR;
		delaus();
		//		delaus();
	}
	CH452_SDA_SET;
	delaus();
	CH452_SCL_SET; // ������ЧӦ��
	delaus();
	delaus();
	CH452_SCL_CLR;
	delaus();
	return (dat);*/
	return IIC_Read_Byte(1);
}

void CH452_Write(unsigned short cmd) //д����
{
	CH452_I2c_Start_2();															/*��������*/
#ifdef ENABLE_2_CH452																// ��������CH452����
	CH452_I2c_WrByte((unsigned char)(cmd >> 7) & CH452_I2C_MASK | CH452_I2C_ADDR0); // CH452��ADDR=0ʱ
#else
	CH452_I2c_WrByte((unsigned char)(cmd >> 7) & CH452_I2C_MASK | CH452_I2C_ADDR1); // CH452��ADDR=1ʱ(Ĭ��)
#endif
	CH452_I2c_WrByte((unsigned char)cmd); /*��������*/
	CH452_I2c_Stop_2();					  /*��������*/
}

unsigned char CH452_Read(void) //��ȡ����
{
	unsigned char keycode;
	CH452_I2c_Start_2();																			 /*��������*/
	CH452_I2c_WrByte((unsigned char)(CH452_GET_KEY >> 7) & CH452_I2C_MASK | 0x01 | CH452_I2C_ADDR1); // ��������CH452����,��ADDR=0ʱ,���޸�ΪCH452_I2C_ADDR0
	keycode = CH452_I2c_RdByte();																	 /*��ȡ����*/
	CH452_I2c_Stop_2();																				 /*��������*/
	return (keycode);
}

void EXTI9_5_IRQHandler(){
	EXTI_ClearFlag(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line6);
}


