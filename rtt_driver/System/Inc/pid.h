#ifndef __PID_H
#define __PID_H

typedef struct
{
    float kp;
    float ki;
    float kd;

    float bias;
    float last_bias;
    float last_last_bias;
    float integral_bias;
    float output;
} PID_TYPE;

typedef struct
{
    PID_TYPE A_location;
    PID_TYPE B_location;
    PID_TYPE A_increment;
    PID_TYPE B_increment;
} PID_SAVE;

float pid_location(int actual_location, int expect_location, PID_TYPE pid);
float pid_increment(float actual_cnt, float expect_cnt, PID_TYPE pid);

#endif
