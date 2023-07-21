#include "pid.h"

/*
 *位置环PID
 *cnt:    编码器计数
 *target: 目标编码数
 *_pid:   PID参数结构体
 */
float pid_location(int actual_location, int expect_location, PID_TYPE pid)
{
    pid.bias = expect_location - actual_location;

    pid.output = pid.kp * (pid.bias - pid.last_bias) +
                 pid.ki * pid.bias +
                 pid.kd * (pid.bias - 2 * pid.last_bias + pid.last_last_bias);

    pid.last_last_bias = pid.last_bias;
    pid.last_bias = pid.bias;

    return pid.output;
}

/*
 *增量环PID(用于调速)
 *CNT：          编码器计数
 *expect_cnt：   目标编码器数
 *pid：          PID参数结构体
 */
float pid_increment(float actual_cnt, float expect_cnt, PID_TYPE pid)
{
    pid.bias = expect_cnt - actual_cnt;

    pid.output += pid.kp * (pid.bias - pid.last_bias) +
                  pid.ki * pid.bias;

    pid.last_bias = pid.bias;

    return pid.output;
}
