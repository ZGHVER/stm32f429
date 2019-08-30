#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f4xx_rcc.h"
#include "PinDef.h"

//IO��������	 
#define IIC_SCL    PAout(4) //SCL
#define IIC_SDA    PAout(6) //SDA	 
#define READ_SDA   PAin(6)  //����SDA 

#define SCL_PIN    GPIO_Pin_4
#define SDA_PIN    GPIO_Pin_6
#define IIC_GPIO   GPIOA
#define IIC_CLK    RCC_AHB1Periph_GPIOA

//IO��������
#define SDA_IN()  {IIC_GPIO->MODER &= ~(3 << (6 * 2));IIC_GPIO->MODER |= 0 << 6 * 2;}	
#define SDA_OUT() {IIC_GPIO->MODER &= ~(3 << (6 * 2));IIC_GPIO->MODER |= 1 << 6 * 2;} 

//IIC���в�����??
void IIC_Init(void);                //��ʼ��IIC��IO??				 
void IIC_Start(void);				//����IIC��ʼ��??
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ����??
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ����??
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















