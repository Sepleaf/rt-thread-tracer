#include "my_rtt.h"

/* 定义线程控制块指针 */
static rt_thread_t pid_thread = RT_NULL;
static rt_thread_t gray_thread = RT_NULL;

static void pid_thread_entry(void *parameter);
static void gray_thread_entry(void *parameter);

/*变量声明*/
extern CONTROL_SAVE con_save;
extern CCD_TYPE ccd;

int my_rtt_init(void)
{

    pid_thread = rt_thread_create("pid_control",
                                  pid_thread_entry,
                                  RT_NULL,
                                  512,
                                  3,
                                  20);
    if (pid_thread != RT_NULL)
        rt_thread_startup(pid_thread);
    else
        return -1;

    gray_thread = rt_thread_create("gray_scan",
                                   gray_thread_entry,
                                   RT_NULL,
                                   512,
                                   3,
                                   20);
    if (gray_thread != RT_NULL)
        rt_thread_startup(gray_thread);
    else
        return -1;

    return 0;
}

static void pid_thread_entry(void *parameter)
{
    float a_bias;
    float b_bias;

    motor_forward();

    while (1)
    {
        ccd_bias(&a_bias, &b_bias);
        bias_controller(a_bias, b_bias, &con_save.bias_control);

        rt_thread_delay(10);
    }
}

static void gray_thread_entry(void *parameter)
{
    while (1)
    {
        data_acquisition();
        OLED_ShowNum(1, 1, ccd.median, 3);

        rt_thread_delay(10);
    }
}
