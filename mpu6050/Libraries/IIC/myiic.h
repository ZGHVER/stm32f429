#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f4xx_rcc.h"
#include "PinDef.h"
//IO方向设置
#define SDA_IN()  {GPIOB->MODER &= ~(3 << (7 * 2));GPIOB->MODER |= 0 << 7 * 2;}	//PB7输入模式
#define SDA_OUT() {GPIOB->MODER &= ~(3 << (7 * 2));GPIOB->MODER |= 1 << 7 * 2;} //PB7输出模式
//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

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
















