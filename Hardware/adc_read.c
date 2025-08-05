#include "adc_read.h"

#define ADC_CHANNEL      ADC_Channel_1    // ��Ӧ PA1
#define ADC_GPIO_PIN     GPIO_Pin_1
#define ADC_GPIO_PORT    GPIOA
#define VREF             3.3f             // �ο���ѹ
#define ADC_SAMPLE_TIME  ADC_SampleTime_55Cycles5

void ADC_Read_Init(void)
{
    // 1. ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    // 2. ���� PA1 Ϊģ������
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = ADC_GPIO_PIN;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(ADC_GPIO_PORT, &gpio);

    // 3. ��ʼ�� ADC
    ADC_InitTypeDef adc;
    ADC_DeInit(ADC1);
    adc.ADC_Mode = ADC_Mode_Independent;
    adc.ADC_ScanConvMode = DISABLE;
    adc.ADC_ContinuousConvMode = DISABLE;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &adc);

    // 4. ����ͨ��
    ADC_RegularChannelConfig(ADC1, ADC_CHANNEL, 1, ADC_SAMPLE_TIME);

    // 5. ���� ADC
    ADC_Cmd(ADC1, ENABLE);

    // 6. У׼
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

// ����ԭʼ ADC ��ֵ��0~4095��
uint16_t ADC_Read_Value(void)
{
    ADC_RegularChannelConfig(ADC1, ADC_CHANNEL, 1, ADC_SAMPLE_TIME);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}

// ����ת�������Ե�ѹֵ��0~1��
float ADC_Read_Voltage(void)
{
    uint16_t value = ADC_Read_Value();
    return (value) / 4095.0f;
}
