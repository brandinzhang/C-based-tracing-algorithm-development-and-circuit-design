#include "motor.h"

// ռ�ձȵȼ�����������ȫ�� = 100������ = 70��
#define SPEED_FULL     50
#define SPEED_TURN     30

// ��ǰ���ҵ���ٶȣ����ڸ��£�
static uint16_t left_speed = SPEED_FULL;
static uint16_t right_speed = SPEED_FULL;

void Motor_Init(void)
{
    // ����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ��ʼ��GPIO (PA6 - TIM3_CH1, PA7 - TIM3_CH2)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // ?? ��ԭ��д���� GPIO_InitStructuare����ƴд����

    // ���ö�ʱ��
    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;      // ARR = 100
    TIM_TimeBaseStructure.TIM_Prescaler = 36 - 1;    // PSC = 36
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // ��ʼ��PWM���
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  // ��ʼռ�ձ�Ϊ0

    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // PA6 - ����
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); // PA7 - �ҵ��

    TIM_Cmd(TIM3, ENABLE);
}

// �������ҵ��ռ�ձȣ�0~100��
void Motor_SetSpeed(uint16_t left, uint16_t right)
{
    if (left > 100) left = 100;
    if (right > 100) right = 100;

    left_speed = left;
    right_speed = right;

    TIM_SetCompare1(TIM3, left_speed);
    TIM_SetCompare2(TIM3, right_speed);
}
