#include "stm32f4xx.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"
#include"arm_math.h"
#include"stm32f4xx_adc.h"
#include"bsp_debug_usart.h"
#include"delay.h"
#include"oled.h"
#include"CH452_bas.h"
#include"myiic.h"

void INITS(void );
void ADC_gpioinit(void);
void ADC_COMinit(void);
void ADC1_init(void);

__IO uint16_t ADC_ConvertedValue;
__IO float ADC_Vol; 

int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	char s[20];
  Debug_USART_Config();
	delay_init(180);
	OLED_Init();
	delay_us(1);
	INITS();

	
		CH452_init();
		//CH452_Read();
		/*IIC_Init();
		IIC_Start();
		IIC_Send_Byte(0x40 << 1 | 0);
		IIC_Wait_Ack();*/
    while (1)
    {
			ADC_Vol =(float) ADC_ConvertedValue/4096*(float)3.3; // ��ȡת����ADֵ
    	sprintf(s, "val:%f",ADC_Vol);
			OLED_ShowString(0, 0, s, 12);
			OLED_Refresh_Gram();
      printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
      printf("\r\n The current AD value = %f V \r\n",ADC_Vol); 
			delay_ms(5);

    }
}



void ADC_gpioinit(){
	GPIO_InitTypeDef ioC;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	ioC.GPIO_Pin = GPIO_Pin_3;
	ioC.GPIO_Mode = GPIO_Mode_AN;
	ioC.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &ioC);
}

void ADC_COMinit(){
	//ADC_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_CommonInitTypeDef cominit;
	cominit.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	cominit.ADC_Mode = ADC_Mode_Independent;
	cominit.ADC_Prescaler = ADC_Prescaler_Div2;
	cominit.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&cominit);
}

void ADC1_init(){
	
	ADC_InitTypeDef adc1type;
	ADC_StructInit(&adc1type);
	adc1type.ADC_Resolution = ADC_Resolution_12b;
	adc1type.ADC_ScanConvMode = DISABLE;
	adc1type.ADC_ContinuousConvMode = ENABLE;
	adc1type.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc1type.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	adc1type.ADC_DataAlign = ADC_DataAlign_Right;
	adc1type.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &adc1type);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_56Cycles);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);


	NVIC_InitTypeDef ADC1_NvicInit;
	ADC1_NvicInit.NVIC_IRQChannel = ADC_IRQn;                       //�ж�Ƶ��
	ADC1_NvicInit.NVIC_IRQChannelCmd = ENABLE;
	ADC1_NvicInit.NVIC_IRQChannelPreemptionPriority = 1;     //��ռ���ȼ��������ȼ�
	ADC1_NvicInit.NVIC_IRQChannelSubPriority = 1;            //�ύ���ȼ��������ȼ�
	NVIC_Init(&ADC1_NvicInit);
	

	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
}

void INITS(){
	ADC_gpioinit();
	ADC_COMinit();
	ADC1_init();
}

void ADC_IRQHandler(void){
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET){
		ADC_ConvertedValue = ADC_GetConversionValue(ADC1);
	}
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}


