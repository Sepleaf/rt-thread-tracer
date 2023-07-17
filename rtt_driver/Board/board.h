#ifndef __BOARD_H
#define __BOARD_H
#include "motor.h"
#include "OLED.h"
#include "gray.h"
#include "pid.h"
#include "key.h"
#include "led.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif
