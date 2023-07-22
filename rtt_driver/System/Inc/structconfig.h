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
    // 增量环
    PID_TYPE a_increment;
    PID_TYPE b_increment;
} PID_SAVE;

/*控制环参数*/
typedef struct
{
    uint16_t A_CNT;
    uint16_t B_CNT;

    uint16_t A_ACC_CNT;
    uint16_t B_ACC_CNT;

    int16_t A_EXPECT_CNT;
    int16_t B_EXPECT_CNT;

    uint16_t A_PWM;
    uint16_t B_PWM;

} CONTROL_TYPE;

typedef struct
{
    CONTROL_TYPE speed_control;
    CONTROL_TYPE location_control;
} CONTROL_SAVE;

#endif
