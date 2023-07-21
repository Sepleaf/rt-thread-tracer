#include "control.h"

PID_SAVE pid_save =
    {
        // 位置环
        {0.1, 0.05, 0, 0, 0, 0, 0, 0},
        {0.1, 0.05, 0, 0, 0, 0, 0, 0},
        // 增量环
        {2, 0.5, 0, 0, 0, 0, 0, 0},
        {2, 0.5, 0, 0, 0, 0, 0, 0},
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
 *Excpect_A_CNT: A电机预期速度 CNT/10ms
 *Excpect_B_CNT: B电机预期速度 CNT/10ms
 */
void speed_controller(float Excpect_A_CNT, float Excpect_B_CNT)
{
    // A_CNT = get_timer_cnt(TIM3);
    // B_CNT = get_timer_cnt(TIM2);

    A_PWM += pid_increment(A_CNT, Excpect_A_CNT, pid_save.A_increment);
    B_PWM += pid_increment(B_CNT, Excpect_B_CNT, pid_save.B_increment);

    load_pwm(A_PWM, B_PWM);
}

float motor_cnt;
uint16_t a_acc_cnt = 0;
uint16_t b_acc_cnt = 0;
int16_t a_expect_speed;
int16_t b_expect_speed;
/*
 *位置PID控制器
 *distance_cm： 目标距离
 *轮子周长 = 20.42cm
 *一圈CNT = 1300
 */
void location_controller(uint16_t distance_cm)
{
    motor_cnt = distance_cm * 62.66; // 63.66 CNT/cm
    A_CNT = get_timer_cnt(TIM3);
    B_CNT = get_timer_cnt(TIM2);
    a_acc_cnt += A_CNT;
    b_acc_cnt += B_CNT;

    a_expect_speed = pid_location(a_acc_cnt, motor_cnt, pid_save.A_location);
    b_expect_speed = pid_location(b_acc_cnt, motor_cnt, pid_save.B_location);

    A_PWM = (a_expect_speed + b_expect_speed) / 2;

    speed_controller(A_PWM, A_PWM);
}
