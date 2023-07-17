#include "pid.h"

/*
 *k:       偏差系数
 *_pid:    PID参数结构
 */
int pid_location(int k, PID_TYPE _pid)
{
    _pid.bias = k;
    _pid.integral_bias += _pid.bias;

    _pid.pwm = _pid.kp * _pid.bias +
               _pid.ki * _pid.integral_bias +
               _pid.kd * (_pid.bias - _pid.last_bias);

    _pid.last_bias = _pid.bias;

    return _pid.pwm;
}

/*
 *k:       偏差系数
 *_pid:    PID参数结构
 */
int pid_basic(int k, PID_TYPE _pid)
{
    _pid.bias = k;
    _pid.integral_bias += _pid.bias;

    _pid.pwm = 4000 - _pid.kp * _pid.bias +
               _pid.kd * (_pid.bias - _pid.last_bias);

    _pid.last_bias = _pid.bias;

    return _pid.pwm;
}
