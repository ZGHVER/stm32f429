
#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"oled.h"
#include "delay.h"
#include"usart.h"
#include "myiic.h"
#include"mpu6050.h"
#include"inv_mpu_dmp_motion_driver.h"
#include"inv_mpu.h"

void main_init(void);
void timer_init(void);
int main(void)
{
  float pitch, roll, yaw; 
  short ax, ay, az;
  short temp;
  char a[20], b[20], c[20], t[20];
  u16 ts = 0;
  main_init();
  delay_init(180);
  TIM_SetCompare1(TIM9, 0);
  while(1){
    if(mpu_dmp_get_data(&pitch, &roll, &yaw) == 0){
      temp = MPU_Get_Temperature();
			MPU_Get_Accelerometer(&ax, &ay, &az);
      sprintf(t, " ts:%d",ts);
      sprintf(a, "pitch:%.2f", pitch);
      sprintf(b, " roll:%.2f", roll);
      sprintf(c, "  yaw:%.2f", yaw);
      OLED_ShowString(0, 0, a, 12);
      OLED_ShowString(0, 15, b, 12);
      OLED_ShowString(0, 30, c, 12);
      OLED_ShowString(0, 45, t, 12);
      sprintf(a, "ax:%.2f", 9.8 * ax / 15068.0);
      sprintf(b, "ay:%.2f", 9.8 * ay / 15068.0);
      sprintf(c, "az:%.2f", 9.8 * az / 15068.0);
      OLED_ShowString(80, 2, a, 12);
      OLED_ShowString(80, 22, b, 12);
      OLED_ShowString(80, 42, c, 12);
      for(temp = 0; temp < 64; temp++)
        OLED_DrawPoint(73, temp, 1);
      OLED_Refresh_Gram();
      OLED_Clear();
    if(ts >= 2000){
      ts = 50;
      TIM_SetCompare2(TIM9, ts);
      delay_ms(1000);
    }   
    else{
      ts++;
    TIM_SetCompare2(TIM9, ts);
    }
    delay_ms(10); 
    
    };
  } 
}
void main_init(){
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init(180);
  uart_init(115200);
  OLED_Init();

  if(MPU_Init() == 0){
    OLED_ShowString(0, 0, "mpu_init finish!!", 12);
    OLED_Refresh_Gram();
  }
  else{
    OLED_ShowString(0, 0, "mpu_init failed!!", 12);
    OLED_Refresh_Gram();
    return ;
  }
  if(mpu_dmp_init() == 0){
    OLED_ShowString(0, 15, "dmp_init finish!!", 12);
    OLED_Refresh_Gram();
    }
  else{
    OLED_ShowString(0, 15, "dmp_init failed!!", 12);
    OLED_Refresh_Gram();
    return ;
  }
  OLED_Refresh_Gram();
  delay_ms(1000);
  OLED_Clear(); 
  timer_init();
 
}

void timer_init(){

  GPIO_InitTypeDef ioE;
  ioE.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5;
  ioE.GPIO_Mode = GPIO_Mode_AF;
  ioE.GPIO_OType = GPIO_OType_PP;
  ioE.GPIO_PuPd = GPIO_PuPd_DOWN;
  ioE.GPIO_Speed = GPIO_Speed_50MHz;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &ioE);

  GPIO_PinAFConfig(GPIOE, EXTI_PinSource5, GPIO_AF_TIM9);
  GPIO_PinAFConfig(GPIOE, EXTI_PinSource6, GPIO_AF_TIM9);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
  TIM_TimeBaseInitTypeDef tim9;
  tim9.TIM_ClockDivision = TIM_CKD_DIV1;
  tim9.TIM_CounterMode = TIM_CounterMode_Down;      //计数器模式
  tim9.TIM_Period = 2500 - 1;                         //重装载值
  tim9.TIM_Prescaler = 1800 - 1;                   //分频系数
  TIM_TimeBaseInit(TIM9, &tim9);

  TIM_OCInitTypeDef TIM9_oc1_Init;                            //定时器9频道1PWM初始化
  TIM9_oc1_Init.TIM_OCMode = TIM_OCMode_PWM1;                 //PWM模式选择
  TIM9_oc1_Init.TIM_OCPolarity = TIM_OCPolarity_High;         //PWM有效电平设置
  TIM9_oc1_Init.TIM_OutputState = TIM_OutputState_Enable;     //输出比较状态
  TIM_OC1Init(TIM9, &TIM9_oc1_Init);
  TIM_SetCompare1(TIM9, 120);
  TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM9, ENABLE);
  
  TIM_OCInitTypeDef TIM9_oc2_Init;                            //定时器9频道2PWM初始化
  TIM9_oc2_Init.TIM_OCMode = TIM_OCMode_PWM1;                 //PWM模式选择
  TIM9_oc2_Init.TIM_OCPolarity = TIM_OCPolarity_High;         //PWM有效电平设置
  TIM9_oc2_Init.TIM_OutputState = TIM_OutputState_Enable;     //输出比较状态
  TIM_OC2Init(TIM9, &TIM9_oc2_Init);
  TIM_SetCompare2(TIM9, 200);
  TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM9, ENABLE);
  
  TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef Tim9_NvicInit;
  Tim9_NvicInit.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;                       //中断频道
  Tim9_NvicInit.NVIC_IRQChannelCmd = ENABLE;
  Tim9_NvicInit.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级，主优先级
  Tim9_NvicInit.NVIC_IRQChannelSubPriority = 0;            //提交优先级，次优先级
  NVIC_Init(&Tim9_NvicInit);

  TIM_Cmd(TIM9, ENABLE);
  
}

void TIM1_BRK_TIM9_IRQHandler(){
  TIM_ClearFlag(TIM9, TIM_IT_Update);
}

