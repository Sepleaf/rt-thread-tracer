#include "led.h"

#define led_clk         RCC_APB2Periph_GPIOB
#define led_gpio        GPIOB
#define led1_pin        GPIO_Pin_0
#define led2_pin        GPIO_Pin_1

void led_init()
{
    RCC_APB2PeriphClockCmd(led_clk, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin             = led1_pin | led2_pin;
    GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode            = GPIO_Mode_Out_PP;
    GPIO_Init(led_gpio, &GPIO_InitStructure);
}

void led1_on()
{
    GPIO_SetBits(led_gpio, led1_pin);
}

void led1_off()
{
    GPIO_ResetBits(led_gpio, led1_pin);
}

void led2_on()
{
    GPIO_SetBits(led_gpio, led2_pin);
}

void led2_off()
{
    GPIO_ResetBits(led_gpio, led2_pin);
}
