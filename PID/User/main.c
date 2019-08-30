#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"oled.h"
#include "delay.h"
#include"usart.h"
#include"arm_math.h"
#include"PID.h"
#include"serial_key.h"
#include"arm_const_structs.h"
#include"bsp_lcd.h"
#include"bsp_i2c_ee.h"

extern arm_pid_instance_q15 PID_type;

float32_t testInput_f32_10khz[2048];
volatile q15_t dp, di, dd;
void stop_mot(){
  GPIO_ResetBits(GPIOE, GPIO_Pin_4);
  GPIO_ResetBits(GPIOE, GPIO_Pin_5);
}  

void start_mot(){
  GPIO_SetBits(GPIOE, GPIO_Pin_4);
  GPIO_ResetBits(GPIOE, GPIO_Pin_5);
}

void key1(){
  stop_mot();
  dp++;
  PID_init(dp, di, dd);
  delay_ms(500);
  start_mot();
}

void key2(){
  stop_mot();
  dp-=1;
  PID_init(dp, di, dd);
  delay_ms(500);
  start_mot();
}

void key3(){
  stop_mot();
  di+=5;
  PID_init(dp, di, dd);
  delay_ms(500);
  start_mot();
}

void key4(){
  stop_mot();
  di-=5;
  PID_init(dp, di, dd);
  delay_ms(500);
  start_mot();
}

void key5(){
  stop_mot();
  dd+=1;
  PID_init(dp, di, dd);
  delay_ms(500);
  start_mot();
}

void key6(){
  stop_mot();
  dd-=5;
  PID_init(dp, di, dd);
  delay_ms(500);
  start_mot();
}
extern volatile q15_t targetSpeed;
void key7(){
	targetSpeed += 100;
}

void key8(){
	targetSpeed -= 100;
}

void key15(){
  stop_mot();
	TIM_Cmd(TIM7, DISABLE);
  TIM_Cmd(TIM4, DISABLE);
	TIM_Cmd(TIM9, DISABLE);
}

void key16(){
  TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM7, ENABLE);
	TIM_Cmd(TIM9, ENABLE);
  start_mot();
}

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  RCC_PCLK2Config(RCC_HCLK_Div2);
  RCC_PCLK1Config(RCC_HCLK_Div4);
  GPIO_InitTypeDef ioE;
  ioE.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  ioE.GPIO_Mode = GPIO_Mode_OUT;
  ioE.GPIO_OType = GPIO_OType_PP;
  ioE.GPIO_PuPd = GPIO_PuPd_UP;
  ioE.GPIO_Speed = GPIO_Speed_50MHz;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_Init(GPIOE, &ioE);
  stop_mot();
  u8 a = 0xfd;
  delay_init(180);
  uart_init(115200);
  OLED_Init();
  I2C_EE_Init();
  TIM4_EcodingMode_init();
  TIM9_PWM_INIT(2000 - 1, 90 - 1);
  TIM_SetCompare2(TIM9, 0);
  TIM7_BaseTIM_init();
  dp = 1500;
  di = 600;
  dd = 50;
  PID_init(dp, di, dd);
  


	SE_KeyMap_init();
  SE_KeyMap_setFun(key1, 1);
  SE_KeyMap_setFun(key2, 2);
  SE_KeyMap_setFun(key3, 3);
  SE_KeyMap_setFun(key4, 4);
  SE_KeyMap_setFun(key5, 5);
  SE_KeyMap_setFun(key6, 6);
	SE_KeyMap_setFun(key7, 7);
	SE_KeyMap_setFun(key8, 8);
  SE_KeyMap_setFun(key15, 15);
  SE_KeyMap_setFun(key16, 16);

  start_mot();
  while(1){};  
}



