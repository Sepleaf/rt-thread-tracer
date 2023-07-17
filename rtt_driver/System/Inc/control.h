#ifndef __CONTROL_H
#define __CONTROL_H
#include "motor.h"
#include "pid.h"
#include "gray.h"

void gray_bias(int *k, int *error);
void motor_control(int k, int error);

#endif
