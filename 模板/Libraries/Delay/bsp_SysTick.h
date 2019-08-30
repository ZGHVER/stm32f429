
#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#include "stm32f4xx.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
#define Delay_ms(x) Delay_us(100*x)	

#endif /* __SYSTICK_H */
