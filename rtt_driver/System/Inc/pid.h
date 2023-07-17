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
    PID_TYPE increment;
    PID_TYPE location;
    PID_TYPE basic;
} PID_SAVE;

int pid_location(int k, PID_TYPE _pid);
int pid_basic(int k, PID_TYPE _pid);

#endif
