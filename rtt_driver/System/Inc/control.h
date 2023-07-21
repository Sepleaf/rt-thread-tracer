#ifndef __CONTROL_H
#define __CONTROL_H
#include "motor.h"
#include "pid.h"
#include "gray.h"
#include "encoder.h"

void gray_bias(float *A_BIAS, float *B_BIAS);
void speed_controller(float Excpect_A_CNT, float Excpect_B_CNT);
void location_controller(uint16_t distance_cm);

#endif
