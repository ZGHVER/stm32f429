#ifndef __PID_H_
#define __PID_H_

#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"oled.h"
#include "delay.h"
#include"usart.h"
#include"arm_math.h"

void TIM9_PWM_INIT(u16 reload, u16 psc);
void TIM4_EcodingMode_init(void);
void TIM7_BaseTIM_init(void);
void PID_init(q15_t kp, q15_t ki, q15_t kd);
#endif
