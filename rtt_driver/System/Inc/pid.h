#ifndef __PID_H
#define __PID_H
#include "structconfig.h"

int pid_location(int actual_location, int expect_location, PID_TYPE pid);
int pid_speed(int actual_speed, int expect_speed, PID_TYPE pid);
int pid_increment(int actual_cnt, int expect_cnt, PID_TYPE pid);

#endif
