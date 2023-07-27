#include "control.h"

/*PID 参数初始化*/
PID_SAVE pid_save = {

    // 位置环
    {0.1, 0.00001, 2, 0, 0, 0, 0},
    {0.1, 0.00001, 2, 0, 0, 0, 0},
    // 速度环
    {5, 0.01, 0, 0, 0, 0, 0},
    {5, 0.01, 0, 0, 0, 0, 0},
    // 偏差环
    {70, 0.1, 3, 0, 0, 0, 0},
    {70, 0.1, 3, 0, 0, 0, 0},
    // 增量环
    {1.7, 0.01, 0, 0, 0, 0, 0},
    {1.7, 0.01, 0, 0, 0, 0, 0},
};

/*控制器参数初始化*/
CONTROL_SAVE con_save = {

    // 速度控制器
    {0, 0, 0, 0, 0, 0, 0, 0},
    // 位置控制器
    {0, 0, 0, 0, 0, 0, 0, 0},
    // 偏差控制器
    {0, 0, 0, 0, 0, 0, 0, 0},
};

/*
 * PID速度控制器
 * Excpect_A_CNT: A电机目标速度
 * Excpect_B_CNT: B电机预期速度
 *   *controller：控制器参数结构体指针
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
 * PID位置控制器
 * 轮子周长 = 20.42cm (2*PI*R)
 * 一圈CNT = 1300
 * distance_cm：距离值
 * *controller：控制器参数结构体指针
 */
void location_controller(uint16_t distance_cm, CONTROL_TYPE *controller)
{
    float motor_cnt;
    motor_cnt = distance_cm * 62.64;

    controller->A_CNT = get_timer_cnt(TIM3);
    controller->B_CNT = get_timer_cnt(TIM2);
    // 反转纠正
    if (controller->A_CNT > 0x7FFF)
        controller->A_CNT -= 0x10000;
    if (controller->B_CNT > 0x7FFF)
        controller->B_CNT -= 0x10000;
    // CNT累加
    controller->A_ACC_CNT += controller->A_CNT;
    controller->B_ACC_CNT += controller->B_CNT;
    // 位置环->速度环 PID计算
    controller->A_EXPECT_CNT = pid_location(controller->A_ACC_CNT, motor_cnt, pid_save.a_location);
    controller->B_EXPECT_CNT = pid_location(controller->B_ACC_CNT, motor_cnt, pid_save.b_location);
    controller->A_PWM = pid_speed(controller->A_CNT, controller->A_EXPECT_CNT, pid_save.a_speed);
    controller->B_PWM = pid_speed(controller->B_CNT, controller->B_EXPECT_CNT, pid_save.b_speed);

    load_pwm(controller->A_PWM, controller->B_PWM);
}

float a_bias_speed;
float b_bias_speed;
/*
 * 偏差控制器
 * a_bias：a侧偏差值
 * b_bias：b侧偏差值
 * *controller：控制器参数结构体指针
 */
void bias_controller(float a_bias, float b_bias, CONTROL_TYPE *controller)
{
    // 误差累积
    controller->A_CNT = get_timer_cnt(TIM3);
    controller->B_CNT = get_timer_cnt(TIM2);
    // 反转纠正
    if (controller->A_CNT > 0x7FFF)
        controller->A_CNT -= 0x10000;
    if (controller->B_CNT > 0x7FFF)
        controller->B_CNT -= 0x10000;

    //  偏差PID 与 增量PID 通用
    controller->A_EXPECT_CNT = pid_increment(a_bias, 0, pid_save.a_bias);
    controller->B_EXPECT_CNT = pid_increment(b_bias, 0, pid_save.b_bias);

    a_bias_speed = pid_speed(a_bias, 0, pid_save.a_speed);
    b_bias_speed = pid_speed(b_bias, 0, pid_save.b_speed);

    controller->A_PWM = pid_increment(controller->A_CNT, controller->A_EXPECT_CNT + a_bias_speed + 1000, pid_save.a_increment);
    controller->B_PWM = pid_increment(controller->B_CNT, controller->B_EXPECT_CNT + b_bias_speed + 1000, pid_save.b_increment);

    load_pwm(controller->A_PWM, controller->B_PWM);
}
