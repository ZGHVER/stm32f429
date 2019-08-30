	#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"usart.h"
#include"arm_math.h"
#include"arm_const_structs.h"
#include"bsp_sdram.h"
#include"bsp_lcd.h"
#include"delay.h"
#include"RC522.h"

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  delay_init(180);
  uart_init(115200);
  LCD_Init(); 
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
  LCD_SetLayer(LCD_BACKGROUND_LAYER);
  LCD_Clear(LCD_COLOR_BLACK);
  LCD_SetLayer(LCD_FOREGROUND_LAYER); 
  LCD_SetTransparency(0xFF);
  LCD_Clear(LCD_COLOR_BLACK);
  delay_ms(100);
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine_EN_CH(LINE(4),"志刚士大夫是DHL发士大夫");
  while(1){
    
  }
}



