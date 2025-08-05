#include "hall_speed.h"
#include "stm32f10x.h" 
#define WHEEL_DIAMETER_M   0.065f // ����ֱ������λm

volatile uint16_t pulse_count = 0;
volatile uint16_t speed_rpm = 0;

// ��ʼ��GPIO+EXTI+TIM2
void HallSpeed_Init(void)
{
    // 1. ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. ����PA0Ϊ��������
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. �ⲿ�ж�����
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 4. NVIC�ж����ȼ�
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. ����TIM2��1ms��ʱ��1s�ж�һ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;   // 72MHz / 7200 = 10kHz
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;     // ÿ1�����
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, ENABLE);
}

// ��ȡ��ǰRPM
uint16_t HallSpeed_GetRPM(void)
{
    return speed_rpm;
}

// ��ȡ���ٶ� (m/s)
float HallSpeed_GetSpeed(void)
{
    float circ = 3.1416f * WHEEL_DIAMETER_M;
    return (circ * speed_rpm) / 60.0f;
}
