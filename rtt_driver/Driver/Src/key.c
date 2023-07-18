#include "key.h"

#define KEY_CLK RCC_APB2Periph_GPIOA
#define KEY_GPIO GPIOA
#define KEY0_pin GPIO_Pin_3
#define KEY1_pin GPIO_Pin_4
#define KEY2_pin GPIO_Pin_5

#define KEY0 GPIO_ReadInputDataBit(KEY_GPIO, KEY0_pin)
#define KEY1 GPIO_ReadInputDataBit(KEY_GPIO, KEY1_pin)
#define KEY2 GPIO_ReadInputDataBit(KEY_GPIO, KEY2_pin)

void key_init(void)
{

    RCC_APB2PeriphClockCmd(KEY_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = KEY0_pin | KEY1_pin | KEY2_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_GPIO, &GPIO_InitStructure);
}

uint8_t key_scan()
{
    uint8_t key_status;
    if (KEY0 == 0 || KEY1 == 0 || KEY2 == 0)
    {
        if (KEY0 == 0)
        {
            while (KEY0 != 1);
            key_status = key0_press;
        }

        if (KEY1 == 0)
        {
            while (KEY1 != 1);
            key_status = key1_press;
        }

        if (KEY2 == 0)
        {
            while (KEY2 != 1);
            key_status = key2_press;
        }
    }
    return key_status;
}


