
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "CAR.h"
#include "Serial.h"
#include "Servo.h"
#include "Ultrasound.h"
#include "hall_speed.h"
#include "usart1.h"
#include "Motor.h"
#include "speed.h"
#include "Irtracking.h"
#include "bizhang.h"
#include <stdio.h>

char disp_buf[20];

// ȫ�ֺ���������� SysTick_Handler �ۼ�
volatile uint32_t msTicks = 0;

void Delay(uint32_t t) { while(t--); }

int main(void)
{
    uint32_t sec = 0;            // ��������0~99��
    uint32_t lastUpdate = 0;     // �ϴθ���ʱ���
    uint8_t timing_enabled = 0;  // ��ʱʹ�ܱ�־
    uint8_t timing_last = 0;     // ��һ�μ�ʱʹ�ܱ�־

    int lap_count = 0;           // Ȧ��
    uint8_t last_at_mark = 0;    // ��һʱ���Ƿ��ڼ�����

    uint8_t stopped = 0;         // �Ƿ��Ѿ����յ�ͣ��

    // ��ʼ��ģ��
    Motor_Init();
    OLED_Init();
    OLED_Clear();
    Speed_Init();
    Speed_TIM2_Init();
    Irtracking_Init();
    bizhang_Init();
    SysTick_Config(SystemCoreClock / 1000);

    // ��ʼʱ����ʾ
    OLED_ShowString(1, 1, "time:");
    OLED_ShowNum(1, 6, 0, 2);
    OLED_ShowChar(1, 8, 's');

    while (1)
    {
        // ========== 1. �ϰ��������� ==========
        if (bizhang_get() == 0)
        {
            Car_Stop();
            timing_enabled = 0;
            OLED_ShowString(4, 1, "Obstacle     ");
            Buzzer_Init();
            Buzzer_On();
        }
        // ========== 2. �����2Ȧ ==========
        else if (stopped)
        {
            Car_Stop();
            timing_enabled = 0;
            OLED_ShowString(4, 1, "Finish 2 laps");
            Buzzer_Off();
            // ��ѭ������С�������ٱ�����
            while(1)
            {
                
            }
        }
        // ========== 3. ����ѭ�� & ��Ȧ ==========
        else
        {
            Buzzer_Off();

            // --- ��Ȧ�߼� ---
            uint8_t left = Left_Irtracking_Get();
            uint8_t right = Right_Irtracking_Get();
            uint8_t at_mark = (left == 1 && right == 1) ? 1 : 0;
            if (!last_at_mark && at_mark)
						{
							 lap_count++;
							 
							
						}
            last_at_mark = at_mark;

            if (lap_count >= 2)
            {
                stopped = 1;
                Car_Stop();
                timing_enabled = 0;
                OLED_ShowString(4, 1, "Finish 2 laps");
                Buzzer_Off();
                continue;
            }

            // Ѱ������
            if (left == 0 && right == 0)
            {
                Car_Forward();
                timing_enabled = 1;
                OLED_ShowString(4, 1, "Running");
            }
            else if (left == 0 && right == 1)
            {
                Car_Left();
                timing_enabled = 1;
                OLED_ShowString(4, 1, "L-slow-down");
            }
            else if (left == 1 && right == 0)
            {
                Car_Right();
                timing_enabled = 1;
                OLED_ShowString(4, 1, "R-slow-down");
            }
            else
            {
                Car_Forward();
                timing_enabled = 1;
                OLED_ShowString(4, 1, "On Mark      ");
            }
        }

        // ---- ��ʱ ----
        if (timing_enabled && !timing_last)
            lastUpdate = msTicks;
        timing_last = timing_enabled;

        if (timing_enabled && (msTicks - lastUpdate) >= 1000)
        {
            lastUpdate += 1000;
            if (++sec > 99) sec = 0;
            OLED_ShowString(1, 1, "time:");
            OLED_ShowNum(1, 6, sec, 2);
            OLED_ShowChar(1, 8, 's');
        }

        // ---- OLED�ٶ�/���� ----
        sprintf(disp_buf, "speed: %.2f", speed_rpm * 1.0f / 60 * 3.1415f * 0.07f);
        OLED_ShowString(2, 1, disp_buf);
        sprintf(disp_buf, "D: %.2f m ", total_distance_m);
        OLED_ShowString(3, 1, disp_buf);

        // ��ѡ��ʵʱ��ʾȦ��
        // OLED_ShowString(4, 13, "Lap:");
        // OLED_ShowNum(4, 17, lap_count, 1);

				Delay(50000);
    }
}
