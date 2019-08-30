#include "PID.h"
#include "PID.h"
#include "oled.h"
#include"arm_math.h"

q15_t targetSpeed = 2000;
arm_pid_instance_q15 PID_type;

u16 curARR = 0;
void PID_control(q15_t speed);
void get_speed(float* speed);

void TIM9_PWM_INIT(u16 reload, u16 psc)
{ // 2000 180/180

    GPIO_InitTypeDef ioE;
    ioE.GPIO_Pin = GPIO_Pin_6;
    ioE.GPIO_Mode = GPIO_Mode_AF;
    ioE.GPIO_OType = GPIO_OType_PP;
    ioE.GPIO_PuPd = GPIO_PuPd_UP;
    ioE.GPIO_Speed = GPIO_Speed_50MHz;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_Init(GPIOE, &ioE);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    TIM_TimeBaseInitTypeDef tim9_BaseInit;
    tim9_BaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
    tim9_BaseInit.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式
    tim9_BaseInit.TIM_Period = reload;                  //重装载值
    tim9_BaseInit.TIM_Prescaler = psc;                  //分频系数
    TIM_TimeBaseInit(TIM9, &tim9_BaseInit);

    /* TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
    NVIC_InitTypeDef TIM9_NvicInit;
    TIM9_NvicInit.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;                       //中断频道
    TIM9_NvicInit.NVIC_IRQChannelCmd = ENABLE;
    TIM9_NvicInit.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级，主优先级
    TIM9_NvicInit.NVIC_IRQChannelSubPriority = 1;            //提交优先级，次优先级
    NVIC_Init(&TIM9_NvicInit);*/

    TIM_OCInitTypeDef TIM9_oc2_Init;                        //定时器9频道2PWM初始化
    TIM9_oc2_Init.TIM_OCMode = TIM_OCMode_PWM1;             //PWM模式选择
    TIM9_oc2_Init.TIM_OCPolarity = TIM_OCPolarity_High;     //PWM有效电平设置
    TIM9_oc2_Init.TIM_OutputState = TIM_OutputState_Enable; //输出比较状态
    TIM_OC2Init(TIM9, &TIM9_oc2_Init);
    TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM9, ENABLE);
    TIM_SetCompare2(TIM9, 0);

    TIM_Cmd(TIM9, ENABLE);
}

void TIM4_EcodingMode_init()
{

    GPIO_InitTypeDef ioB;
    ioB.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    ioB.GPIO_Mode = GPIO_Mode_AF;
    ioB.GPIO_OType = GPIO_OType_PP;
    ioB.GPIO_PuPd = GPIO_PuPd_UP;
    ioB.GPIO_Speed = GPIO_Speed_100MHz;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_Init(GPIOB, &ioB);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef tim4;
    tim4.TIM_ClockDivision = TIM_CKD_DIV1;
    tim4.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式
    tim4.TIM_Period = 2000;                     //重装载值
    tim4.TIM_Prescaler = 0;                    //分频系数
    TIM_TimeBaseInit(TIM4, &tim4);

    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICInitTypeDef icint;
    TIM_ICStructInit(&icint);
    icint.TIM_ICFilter = 20;
    TIM_ICInit(TIM4, &icint);

  /*  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef TIM4_NvicInit;
    TIM4_NvicInit.NVIC_IRQChannel = TIM4_IRQn; //中断频道
    TIM4_NvicInit.NVIC_IRQChannelCmd = ENABLE;
    TIM4_NvicInit.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级，主优先级
    TIM4_NvicInit.NVIC_IRQChannelSubPriority = 0;        //提交优先级，次优先级
    NVIC_Init(&TIM4_NvicInit);
    TIM4->CNT = 0;  */
    TIM_Cmd(TIM4, ENABLE);
}

void TIM7_BaseTIM_init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //(45MHz/45)/10000 = 100Hz
    TIM_TimeBaseInitTypeDef tim7;
    tim7.TIM_ClockDivision = TIM_CKD_DIV1;
    tim7.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式
    tim7.TIM_Period = 10000;                   //重装载值
    tim7.TIM_Prescaler = 45 - 1;               //分频系数
    TIM_TimeBaseInit(TIM7, &tim7);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    NVIC_InitTypeDef TIM7_NvicInit;
    TIM7_NvicInit.NVIC_IRQChannel = TIM7_IRQn; //中断频道
    TIM7_NvicInit.NVIC_IRQChannelCmd = ENABLE;
    TIM7_NvicInit.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级，主优先级
    TIM7_NvicInit.NVIC_IRQChannelSubPriority = 2;        //提交优先级，次优先级
    NVIC_Init(&TIM7_NvicInit);

    TIM_Cmd(TIM7, ENABLE);
}

void TIM7_IRQHandler()
{
    GPIOC->ODR ^= GPIO_Pin_9;
    GPIOC->ODR |= GPIO_Pin_10;
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) == RESET)
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        return;
    }
    float speed = 0;
    get_speed(&speed);
    PID_control((q15_t)speed);
    OLED_ADDval((u8)(speed * 0.018));
    for(u8 i = 0; i < 128; i++)
        OLED_DrawPoint(i, 63 - (u8)(targetSpeed * 0.018), 1);
    OLED_Refresh_Gram();

    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    GPIOC->ODR &= ~GPIO_Pin_10;
}

void get_speed(float* speed){
    static u32 lastCNT = 0;
    u32 CNT = TIM4->CNT;
    float row = 0;

    if ((TIM4->CR1 >> 4 & 0x01) == 0)
    { 
        if (CNT < lastCNT)
            row = 2000 - lastCNT + CNT;
        else
            row = CNT - lastCNT;
    }
    else if ((TIM4->CR1 >> 4 & 0x01) == 1)
    {
        if (CNT > lastCNT)
            row = 2000 - CNT + lastCNT;
        else
            row = lastCNT - CNT;
    }
    *speed = row * 2;
    lastCNT = CNT;
}

void PID_init(q15_t kp, q15_t ki, q15_t kd){
    arm_pid_reset_q15(&PID_type);
    PID_type.Kp = kp;
    PID_type.Ki = ki;
    PID_type.Kd = kd;
    arm_pid_init_q15(&PID_type, 1);
}

void PID_control(q15_t speed){
	q15_t out;     
	out = arm_pid_q15(&PID_type, speed - targetSpeed);
    u16 cur = curARR + out;
    if(cur > 2000)       curARR = 2000;
    else if(cur <= 0)    curARR = 0;
    else                 curARR = cur;
    TIM_SetCompare2(TIM9, curARR);
}
