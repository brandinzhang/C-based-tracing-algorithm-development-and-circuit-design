/**
  ******************************************************************************
  * @file    stm32f10x_it.h
  * @author  �û�
  * @brief   ͷ�ļ��������жϴ�����������
  ******************************************************************************
  */

#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void EXTI0_IRQHandler(void);    // ������������PA0��
void TIM2_IRQHandler(void);     // ��ʱ�� TIM2������

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */
