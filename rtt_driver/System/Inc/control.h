#ifndef __CONTROL_H
#define __CONTROL_H
#include "motor.h"
#include "pid.h"
#include "gray.h"
#include "ccd.h"
#include "encoder.h"
#include "structconfig.h"

void gray_bias(float *A_BIAS, float *B_BIAS);
void speed_controller(float Excpect_A_CNT, float Excpect_B_CNT, CONTROL_TYPE *controller);
void location_controller(uint16_t distance_cm, CONTROL_TYPE *controller);
void bias_controller(float a_bias, float b_bias, CONTROL_TYPE *controller);

#endif
