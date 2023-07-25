#ifndef __PID_H
#define __PID_H
#include "structconfig.h"

float pid_location(float actual_location, float expect_location, PID_TYPE pid);
float pid_speed(float actual_speed, float expect_speed, PID_TYPE pid);
float pid_increment(float actual_cnt, float expect_cnt, PID_TYPE pid);

#endif
