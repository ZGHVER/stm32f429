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


void CH452_I2c_Start_2(void) // 操作起始，两线制方式，SDA用做中断输出，使用以下两个函数
{
	//CH452_SDA_SET;
	IIC_SDA = 1;
	//CH452_SDA_D_IN;  /* 设置SDA为输入方向 */
	SDA_IN();
	//CH452_SCL_CLR;   //通知CH452，将要对其操作，此段时间不要再产生按键中断
	IIC_SCL = 0;
	do
	{
		delaus(); // 保留足够的时间给CH452产生中断
		delaus();
	} while (READ_SDA == 0); // CH452正在请求中断?
	delaus();
	//DISABLE_KEY_INTERRUPT; //禁止键盘中断,防止开始时被CH452中断而进入中断服务程序中的START
	//CH452_SDA_SET;		   /*发送起始条件的数据信号*/
	IIC_SDA = 1;
	/*CH452_SDA_D_OUT;	   // 设置SDA为输出方向 
	CH452_SCL_SET;
	delaus();
	CH452_SDA_CLR; //发送起始信号
	delaus();
	CH452_SCL_CLR; //钳住I2C总线，准备发送或接收数据 
	delaus();*/
	IIC_Start();
	
}

void CH452_I2c_Stop_2(void) // 操作结束
{
	/*CH452_SDA_CLR;
	SDA_OUT();  //设置SDA为输出方向 
	delaus();
	CH452_SCL_SET;
	delaus();
	CH452_SDA_SET; //发送I2C总线结束信号
	delaus();
	CH452_SDA_D_IN;  //设置SDA为输入方向 
	delaus();*/
	IIC_Stop();
	//CLEAR_KEY_INTER_FLAG; // 清除中断标志,因为通过SDA输出数据所以会误中断
	//ENABLE_KEY_INTERRUPT;
}

void CH452_I2c_WrByte(unsigned char dat) //写一个字节数据
{
	/*unsigned char i;
	CH452_SDA_D_OUT;		  //设置SDA为输出方向 
	for (i = 0; i != 8; i++) // 输出8位数据
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
	/*CH452_SDA_D_IN;  //设置SDA为输入方向 
	CH452_SDA_SET;
	delaus();
	CH452_SCL_SET; // 接收应答
	delaus();
	delaus();
	CH452_SCL_CLR;
	delaus();*/
}


unsigned char CH452_I2c_RdByte(void) //读一个字节数据
{
/*	unsigned char dat, i;
	CH452_SDA_SET;
	CH452_SDA_D_IN; // 设置SDA为输入方向 
	dat = 0;
	for (i = 0; i != 8; i++) // 输入8位数据
	{
		CH452_SCL_SET;
		delaus();
		delaus();
		dat <<= 1;
		if (CH452_SDA_IN)
			dat++; // 输入1位
		CH452_SCL_CLR;
		delaus();
		//		delaus();
	}
	CH452_SDA_SET;
	delaus();
	CH452_SCL_SET; // 发出无效应答
	delaus();
	delaus();
	CH452_SCL_CLR;
	delaus();
	return (dat);*/
	return IIC_Read_Byte(1);
}

void CH452_Write(unsigned short cmd) //写命令
{
	CH452_I2c_Start_2();															/*启动总线*/
#ifdef ENABLE_2_CH452																// 若有两个CH452并连
	CH452_I2c_WrByte((unsigned char)(cmd >> 7) & CH452_I2C_MASK | CH452_I2C_ADDR0); // CH452的ADDR=0时
#else
	CH452_I2c_WrByte((unsigned char)(cmd >> 7) & CH452_I2C_MASK | CH452_I2C_ADDR1); // CH452的ADDR=1时(默认)
#endif
	CH452_I2c_WrByte((unsigned char)cmd); /*发送数据*/
	CH452_I2c_Stop_2();					  /*结束总线*/
}

unsigned char CH452_Read(void) //读取按键
{
	unsigned char keycode;
	CH452_I2c_Start_2();																			 /*启动总线*/
	CH452_I2c_WrByte((unsigned char)(CH452_GET_KEY >> 7) & CH452_I2C_MASK | 0x01 | CH452_I2C_ADDR1); // 若有两个CH452并连,当ADDR=0时,需修改为CH452_I2C_ADDR0
	keycode = CH452_I2c_RdByte();																	 /*读取数据*/
	CH452_I2c_Stop_2();																				 /*结束总线*/
	return (keycode);
}

void EXTI9_5_IRQHandler(){
	EXTI_ClearFlag(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line6);
}


