# rt_thread driver
---

一个循迹小车的模板,移植的RTOS为RT-Thread

## 使用芯片
STM32F103C8T6

## 更新内容
* 增加较为稳定的速度环PID。
* 增加稳定的位置环PID
  >由于结构体使用较多所以放同一个文件里了

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

    controller->A_EXPECT_CNT = pid_location(controller->A_ACC_CNT, 1220, pid_save.a_location);
    controller->B_EXPECT_CNT = pid_location(controller->B_ACC_CNT, 1220, pid_save.b_location);
    controller->A_PWM = pid_speed(controller->A_CNT, controller->A_EXPECT_CNT, pid_save.a_speed);
    controller->B_PWM = pid_speed(controller->B_CNT, controller->B_EXPECT_CNT, pid_save.b_speed);

    load_pwm(controller->A_PWM, controller->B_PWM);
}
```
>周长C = 2* PI * R,一圈的脉冲数是1300,计算可得每cm的脉冲数。根据具体情况自行调整参数

* 灰度循迹

  __待更新__

* CCD循迹

__control.c__
```
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
    // CNT累加
    controller->A_ACC_CNT += controller->A_CNT;
    controller->B_ACC_CNT += controller->B_CNT;
    // 偏差PID 与 位置PID 通用
    controller->A_EXPECT_CNT = pid_location(a_bias, 0, pid_save.a_bias);
    controller->B_EXPECT_CNT = pid_location(b_bias, 0, pid_save.b_bias);

    controller->A_PWM = pid_increment(controller->A_CNT, controller->A_EXPECT_CNT + 1000, pid_save.a_increment);
    controller->B_PWM = pid_increment(controller->B_CNT, controller->B_EXPECT_CNT + 1000, pid_save.b_increment);

    load_pwm(controller->A_PWM, controller->B_PWM);
}
```
  

## 功能
本项目使用自适应灰度传感器的反馈，rt-thread移植，附加反馈计算控制车身实现循迹，参数不固定，由电机情况而决定。

## 器件详情
* 电机 ：JGB37-520编码电机 12V 333rad/min
* 电机驱动：TB6612FNG
* 灰度传感器：感为智能 八路灰度传感器绿光款 带IIC 淘宝连接： [八路灰度传感器](https://item.taobao.com/item.htm?spm=a21n57.1.0.0.1103523crdpRAx&id=700000730878&ns=1&abbucket=0#detail)

>keil 编译版本：5.24.2.0


