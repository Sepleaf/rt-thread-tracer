#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

#define key0_press 1
#define key1_press 2
#define key2_press 3

void key_init(void);
uint8_t key_scan(void);

#endif
