# rt_thread driver
---

一个循迹小车的模板,移植的RTOS为RT-Thread

## 使用芯片
STM32F103C8T6

## 更新内容
* 增加较为稳定的速度环PID。
* 增加稳定的位置环PID
* CCD寻迹
* 灰度传感器寻迹
  >由于结构体使用较多所以放同一个文件里了



## 说明
* `control.c` 位置控制器（location_controller）在实际循迹中未使用，仅用于测试。
* TSL1401线性CCD易受环境光照影响，可加外置照明灯或调整曝光时间：

__ccd.c__
```  
 CCD_TYPE ccd={
    100, 0, 0, 0, 0, 0, 0, 0, 0
};
```
默认为100us，可适度调节。
  
* 初始化

__board.c__
```
void rt_hw_board_init()
{
    /* System Clock Update */
    SystemCoreClockUpdate();

    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /*此处为各部分功能的初始化*/
    motor_pwm_init(7200 - 1, 1 - 1);
    gray_iic_init();
    encoder_init();
    ccd_init();
    beep_init();
    oled_init();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}
```

* 增量PID `可用于调速`

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
* 位置环PID `测试通过`
  
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


* 灰度循迹

 偏差值根据实际情况适当更改即可

__gray.c__
```
bias_list[8] = {0, 1, 2, 5, 6, 8, 10, 12};
```

* 偏差控制器

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

    // A_EXPECT_CNT + 1000 为速度控制，基础速度为1000（误差为0的时候小车不停）
    controller->A_PWM = pid_increment(controller->A_CNT, controller->A_EXPECT_CNT + 1000, pid_save.a_increment);
    controller->B_PWM = pid_increment(controller->B_CNT, controller->B_EXPECT_CNT + 1000, pid_save.b_increment);

    load_pwm(controller->A_PWM, controller->B_PWM);
}
```
* RT-Thread线程
  
__my_rtt.c__
```
static void trace_thread_entry(void *parameter)
{
    while (1)
    {
        /* 灰度传感器循迹部分 */
        gray_bias(&a_bias, &b_bias);
        OLED_ShowNum(1, 1, a_bias, 2);
        OLED_ShowNum(1, 2, b_bias, 2);

        /* CCD循迹部分 */
        // ccd_bias(&a_bias, &b_bias);
        // data_acquisition();
        // OLED_ShowNum(1, 1, ccd.median, 3);

        rt_thread_delay(10);
    }
}
    
```


## 器件详情
* 电机 ：JGB37-520编码电机 12V 333rad/min
* 电机驱动：TB6612FNG
* 灰度传感器：感为智能 八路灰度传感器绿光款 带IIC 淘宝连接： [八路灰度传感器](https://item.taobao.com/item.htm?spm=a21n57.1.0.0.1103523crdpRAx&id=700000730878&ns=1&abbucket=0#detail)
* CCD：TSL1401线性CCD

>keil 编译版本：5.24.2.0


