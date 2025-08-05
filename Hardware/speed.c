#include "speed.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"


// === ���Ӳ��� ===
#define WHEEL_DIAMETER_M 0.065/2     // ���Ӱ뾶��7cm
#define PULSES_PER_ROUND 20          // ÿȦ������


// === ���峣�� ===
#define DEBOUNCE_MS 5                // ����ʱ�䣨���룩
#define TIMER_PERIOD_MS 10           // ��ʱ���ж�����


// === ȫ�ֱ��� ===
volatile uint32_t ms_ticks = 0;           // ϵͳ����ʱ���ʱ����λms��
volatile uint32_t pulse_count = 0;        // 1���ڼ�⵽��������
volatile uint16_t speed_rpm = 0;          // ��ǰת�٣���λRPM��
volatile float total_distance_m = 0.0f;   // �ۼ���ʻ���루��λ�ף�
volatile uint8_t car_moving = 0;          // ��־���Ƿ������˶�

static uint8_t last_PA0_level = 0;        // ��һ��PA0��ƽ
static uint32_t last_edge_tick = 0;       // ��һ����Ч����ʱ��

// === ��ȡ��ǰ����ʱ�� ===
uint32_t get_ms_ticks(void)
{
    return ms_ticks;
}

// === ��ʼ����ʱ��TIM2��ÿ10ms����һ���ж� ===
void Speed_TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 100 - 1;         // �Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;     // Ԥ��Ƶ 72MHz / 7200 = 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

// === ��ʼ���ٶ�ģ�� ===
void Speed_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ����PA0Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    Speed_TIM2_Init();

    // ��ʼ������
    last_PA0_level = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    last_edge_tick = 0;
    ms_ticks = 0;
    pulse_count = 0;
    speed_rpm = 0;
    total_distance_m = 0.0f;
    car_moving = 0;
}

// === TIM2�жϺ�����ÿ10ms����һ�� ===
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        ms_ticks += TIMER_PERIOD_MS;  // ϵͳʱ�����

        uint8_t current_level = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
        uint32_t now = get_ms_ticks();

        if (current_level != last_PA0_level)
        {
            if ((now - last_edge_tick) >= DEBOUNCE_MS)
            {
                pulse_count++;
                total_distance_m += (3.1415926f * WHEEL_DIAMETER_M) / PULSES_PER_ROUND;
                last_edge_tick = now;
            }
        }

        last_PA0_level = current_level;

        static uint16_t interrupt_count = 0;
        interrupt_count++;
        if (interrupt_count >= (1000 / TIMER_PERIOD_MS))  // 100�� * 10ms = 1s
        {
            speed_rpm = (pulse_count * 60) / PULSES_PER_ROUND;
            pulse_count = 0;
            interrupt_count = 0;
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

// === ��ȡ��ǰ�ٶȣ�km/h�� ===
float get_speed_kph(void)
{
    return (speed_rpm * 3.1415926f * WHEEL_DIAMETER_M * 60.0f) / 1000.0f;
}

// === ��ȡ����̣�km�� ===
float get_total_distance_km(void)
{
    return total_distance_m / 1000.0f;
}