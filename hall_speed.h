#ifndef __HALL_SPEED_H
#define __HALL_SPEED_H

#include "stm32f10x.h"

void HallSpeed_Init(void);     // ��ʼ����������ģ��
uint16_t HallSpeed_GetRPM(void); // ��ȡ��ǰת�٣���λ RPM��
float HallSpeed_GetSpeed(void);  // ��ȡ���ٶȣ���λ m/s��

#endif
