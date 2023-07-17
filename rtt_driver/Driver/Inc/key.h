#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

#define key_press 1

void key_init(void);
uint8_t key_scan(void);

#endif
