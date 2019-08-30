#include"serial_key.h"
#include"stm32f4xx_usart.h"


ACTION_FUN action_list[16];
void SE_KeyMap_init(){
    
    for(int i = 0; i < 16; i++)
        action_list[i] = VOID_FUN;

    GPIO_InitTypeDef ioB;
    ioB.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    ioB.GPIO_Mode = GPIO_Mode_AF;
    ioB.GPIO_OType = GPIO_OType_PP;
    ioB.GPIO_PuPd = GPIO_PuPd_UP;
    ioB.GPIO_Speed = GPIO_Speed_50MHz;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_Init(GPIOB, &ioB);

    GPIO_PinAFConfig(GPIOB, EXTI_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, EXTI_PinSource11, GPIO_AF_USART3);
    USART_InitTypeDef Usart3_init;
    Usart3_init.USART_BaudRate = 115200;
    Usart3_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    Usart3_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    Usart3_init.USART_Parity = USART_Parity_No;
    Usart3_init.USART_StopBits = USART_StopBits_1;
    Usart3_init.USART_WordLength = USART_WordLength_8b;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    USART_Init(USART3, &Usart3_init);
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef USART3_NvicInit;
    USART3_NvicInit.NVIC_IRQChannel = USART3_IRQn;                       //中断频道
    USART3_NvicInit.NVIC_IRQChannelCmd = ENABLE;
    USART3_NvicInit.NVIC_IRQChannelPreemptionPriority = 1;     //抢占优先级，主优先级
    USART3_NvicInit.NVIC_IRQChannelSubPriority = 1;            //提交优先级，次优先级
    NVIC_Init(&USART3_NvicInit);
    
    USART_Cmd(USART3, ENABLE);
}

void USART3_IRQHandler(){
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET){
        u8 resdat = USART_ReceiveData(USART3);
        if(resdat <= 0x10 && resdat >= 0x00)
            action_list[resdat - 1]();
    }
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);
}

void SE_KeyMap_setFun(ACTION_FUN fun, u8 key_number){
    action_list[key_number - 1] = fun;
}

void VOID_FUN(){
}
