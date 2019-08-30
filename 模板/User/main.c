#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"oled.h"
#include "delay.h"
#include"usart.h"
#include"arm_math.h"
int main(void)
{
	//TEST a;
  delay_init(180);
  uart_init(115200);
  OLED_Init();
  OLED_ShowChar(5, 5, 'c', 12, 1);
  OLED_Refresh_Gram();
    printf("asdf");
    delay_ms(100);
/*	while(1){
	a.show();
	delay_ms(1000);
	a.show();
	delay_ms(1000);
	}*/
}


