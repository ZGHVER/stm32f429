#ifndef __CH452_H_
#define __CH452_H_
#include"delay.h"
#include"PinDef.h"

#define		DELAY_1US	    {delaus();}	// MCS51<=30MHz
#define 	CH452_SCL       PAout(2)
#define 	CH452_SDA       PAout(6)

#define		CH452_SCL_SET		{CH452_SCL = 1;}
#define		CH452_SCL_CLR		{CH452_SCL = 0;}
#define		CH452_SCL_D_OUT		{}		 	 	// ����SCLΪ�������,����˫��I/O���л�Ϊ���
#define		CH452_SDA_SET		{CH452_SDA = 1;}
#define		CH452_SDA_CLR		{CH452_SDA = 0;}
#define		CH452_SDA_IN		(PAin(6) ) 
#define     CH452_SDA_D_OUT     {GPIOA->MODER &= ~GPIO_MODER_MODER6_1;GPIOA->MODER |= GPIO_MODER_MODER6_0;}
#define		CH452_SDA_D_IN		{GPIOA->MODER &= ~GPIO_MODER_MODER6_1;GPIOA->MODER &= ~GPIO_MODER_MODER6_0;}	// ����SDAΪ���뷽��,����˫��I/O���л�Ϊ����
#define     DISABLE_KEY_INTERRUPT {EXTI->IMR &= ~EXTI_IMR_MR6;}
#define     ENABLE_KEY_INTERRUPT  {EXTI->IMR |=  EXTI_IMR_MR6;}
#define     CLEAR_KEY_INTER_FLAG  {}

#define		CH452_KEY_MIN		0x40
#define		CH452_KEY_MAX		0x7F


#define		CH452_I2C_ADDR0		0x40			// CH452��ADDR=0ʱ�ĵ�ַ
#define		CH452_I2C_ADDR1		0x60			// CH452��ADDR=1ʱ�ĵ�ַ,Ĭ��ֵ
#define		CH452_I2C_MASK		0x3E			// CH452��2�߽ӿڸ��ֽ���������


#define CH452_NOP		0x0000					// �ղ���
#define CH452_RESET     0x0201					// ��λ
#define CH452_LEVEL		0x0100					// ���ع���ֵ,�����7λ����
#define CH452_CLR_BIT	0x0180					// ��λ��0,�����6λ����
#define CH452_SET_BIT	0x01C0					// ��λ��1,�����6λ����
#define CH452_SLEEP		0x0202					// ����˯��״̬
#define CH452_LEFTMOV   0x0300		            // �����ƶ���ʽ-����
#define CH452_LEFTCYC   0x0301		            // �����ƶ���ʽ-��ѭ��
#define CH452_RIGHTMOV  0x0302		            // �����ƶ���ʽ-����
#define CH452_RIGHTCYC  0x0303		            // �����ƶ���ʽ-��ѭ��	
#define CH452_SELF_BCD	0x0380					// �Զ���BCD��,�����7λ����
#define CH452_SYSOFF    0x0400					// �ر���ʾ���رռ���
#define CH452_SYSON1    0x0401					// ������ʾ
#define CH452_SYSON2    0x0403					// ������ʾ������
#define CH452_SYSON2W   0x0423					// ������ʾ������, ����2�߽ӿ�
#define CH452_NO_BCD    0x0500					// ����Ĭ����ʾ��ʽ,�����3λɨ�輫��
#define CH452_BCD       0x0580					// ����BCD���뷽ʽ,�����3λɨ�輫��
#define CH452_TWINKLE   0x0600		            // ������˸����,�����8λ����
#define CH452_GET_KEY	0x0700					// ��ȡ����,���ذ�������
#define CH452_DIG0      0x0800					// �����λ0��ʾ,�����8λ����
#define CH452_DIG1      0x0900		            // �����λ1��ʾ,�����8λ����
#define CH452_DIG2      0x0a00		            // �����λ2��ʾ,�����8λ����
#define CH452_DIG3      0x0b00		            // �����λ3��ʾ,�����8λ����
#define CH452_DIG4      0x0c00		            // �����λ4��ʾ,�����8λ����
#define CH452_DIG5      0x0d00					// �����λ5��ʾ,�����8λ����
#define CH452_DIG6      0x0e00					// �����λ6��ʾ,�����8λ����
#define CH452_DIG7      0x0f00		            // �����λ7��ʾ,�����8λ����

void CH452_init(void);
void CH452_I2c_Start_2(void);
void CH452_I2c_Stop_2(void);
void CH452_I2c_WrByte(unsigned char dat);
unsigned char CH452_I2c_RdByte(void);
void CH452_Write(unsigned short cmd);
void delaus(void);

#endif
