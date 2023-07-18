# rt_thread driver
---

一个循迹小车的模板，适用于白底黑线、白底红线。

## 使用芯片
STM32F103C8T6

## 更新内容
* 增加较为稳定的速度环PID。

__control.c__
```
int pid_increment(float CNT, float expect_cnt, PID_TYPE pid)
{
    pid.bias = expect_cnt - CNT;
    pid.pwm += pid.kp * (pid.bias - pid.last_bias) +
               pid.ki * pid.bias;
    return pid.pwm;
}
```
* 位置环PID（待测试）
  
__control.c__
```
int pid_position(int cnt, int target, PID_TYPE _pid)
{
    _pid.bias = cnt - target;
    _pid.integral_bias += _pid.bias;
    _pid.pwm += _pid.kp * (_pid.bias - _pid.last_bias) +
                _pid.ki * _pid.bias;
    _pid.last_bias = _pid.bias;
    return _pid.pwm;
}
```
* 灰度探头固定误差值，设定为目标CNT值
__control.c__
```
error_cnt[10] = {50, 40, 30, 20, 10, 10, 20, 30, 40, 50};
```
可自行调整数值

## 功能
本项目使用自适应灰度传感器的反馈，rt-thread移植，附加反馈计算控制车身实现循迹，参数不固定，由电机情况而决定。

## 器件详情
* 电机 ：JGB37-520编码电机 12V 333rad/min
* 电机驱动：TB6612FNG
* 灰度传感器：感为智能 八路灰度传感器绿光款 带IIC 淘宝连接： [八路灰度传感器](https://item.taobao.com/item.htm?spm=a21n57.1.0.0.1103523crdpRAx&id=700000730878&ns=1&abbucket=0#detail)

>keil 编译版本：5.24.2.0


