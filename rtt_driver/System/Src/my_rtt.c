#include "my_rtt.h"

/* 定义线程控制块指针 */
static rt_thread_t pid_thread = RT_NULL;
static rt_thread_t gray_thread = RT_NULL;

static void pid_thread_entry(void *parameter);
static void gray_thread_entry(void *parameter);

int k_value;
int error_value;

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
    // uint8_t key_status;
    motor_forward();
    while (1)
    {
        motor_control(k_value, error_value);
        rt_thread_delay(10);
        //     key_status = key_scan();
        //       if (key_status)
        //	   {
        //           motor_control(k_value, error_value);
        //           motor_forward();
        //       }
        //       else
        //           motor_stop();
    }
}

static void gray_thread_entry(void *parameter)
{
    while (1)
    {
        gray_bias(&k_value, &error_value);
        rt_thread_delay(10);
    }
}
