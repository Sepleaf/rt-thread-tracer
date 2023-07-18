#include "pid.h"

/*
 *位置环PID
 *cnt:    编码器计数
 *target: 目标编码数
 *_pid:   PID参数结构体
 *return: PWM
 */
int pid_position(int cnt, int target, PID_TYPE _pid)
{
    _pid.bias = cnt - target;
    _pid.integral_bias += _pid.bias;

    _pid.pwm += _pid.kp * (_pid.bias - _pid.last_bias) +
                _pid.ki * _pid.bias;
    _pid.last_bias = _pid.bias;

    return _pid.pwm;
}

/*
 *速度环PID
 *CNT：          编码器计数
 *expect_cnt：   目标编码器数
 *pid：          PID参数结构体
 */
int pid_increment(float CNT, float expect_cnt, PID_TYPE pid)
{
    pid.bias = expect_cnt - CNT;
    pid.pwm += pid.kp * (pid.bias - pid.last_bias) +
               pid.ki * pid.bias;
    pid.last_bias = pid.bias;
    return pid.pwm;
}
