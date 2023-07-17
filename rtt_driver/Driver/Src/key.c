#include "key.h"

#define KEY_CLK RCC_APB2Periph_GPIOB
#define KEY_GPIO GPIOB
#define KEY_pin GPIO_Pin_10

#define KEY0 GPIO_ReadInputDataBit(KEY_GPIO, KEY_pin)

void key_init(void)
{

    RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = KEY_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
}

uint8_t key_scan()
{
    uint8_t key_num = 0;
    if (KEY0 == 0)
    {
        while (KEY0 != 1);
            key_num = 1;
    }

    return key_num;
}
