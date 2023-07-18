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
    float pwm;
} PID_TYPE;

typedef struct
{
    PID_TYPE position;
    PID_TYPE A_increment;
    PID_TYPE B_increment;
} PID_SAVE;

int pid_position(int cnt, int target, PID_TYPE _pid);
int pid_increment(float CNT, float expect_cnt, PID_TYPE pid);

#endif
