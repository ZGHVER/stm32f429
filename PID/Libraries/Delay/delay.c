#include "delay.h"
#include "misc.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用OS,则包括下面的头文件（以ucos为例）即�?.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//支持OS时，使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////  
//本程序只供学习使用，未经作者许可，不得用于其它任何用�?
//ALIENTEK STM32F407开发板
//使用SysTick的普通计数模式对延迟进行管理(支持OS)
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论�?:www.openedv.com
//创建日期:2014/5/2
//版本：V1.3
//版权所有，盗版必究�?
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//V1.1 20140803 
//1,delay_us,添加参数等于0判断,如果参数等于0,则直接退�?. 
//2,修改ucosii�?,delay_ms函数,加入OSLockNesting的判�?,在进入中断后,也可以准确延�?.
//V1.2 20150411  
//修改OS支持方式,以支持任意OS(不限于UCOSII和UCOSIII,理论上任意OS都可以支�?)
//添加:delay_osrunning/delay_ostickspersec/delay_osintnesting三个宏定�?
//添加:delay_osschedlock/delay_osschedunlock/delay_ostimedly三个函数
//V1.3 20150521
//修正UCOSIII支持时的2个bug�?
//delay_tickspersec改为：delay_ostickspersec
//delay_intnesting改为：delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 

static u8  fac_us=0;							//us延时倍乘�?			   
static u16 fac_ms=0;							//ms延时倍乘�?,在os�?,代表每个节拍的ms�?
	
#if SYSTEM_SUPPORT_OS							//如果SYSTEM_SUPPORT_OS定义�?,说明要支持OS�?(不限于UCOS).
//当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
//首先�?3个宏定义:
//    delay_osrunning:用于表示OS当前是否正在运行,以决定是否可以使用相关函�?
//delay_ostickspersec:用于表示OS设定的时钟节�?,delay_init将根据这个参数来初始哈systick
// delay_osintnesting:用于表示OS中断嵌套级别,因为中断里面不可以调�?,delay_ms使用该参数来决定如何运行
//然后�?3个函�?:
//  delay_osschedlock:用于锁定OS任务调度,禁止调度
//delay_osschedunlock:用于解锁OS任务调度,重新开启调�?
//    delay_ostimedly:用于OS延时,可以引起任务调度.

//本例程仅作UCOSII和UCOSIII的支�?,其他OS,请自行参考着移植
//支持UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义�?,说明要支持UCOSII				
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运�?;1,在运�?
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次�?
#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次�?
#endif

//支持UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义�?,说明要支持UCOSIII	
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运�?;1,在运�?
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次�?
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次�?
#endif


//us级延时时,关闭任务调度(防止打断us级延�?)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//使用UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);						//UCOSIII的方�?,禁止调度，防止打断us延时
#else										//否则UCOSII
	OSSchedLock();							//UCOSII的方�?,禁止调度，防止打断us延时
#endif
}

//us级延时时,恢复任务调度
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			//使用UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);					//UCOSIII的方�?,恢复调度
#else										//否则UCOSII
	OSSchedUnlock();						//UCOSII的方�?,恢复调度
#endif
}

//调用OS自带的延时函数延�?
//ticks:延时的节拍数
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);//UCOSIII延时采用周期模式
#else
	OSTimeDly(ticks);						//UCOSII延时
#endif 
}
 
//systick中断服务函数,使用OS时用�?
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)					//OS开始跑�?,才执行正常的调度处理
	{
		OSIntEnter();						//进入中断
		OSTimeTick();       				//调用ucos的时钟服务程�?               
		OSIntExit();       	 				//触发任务切换软中�?
	}
}
#endif
			   
//初始化延迟函�?
//当使用OS的时�?,此函数会初始化OS的时钟节�?
//SYSTICK的时钟固定为AHB时钟�?1/8
//SYSCLK:系统时钟频率
void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 						//如果需要支持OS.
	u32 reload;
#endif
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//不论是否使用OS,fac_us都需要使�?
#if SYSTEM_SUPPORT_OS 						//如果需要支持OS.
	reload=SYSCLK/8;						//每秒钟的计数次数 单位为M	   
	reload*=1000000/delay_ostickspersec;	//根据delay_ostickspersec设定溢出时间
											//reload�?24位寄存器,最大�?:16777216,�?168M�?,约合0.7989s左右	
	fac_ms=1000/delay_ostickspersec;		//代表OS可以延时的最少单�?	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 					//�?1/delay_ostickspersec秒中断一�?	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//开启SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;				//非OS�?,代表每个ms需要的systick时钟�?   
#endif
}								    

#if SYSTEM_SUPPORT_OS 						//如果需要支持OS.
//延时nus
//nus:要延时的us�?.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的�?	    	 
	ticks=nus*fac_us; 						//需要的节拍�? 
	delay_osschedlock();					//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        				//刚进入时的计数器�?
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退�?.
		}  
	};
	delay_osschedunlock();					//恢复OS调度											    
}  
//延时nms
//nms:要延时的ms�?
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//如果OS已经在跑�?,并且不是在中断里�?(中断里面不能任务调度)	    
	{		 
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周�? 
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时�?,采用普通方式延�?    
	}
	delay_us((u32)(nms*1000));				//普通方式延�?
}
#else  //不用ucos�?
//延时nus
//nus为要延时的us�?.	
//注意:nus的�?,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//时间加载	  		 
	SysTick->VAL=0x00;        				//清空计数�?
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数�?
	SysTick->VAL =0X00;       				//清空计数�? 
}
//延时nms
//注意nms的范�?
//SysTick->LOAD�?24位寄存器,所�?,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//�?168M条件�?,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//时间加载(SysTick->LOAD�?24bit)
	SysTick->VAL =0x00;           			//清空计数�?
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数�?
	SysTick->VAL =0X00;     		  		//清空计数�?	  	    
} 
//延时nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//这里�?540,是考虑到某些客户可能超频使�?,
											//比如超频�?248M的时�?,delay_xms最大只能延�?541ms左右�?
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
#endif
			 



































