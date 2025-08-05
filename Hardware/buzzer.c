#include "buzzer.h"

void Buzzer_Init(void)
{
    // 1. ����GPIO��TIM2��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 2. ����PA2Ϊ�������������TIM2_CH3��
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ��ʱ����������
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 4499*2;               // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 0;               // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 4. PWM ���ã�TIM2_CH3 - PA2
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 2249*2; // ռ�ձ� 50%
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // 5. ʹ�ܶ�ʱ��
    TIM_Cmd(TIM2, ENABLE);
}

// ������������PWM�����
void Buzzer_On(void)
{
    TIM_CtrlPWMOutputs(TIM2, ENABLE);  // �߼���ʱ���ã���ͨ��ʱ����ʡ��
}

// �رշ�����������͵�ƽ��
void Buzzer_Off(void)
{
    TIM_SetCompare3(TIM2, 0);  // ���ռ�ձ���Ϊ0%
}
