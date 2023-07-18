#include "control.h"

PID_SAVE pid_save =
{
   {100, 100, 0, 0, 0, 0, 0},
   {5, 10, 0, 0, 0, 0, 0}
};

float error_cnt[10] = {50, 40, 30, 20, 10, 10, 20, 30, 40, 50};

uint8_t gray_value;
uint8_t gray_bit[8];
uint8_t index_A;
uint8_t index_B;

void gray_bias(float *A_BIAS, float *B_BIAS)
{
    uint8_t i;

    gray_value = get_gray_value();
    for (i = 0; i < 8; i++)
        gray_bit[i] = gray_value & (0x01 << i);
    for (i = 0; i < 8; i++)
    {
        index_A = 7 - i;
        index_B = i;
    }

    *A_BIAS = error_cnt[index_A];
    *B_BIAS = error_cnt[index_B];
}

uint16_t A_PWM = 0;
uint16_t B_PWM = 0;
uint16_t A_CNT;
uint16_t B_CNT;
/*
 *PID控制器
 */
void pid_controller(float Excpect_A_CNT, float Excpect_B_CNT)
{
    A_CNT = get_timer_cnt(TIM3);
    B_CNT = get_timer_cnt(TIM2);

    A_PWM += pid_increment(A_CNT, Excpect_A_CNT, pid_save.increment);
    B_PWM += pid_increment(B_CNT, Excpect_B_CNT, pid_save.increment);

    load_pwm(A_PWM, B_PWM);
}
