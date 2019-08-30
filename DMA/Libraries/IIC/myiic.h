#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f4xx_rcc.h"
#include "PinDef.h"

//IO操作函数	 
#define IIC_SCL    PAout(4) //SCL
#define IIC_SDA    PAout(6) //SDA	 
#define READ_SDA   PAin(6)  //输入SDA 

#define SCL_PIN    GPIO_Pin_4
#define SDA_PIN    GPIO_Pin_6
#define IIC_GPIO   GPIOA
#define IIC_CLK    RCC_AHB1Periph_GPIOA

//IO方向设置
#define SDA_IN()  {IIC_GPIO->MODER &= ~(3 << (6 * 2));IIC_GPIO->MODER |= 0 << 6 * 2;}	
#define SDA_OUT() {IIC_GPIO->MODER &= ~(3 << (6 * 2));IIC_GPIO->MODER |= 1 << 6 * 2;} 

//IIC所有操作函??
void IIC_Init(void);                //初始化IIC的IO??				 
void IIC_Start(void);				//发送IIC开始信??
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字??
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字??
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















