#ifndef __CCD_H
#define __CCD_H
#include "stm32f10x.h"
#include "delay.h"
#include "structconfig.h"

void ccd_adc_init(void);
void ccd_init(void);
uint16_t get_ccd_value(uint8_t channel);
void data_acquisition(void);
void ccd_bias(float *a_bias, float *b_bias);

#endif
