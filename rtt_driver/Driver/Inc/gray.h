#ifndef __GRAY_H
#define __GRAY_H
#include "stm32f10x.h"
#include "delay.h"

void gray_iic_init(void);
void gray_write_reg(uint8_t RegAddress, uint8_t Data);
uint8_t gray_read_reg(uint16_t RegAddress);
uint8_t get_gray_value(void);

#endif
