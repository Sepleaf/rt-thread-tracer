#ifndef __CCD_H
#define __CCD_H
#include "stm32f10x.h"
#include "delay.h"

typedef struct
{
    uint8_t times;
    uint8_t right_acq;
    uint8_t left_acq;
    uint8_t left_value;
    uint8_t right_value;
    uint8_t median;
    uint8_t threshold;
    uint16_t value_max;
    uint16_t value_min;
} CCD_TYPE;

void ccd_init(void);
uint16_t get_ccd_value(uint8_t channel);
void data_acquisition(void);

#endif
