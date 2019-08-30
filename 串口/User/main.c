
#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"oled.h"
#include "delay.h"
#include"usart.h"
u32 TIM_CAP = 0;
u32 FINISH = 0x8;
u32 RISING = 0x4;
u32 TIMS =   0x3;
u32 DATA = 0xfffffff0;



//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM9CH2_CAPTURE_STA = 0;	//输入捕获状态		    				
u32	TIM9CH2_CAPTURE_VAL;	//输入捕获值(TIM2/TIM9是32位)



void init(void);
void TIM1_BRK_TIM9_IRQHandler(void);

int main(void)
{
  
  GPIO_InitTypeDef ioH;
  ioH.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  ioH.GPIO_Mode = GPIO_Mode_OUT;
  ioH.GPIO_OType = GPIO_OType_PP;
  ioH.GPIO_PuPd = GPIO_PuPd_UP;
  ioH.GPIO_Speed = GPIO_Speed_50MHz;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
  GPIO_Init(GPIOH, &ioH);
  GPIOH->ODR = 0xffff;


  GPIO_InitTypeDef ioA;
  ioA.GPIO_Pin = GPIO_Pin_0;
  ioA.GPIO_Mode = GPIO_Mode_IN;
  ioA.GPIO_OType = GPIO_OType_PP;
  ioA.GPIO_PuPd = GPIO_PuPd_DOWN;
  ioA.GPIO_Speed = GPIO_Speed_50MHz;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &ioA);

  RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
  EXTI_InitTypeDef exit_init;
  exit_init.EXTI_Line = EXTI_Line0;              
  exit_init.EXTI_LineCmd = ENABLE;              
  exit_init.EXTI_Mode = EXTI_Mode_Interrupt;     
  exit_init.EXTI_Trigger = EXTI_Trigger_Rising;   
  EXTI_Init(&exit_init);
  NVIC_InitTypeDef nvic;
  nvic.NVIC_IRQChannel = EXTI0_IRQn;             
  nvic.NVIC_IRQChannelCmd = ENABLE;              
  nvic.NVIC_IRQChannelPreemptionPriority = 0;    
  nvic.NVIC_IRQChannelSubPriority = 0;            
  NVIC_Init(&nvic);


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init(180);
  init();
  OLED_Init();
  uart_init(115200);
  GPIOI->ODR = 0x0000;
	char a[20];
  long long temp=0; 
 /* while(1){
    if(TIM9CH2_CAPTURE_STA & 0X80){    //成功捕获到了一次高电平    
    	temp = TIM9CH2_CAPTURE_STA & 0X3F; 
    	temp *= 0xFFFF;		 		              //溢出时间总和
    	temp += TIM9CH2_CAPTURE_VAL;		    //得到总的高电平时间
    	
			sprintf(a, "%.3fm", (temp/1000000.0) * 340);
			OLED_ShowString(0, 0, a, 12);
			printf("%s\r\n", a);   //打印总的高点平时间
			OLED_Refresh_Gram();
    	TIM9CH2_CAPTURE_VAL = 0;	
      TIM9CH2_CAPTURE_STA = 0;	
      }	      //开启下一次捕获 
  }*/
	
	int8_t wave = -100,c = 010;
	while(1){
	//	wave += 10;
		if(wave == 127)
			wave = -127;
		while(c--){
		USART_SendData(USART1, wave);
		}
		c = 10;
	}
}

void init(){
  GPIO_InitTypeDef ioE;
  ioE.GPIO_Pin = GPIO_Pin_6;
  ioE.GPIO_Mode = GPIO_Mode_AF;
  ioE.GPIO_OType = GPIO_OType_PP;
  ioE.GPIO_PuPd = GPIO_PuPd_DOWN;
  ioE.GPIO_Speed = GPIO_Speed_100MHz;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &ioE);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);

  GPIO_InitTypeDef ioI;
  ioI.GPIO_Pin = GPIO_Pin_8;
  ioI.GPIO_Mode = GPIO_Mode_OUT;
  ioI.GPIO_OType = GPIO_OType_PP;
  ioI.GPIO_PuPd = GPIO_PuPd_DOWN;
  ioI.GPIO_Speed = GPIO_Speed_100MHz;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
  GPIO_Init(GPIOI, &ioI);


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
  TIM_TimeBaseInitTypeDef tim9;
  tim9.TIM_ClockDivision = TIM_CKD_DIV1;
  tim9.TIM_CounterMode = TIM_CounterMode_Up;
  tim9.TIM_Period = 0xffff;
  tim9.TIM_Prescaler = 180 - 1;
  TIM_TimeBaseInit(TIM9, &tim9);

  TIM_ICInitTypeDef tim9ic;
  tim9ic.TIM_Channel = TIM_Channel_2;
  tim9ic.TIM_ICPolarity = TIM_ICPolarity_Rising;
  tim9ic.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  tim9ic.TIM_ICSelection = TIM_ICSelection_DirectTI;
  tim9ic.TIM_ICFilter = 0x00;
  TIM_ICInit(TIM9, &tim9ic);
  TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM9, TIM_IT_CC2, ENABLE);

  NVIC_InitTypeDef tim;
  tim.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
  tim.NVIC_IRQChannelCmd = ENABLE;
  tim.NVIC_IRQChannelPreemptionPriority = 2;
  tim.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&tim);
  TIM_Cmd(TIM9, ENABLE);
}
 
/*void TIM1_BRK_TIM9_IRQHandler(){
  if(TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET){
    if((TIM_CAP & RISING) == RISING )
      TIM_CAP++;
    TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
  }
  if(TIM_GetITStatus(TIM9, TIM_IT_CC2) == SET){
    GPIOH->ODR ^= GPIO_Pin_12;
      if((TIM_CAP & RISING) == 0){
        TIM_CAP = 0;
        TIM_CAP |= RISING;
        TIM_Cmd(TIM9, DISABLE);
        TIM_SetIC2Prescaler(TIM9, TIM_ICPolarity_Falling);
        TIM_SetCounter(TIM9, 0);
        TIM_Cmd(TIM9, ENABLE);
      }else{
        GPIOH->ODR ^= GPIO_Pin_11;
        TIM_CAP |= FINISH;
        TIM_CAP |= ((TIM_CAP & 0x3) * 0xffff + TIM_GetCapture2(TIM9)) << 4;
        printf("%d\n",(TIM_CAP & DATA) >> 4);
        TIM_SetIC2Prescaler(TIM9, TIM_ICPolarity_Rising);
      } 
      TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);       
  }
}*/


void TIM1_BRK_TIM9_IRQHandler(){
  	if((TIM9CH2_CAPTURE_STA & 0X80) == 0)//还未成功捕获	
	  {
		if(TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM9CH2_CAPTURE_STA & 0X40)//已经捕获到高电平了
			{
				if((TIM9CH2_CAPTURE_STA & 0X3F) == 0X3F)//高电平太长了
				{
					TIM9CH2_CAPTURE_STA |= 0X80;		//标记成功捕获了一次
					TIM9CH2_CAPTURE_VAL = 0xFFFF;
				}else TIM9CH2_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET)//捕获1发生捕获事件
		{	
			if(TIM9CH2_CAPTURE_STA & 0X40)		//捕获到一个下降沿 		
			{	  			
				TIM9CH2_CAPTURE_STA |= 0X80;		//标记成功捕获到一次高电平脉宽
			  TIM9CH2_CAPTURE_VAL = TIM_GetCapture2(TIM9);//获取当前的捕获值.
	 			TIM_OC2PolarityConfig(TIM9, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM9CH2_CAPTURE_STA = 0;			//清空
				TIM9CH2_CAPTURE_VAL = 0;
				TIM9CH2_CAPTURE_STA |= 0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM9, DISABLE); 	//关闭定时器5
	 			TIM_SetCounter(TIM9, 0);
	 			TIM_OC2PolarityConfig(TIM9, TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM9, ENABLE); 	//使能定时器5
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM9, TIM_IT_CC2 | TIM_IT_Update); //清除中断标志位
}

void EXTI0_IRQHandler(){
  GPIOI->ODR |= GPIO_Pin_8;
  delay_us(13);
	GPIOI->ODR ^= GPIO_Pin_8;
  GPIOH->ODR ^= GPIO_Pin_11;
  EXTI_ClearITPendingBit(EXTI_Line0);
}


