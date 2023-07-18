#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"

void encoder_init(void);
uint16_t get_timer_cnt(TIM_TypeDef *TIMx);

#endif
