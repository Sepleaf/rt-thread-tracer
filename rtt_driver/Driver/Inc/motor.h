#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"

void motor_pwm_init(uint16_t arr, uint16_t psc);
void motor_in_init(void);
void motor_stop(void);
void motor_forward(void);
void motor_right(void);
void motor_left(void);
void motor_back(void);
void load_pwm(uint16_t PWMA, uint16_t PWMB);

#endif
