#include "encoder.h"

#define encoder_A_CLK               RCC_APB1Periph_TIM3
#define encoder_B_CLK               RCC_APB1Periph_TIM2
#define encoder_A_GPIO_CLK          RCC_APB2Periph_GPIOA
#define encoder_B_GPIO_CLK          RCC_APB2Periph_GPIOA

#define encoder_A_TIMx              TIM3
#define encoder_B_TIMx              TIM2

#define encoder_A_GPIO              GPIOA
#define encoder_B_GPIO              GPIOA

#define encoder_A_aside             GPIO_Pin_6
#define encoder_A_bside             GPIO_Pin_7
#define encoder_B_aside             GPIO_Pin_0
#define encoder_B_bside             GPIO_Pin_1

void encoder_a_init()
{
    RCC_APB1PeriphClockCmd(encoder_A_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(encoder_A_GPIO_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode                        = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin                         = encoder_A_aside | encoder_A_bside;
    GPIO_InitStructure.GPIO_Speed                       = GPIO_Speed_50MHz;
    GPIO_Init(encoder_A_GPIO, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period                = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler             = 1 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(encoder_A_TIMx, &TIM_TimeBaseInitStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel                     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_Channel                     = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter                    = 0xF;
    TIM_ICInit(encoder_A_TIMx, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(encoder_A_TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);

    TIM_SetCounter(encoder_A_TIMx, 0);
    TIM_Cmd(encoder_A_TIMx, ENABLE);
}

void encoder_b_init()
{
    RCC_APB1PeriphClockCmd(encoder_B_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(encoder_B_GPIO_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode                        = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin                         = encoder_B_aside | encoder_B_bside;
    GPIO_InitStructure.GPIO_Speed                       = GPIO_Speed_50MHz;
    GPIO_Init(encoder_B_GPIO, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period                = 65536 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler             = 1 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(encoder_B_TIMx, &TIM_TimeBaseInitStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel                     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_Channel                     = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter                    = 0xF;
    TIM_ICInit(encoder_B_TIMx, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(encoder_B_TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);

    TIM_SetCounter(encoder_B_TIMx, 0);
    TIM_Cmd(encoder_B_TIMx, ENABLE);
}

void encoder_init()
{
    encoder_a_init();
    encoder_b_init();
}

/*
 *获取编码器的计数值CNT
 *TIMx：    定时器序号
 *retval:   CNT
 */
uint16_t get_timer_cnt(TIM_TypeDef *TIMx)
{
    uint16_t CNT;
    CNT = TIM_GetCounter(TIMx);
    TIM_SetCounter(TIMx, 0);
    return CNT;
}


