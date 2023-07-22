#include "control.h"

PID_SAVE pid_save = {

    // 位置环
    {0.1, 0.00001, 2, 0, 0, 0, 0},
    {0.1, 0.00001, 2, 0, 0, 0, 0},
    // 速度环
    {6, 0.01, 0, 0, 0, 0, 0},
    {6, 0.01, 0, 0, 0, 0, 0},
    // 增量环
    {2, 0.5, 0, 0, 0, 0, 0},
    {2, 0.5, 0, 0, 0, 0, 0},
};

CONTROL_SAVE con_save = {

    // 速度控制器
    {0, 0, 0, 0, 0, 0, 0, 0},
    // 位置控制器
    {0, 0, 0, 0, 0, 0, 0, 0},
};

float error_cnt[10] = {50, 40, 30, 20, 10, 10, 20, 30, 40, 50};
uint8_t gray_value;
uint8_t gray_bit[8];
uint8_t index_A;
uint8_t index_B;
/*
 *灰度传感器偏差
 */
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

/*
 *CCD偏差
 */
void ccd_bias(float *A_BIAS, float *B_BIAS, CCD_TYPE *ccd)
{
    /*待完善*/
}

/*
 *PID速度控制器
 *Excpect_A_CNT: A电机目标速度
 *Excpect_B_CNT: B电机预期速度
 *  *controller：控制环参数结构体指针
 */
void speed_controller(float Excpect_A_CNT, float Excpect_B_CNT, CONTROL_TYPE *controller)
{
    controller->A_CNT = get_timer_cnt(TIM3);
    controller->B_CNT = get_timer_cnt(TIM2);
    controller->A_PWM += pid_increment(controller->A_CNT, Excpect_A_CNT, pid_save.a_increment);
    controller->B_PWM += pid_increment(controller->B_CNT, Excpect_B_CNT, pid_save.b_increment);

    load_pwm(controller->A_PWM, controller->B_PWM);
}

/*
 *PID位置控制器
 *轮子周长 = 20.42cm (2*PI*R)
 *一圈CNT = 1300
 *distance_cm：距离值
 **controller：控制环参数结构体指针
 */
void location_controller(uint16_t distance_cm, CONTROL_TYPE *controller)
{
    float motor_cnt;
    motor_cnt = distance_cm * 62.64;

    controller->A_CNT = get_timer_cnt(TIM3);
    controller->B_CNT = get_timer_cnt(TIM2);
    if (controller->A_CNT > 0x7FFF)
        controller->A_CNT -= 0x10000;
    if (controller->B_CNT > 0x7FFF)
        controller->B_CNT -= 0x10000;

    controller->A_ACC_CNT += controller->A_CNT;
    controller->B_ACC_CNT += controller->B_CNT;

    controller->A_EXPECT_CNT = pid_location(controller->A_ACC_CNT, motor_cnt, pid_save.a_location);
    controller->B_EXPECT_CNT = pid_location(controller->B_ACC_CNT, motor_cnt, pid_save.b_location);
    controller->A_PWM = pid_speed(controller->A_CNT, controller->A_EXPECT_CNT, pid_save.a_speed);
    controller->B_PWM = pid_speed(controller->B_CNT, controller->B_EXPECT_CNT, pid_save.b_speed);

    load_pwm(controller->A_PWM, controller->B_PWM);
}
