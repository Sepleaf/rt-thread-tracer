#include "led.h"

#define LED_CLK RCC_APB2Periph_GPIOB
#define LED_GPIO GPIOB
#define LED1_pin GPIO_Pin_0
#define LED2_pin GPIO_Pin_1

void led_init()
{
    RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED1_pin | LED2_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_GPIO, &GPIO_InitStructure);
}

void led1_on()
{
    GPIO_SetBits(LED_GPIO, LED1_pin);
}

void led1_off()
{
    GPIO_ResetBits(LED_GPIO, LED1_pin);
}

void led2_on()
{
    GPIO_SetBits(LED_GPIO, LED2_pin);
}

void led2_off()
{
    GPIO_ResetBits(LED_GPIO, LED2_pin);
}
