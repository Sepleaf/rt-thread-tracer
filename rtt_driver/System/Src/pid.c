#include "pid.h"

/*
 *位置环PID
 *actual_location：  实际位置
 *expect_location：  目标位置
 *            pid:   PID参数结构体
 *         retval：  PID计算结果
 */
int pid_location(int actual_location, int expect_location, PID_TYPE pid)
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
    if (pid.output > 5000)
        pid.output = 5000;
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
int pid_speed(int actual_speed, int expect_speed, PID_TYPE pid)
{

    pid.bias = expect_speed - actual_speed;
    if (pid.bias == 0)
        pid.last_bias = 0;

    pid.last_bias += pid.bias;
    // 误差限幅
    if (pid.last_bias > 10000)
        pid.last_bias = 10000;
    if (pid.last_bias < -10000)
        pid.last_bias = -10000;

    pid.output = pid.kp * pid.bias +
                 pid.ki * pid.last_bias;

    if (pid.output > 5000)
        pid.output = 5000;
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
int pid_increment(int actual_cnt, int expect_cnt, PID_TYPE pid)
{
    pid.bias = expect_cnt - actual_cnt;

    pid.output += pid.kp * (pid.bias - pid.last_bias) +
                  pid.ki * pid.bias;

    pid.last_bias = pid.bias;

    return pid.output;
}
