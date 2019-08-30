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

void main(){
  ADC_CommonInitTypeDef cominit;
  cominit.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  cominit.ADC_Mode = ADC_Mode_Independent;
  cominit.ADC_Prescaler = ADC_Prescaler_Div2;
  cominit.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
  ADC_CommonInit(&cominit);

  ADC_InitTypeDef adc;
  adc.ADC_ContinuousConvMode = DISABLE;
  adc.ADC_DataAlign = ADC_DataAlign_Left;
  adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T5_CC2;
  ADC_Init(ADC1, &adc);
}

