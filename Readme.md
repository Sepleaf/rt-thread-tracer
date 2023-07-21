# rt_thread driver
---

一个循迹小车的模板，适用于白底黑线、白底红线。

## 使用芯片
STM32F103C8T6

## 更新内容
* 增加较为稳定的速度环PID。

__pid.c__
```
float pid_increment(float actual_cnt, float expect_cnt, PID_TYPE pid)
{
    pid.bias = expect_cnt - actual_cnt;

    pid.output += pid.kp * (pid.bias - pid.last_bias) +
                  pid.ki * pid.bias;

    pid.last_bias = pid.bias;

    return pid.output;
}
```
* 位置环PID`测试通过`
  
__pid.c__
```
int pid_location(int actual_location, int expect_location, PID_TYPE pid)
{
    pid.bias = expect_location - actual_location;
    pid.integral_bias += pid.bias;
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
```
>通过调整位置PID的参数增加稳定性,可精确控制至厘米级

* 位置环将距离值（cm）转换为脉冲数CNT

__control.c__
```
void location_controller(uint16_t distance_cm)
{
    motor_cnt = distance_cm * 62.64;

    A_CNT = get_timer_cnt(TIM3);
    B_CNT = get_timer_cnt(TIM2);
    if (A_CNT > 0x7FFF)
        A_CNT -= 0x10000;
    if (B_CNT > 0x7FFF)
        B_CNT -= 0x10000;

    a_acc_cnt += A_CNT;
    b_acc_cnt += B_CNT;

    a_expect_cnt = pid_location(a_acc_cnt, motor_cnt, pid_save.a_location);
    b_expect_cnt = pid_location(b_acc_cnt, motor_cnt, pid_save.b_location);
    A_PWM = pid_speed(A_CNT, a_expect_cnt, pid_save.a_speed);
    B_PWM = pid_speed(B_CNT, b_expect_cnt, pid_save.b_speed);

    load_pwm(A_PWM, B_PWM);
}
```
>周长C = 2* PI * R,一圈的脉冲数是1300,计算可得每cm的脉冲数。根据具体情况自行调整参数

* 灰度循迹

  __待更新__

## 功能
本项目使用自适应灰度传感器的反馈，rt-thread移植，附加反馈计算控制车身实现循迹，参数不固定，由电机情况而决定。

## 器件详情
* 电机 ：JGB37-520编码电机 12V 333rad/min
* 电机驱动：TB6612FNG
* 灰度传感器：感为智能 八路灰度传感器绿光款 带IIC 淘宝连接： [八路灰度传感器](https://item.taobao.com/item.htm?spm=a21n57.1.0.0.1103523crdpRAx&id=700000730878&ns=1&abbucket=0#detail)

>keil 编译版本：5.24.2.0
