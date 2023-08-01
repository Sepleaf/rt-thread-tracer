#include "pid.h"

/*
 *位置环PID
 *actual_location：  实际位置
 *expect_location：  目标位置
 *            pid:   PID参数结构体
 *         retval：  PID计算结果
 */
float pid_location(float actual_location, float expect_location, PID_TYPE pid)
{
    pid.bias = expect_location - actual_location;
    pid.integral_bias += pid.bias;
    // 积分限幅
    if (pid.integral_bias > 20000)
        pid.integral_bias = 20000;
    if (pid.integral_bias < -20000)
        pid.integral_bias = -20000;

    pid.output = pid.kp * pid.bias +
                 pid.ki * pid.integral_bias +
                 pid.kd * (pid.bias - pid.last_bias);

    pid.last_bias = pid.bias;
    // 输出限幅
    if (pid.output > 6000)
        pid.output = 6000;
    if (pid.output < 0)
        pid.output = 0;

    return pid.output;
}

/*
 *速度环PID
 *actual_speed：  实际速度
 *expect_speed：  目标速度
 *         pid:   PID参数结构体
 *      retval：  PID计算结果
 */
float pid_speed(float actual_speed, float expect_speed, PID_TYPE pid)
{

    pid.bias = expect_speed - actual_speed;
    if (pid.bias == 0)
        pid.integral_bias = 0;

    pid.integral_bias += pid.bias;
    // 积分限幅
    if (pid.integral_bias > 10000)
        pid.integral_bias = 10000;
    if (pid.integral_bias < -10000)
        pid.integral_bias = -10000;

    pid.output = pid.kp * pid.bias +
                 pid.ki * pid.integral_bias;
    // 输出限幅
    if (pid.output > 6000)
        pid.output = 6000;
    if (pid.output < 0)
        pid.output = 0;

    return pid.output;
}

/*
 *增量环PID(可用于调速)
 *actual_cnt：   编码器计数
 *expect_cnt：   目标编码器数
 *       pid：   PID参数结构体
 *    retval：  PID计算结果
 */
float pid_increment(float actual_cnt, float expect_cnt, PID_TYPE pid)
{
    pid.bias = expect_cnt - actual_cnt;

    pid.output += pid.kp * (pid.bias - pid.last_bias) +
                  pid.ki * pid.bias;

    pid.last_bias = pid.bias;

    return pid.output;
}
