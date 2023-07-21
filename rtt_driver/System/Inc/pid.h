#ifndef __PID_H
#define __PID_H

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

int pid_location(int actual_location, int expect_location, PID_TYPE pid);
int pid_speed(int actual_speed, int expect_speed, PID_TYPE pid);
int pid_increment(int actual_cnt, int expect_cnt, PID_TYPE pid);

#endif
