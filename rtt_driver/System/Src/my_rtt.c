#include "my_rtt.h"

/* 定义线程控制块指针 */
static rt_thread_t controller_thread = RT_NULL;
static rt_thread_t trace_thread = RT_NULL;

static void controller_thread_entry(void *parameter);
static void trace_thread_entry(void *parameter);

/* 变量声明 */
extern CONTROL_SAVE con_save;
extern CCD_TYPE ccd;

float a_bias;
float b_bias;

int my_rtt_init(void)
{

    controller_thread = rt_thread_create("controller",
                                         controller_thread_entry,
                                         RT_NULL,
                                         512,
                                         3,
                                         20);
    if (controller_thread != RT_NULL)
        rt_thread_startup(controller_thread);
    else
        return -1;

    trace_thread = rt_thread_create("trace",
                                    trace_thread_entry,
                                    RT_NULL,
                                    512,
                                    3,
                                    20);
    if (trace_thread != RT_NULL)
        rt_thread_startup(trace_thread);
    else
        return -1;

    return 0;
}

static void controller_thread_entry(void *parameter)
{
    motor_forward();

    while (1)
    {
        bias_controller(a_bias, b_bias, &con_save.bias_control);

        rt_thread_delay(10);
    }
}

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
