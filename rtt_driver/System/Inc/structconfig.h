#ifndef __STRUCTCONFIG_H
#define __STRUCTCONFIG_H
#include "stm32f10x.h"

/*PID参数*/
typedef struct
{
    float kp;
    float ki;
    float kd;

    float bias;
    float last_bias;
    float integral_bias;
    float output;
} PID_TYPE;

typedef struct
{
    // 位置环
    PID_TYPE a_location;
    PID_TYPE b_location;
    // 速度环
    PID_TYPE a_speed;
    PID_TYPE b_speed;
    // 偏差环
    PID_TYPE a_bias;
    PID_TYPE b_bias;
    // 增量环
    PID_TYPE a_increment;
    PID_TYPE b_increment;
} PID_SAVE;

// CCD结构体
typedef struct
{
    uint8_t times;

    uint8_t right_acq;
    uint8_t left_acq;
    uint8_t left_value;
    uint8_t right_value;

    uint8_t median;
    uint8_t threshold;

    uint16_t value_max;
    uint16_t value_min;
} CCD_TYPE;

/*控制器参数*/
typedef struct
{
    float A_CNT;
    float B_CNT;

    float A_ACC_CNT;
    float B_ACC_CNT;

    float A_EXPECT_CNT;
    float B_EXPECT_CNT;

    uint16_t A_PWM;
    uint16_t B_PWM;

} CONTROL_TYPE;

typedef struct
{
    // 速度控制器
    CONTROL_TYPE speed_control;
    // 位置控制器
    CONTROL_TYPE location_control;
    // 偏差控制器
    CONTROL_TYPE bias_control;
} CONTROL_SAVE;

#endif
