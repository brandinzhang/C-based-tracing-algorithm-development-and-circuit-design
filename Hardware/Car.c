#include "car.h"
#include "motor.h"
#define SPEED_HIGH 70
#define SPEED_LOW   50

void Car_Forward(void)
{
    Motor_SetSpeed(70,70);
}

void Car_Left(void)
{
    // �����������ֿ죬ʵ����ת
    Motor_SetSpeed(SPEED_LOW, SPEED_HIGH);
}

void Car_Right(void)
{
    // �����������ֿ죬ʵ����ת
    Motor_SetSpeed(SPEED_HIGH, SPEED_LOW);
}

void Car_Stop(void)
{
    Motor_SetSpeed(0, 0);
}
