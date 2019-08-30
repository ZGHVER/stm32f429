#ifndef __CH452_H_
#define __CH452_H_
#include"delay.h"
#include"PinDef.h"

#define		DELAY_1US	    {delaus();}	// MCS51<=30MHz
#define 	CH452_SCL       PAout(2)
#define 	CH452_SDA       PAout(6)

#define		CH452_SCL_SET		{CH452_SCL = 1;}
#define		CH452_SCL_CLR		{CH452_SCL = 0;}
#define		CH452_SCL_D_OUT		{}		 	 	// 设置SCL为输出方向,对于双向I/O需切换为输出
#define		CH452_SDA_SET		{CH452_SDA = 1;}
#define		CH452_SDA_CLR		{CH452_SDA = 0;}
#define		CH452_SDA_IN		(PAin(6) ) 
#define     CH452_SDA_D_OUT     {GPIOA->MODER &= ~GPIO_MODER_MODER6_1;GPIOA->MODER |= GPIO_MODER_MODER6_0;}
#define		CH452_SDA_D_IN		{GPIOA->MODER &= ~GPIO_MODER_MODER6_1;GPIOA->MODER &= ~GPIO_MODER_MODER6_0;}	// 设置SDA为输入方向,对于双向I/O需切换为输入
#define     DISABLE_KEY_INTERRUPT {EXTI->IMR &= ~EXTI_IMR_MR6;}
#define     ENABLE_KEY_INTERRUPT  {EXTI->IMR |=  EXTI_IMR_MR6;}
#define     CLEAR_KEY_INTER_FLAG  {}

#define		CH452_KEY_MIN		0x40
#define		CH452_KEY_MAX		0x7F


#define		CH452_I2C_ADDR0		0x40			// CH452的ADDR=0时的地址
#define		CH452_I2C_ADDR1		0x60			// CH452的ADDR=1时的地址,默认值
#define		CH452_I2C_MASK		0x3E			// CH452的2线接口高字节命令掩码


#define CH452_NOP		0x0000					// 空操作
#define CH452_RESET     0x0201					// 复位
#define CH452_LEVEL		0x0100					// 加载光柱值,需另加7位数据
#define CH452_CLR_BIT	0x0180					// 段位清0,需另加6位数据
#define CH452_SET_BIT	0x01C0					// 段位置1,需另加6位数据
#define CH452_SLEEP		0x0202					// 进入睡眠状态
#define CH452_LEFTMOV   0x0300		            // 设置移动方式-左移
#define CH452_LEFTCYC   0x0301		            // 设置移动方式-左循环
#define CH452_RIGHTMOV  0x0302		            // 设置移动方式-右移
#define CH452_RIGHTCYC  0x0303		            // 设置移动方式-右循环	
#define CH452_SELF_BCD	0x0380					// 自定义BCD码,需另加7位数据
#define CH452_SYSOFF    0x0400					// 关闭显示、关闭键盘
#define CH452_SYSON1    0x0401					// 开启显示
#define CH452_SYSON2    0x0403					// 开启显示、键盘
#define CH452_SYSON2W   0x0423					// 开启显示、键盘, 真正2线接口
#define CH452_NO_BCD    0x0500					// 设置默认显示方式,可另加3位扫描极限
#define CH452_BCD       0x0580					// 设置BCD译码方式,可另加3位扫描极限
#define CH452_TWINKLE   0x0600		            // 设置闪烁控制,需另加8位数据
#define CH452_GET_KEY	0x0700					// 获取按键,返回按键代码
#define CH452_DIG0      0x0800					// 数码管位0显示,需另加8位数据
#define CH452_DIG1      0x0900		            // 数码管位1显示,需另加8位数据
#define CH452_DIG2      0x0a00		            // 数码管位2显示,需另加8位数据
#define CH452_DIG3      0x0b00		            // 数码管位3显示,需另加8位数据
#define CH452_DIG4      0x0c00		            // 数码管位4显示,需另加8位数据
#define CH452_DIG5      0x0d00					// 数码管位5显示,需另加8位数据
#define CH452_DIG6      0x0e00					// 数码管位6显示,需另加8位数据
#define CH452_DIG7      0x0f00		            // 数码管位7显示,需另加8位数据

void CH452_init(void);
void CH452_I2c_Start_2(void);
void CH452_I2c_Stop_2(void);
void CH452_I2c_WrByte(unsigned char dat);
unsigned char CH452_I2c_RdByte(void);
void CH452_Write(unsigned short cmd);
void delaus(void);

#endif
