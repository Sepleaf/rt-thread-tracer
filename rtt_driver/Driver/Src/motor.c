#include "motor.h"

#define PWM_CLK RCC_APB2Periph_GPIOA
#define TIM_CLK RCC_APB2Periph_TIM1
#define IN_CLK RCC_APB2Periph_GPIOB
#define TIMx TIM1

#define PWMA_GPIO GPIOA
#define PWMB_GPIO GPIOA
#define PWMA_pin GPIO_Pin_8
#define PWMB_pin GPIO_Pin_11

#define IN_GPIO GPIOB
#define AIN1_pin GPIO_Pin_13
#define AIN2_pin GPIO_Pin_12
#define BIN1_pin GPIO_Pin_14
#define BIN2_pin GPIO_Pin_15

#define AIN1_out GPIO_ResetBits(IN_GPIO, AIN1_pin)
#define AIN2_out GPIO_ResetBits(IN_GPIO, AIN2_pin)
#define BIN1_out GPIO_ResetBits(IN_GPIO, BIN1_pin)
#define BIN2_out GPIO_ResetBits(IN_GPIO, BIN2_pin)

#define AIN1_set GPIO_SetBits(IN_GPIO, AIN1_pin)
#define AIN2_set GPIO_SetBits(IN_GPIO, AIN2_pin)
#define BIN1_set GPIO_SetBits(IN_GPIO, BIN1_pin)
#define BIN2_set GPIO_SetBits(IN_GPIO, BIN2_pin)

#define MAX_PWM 4000
#define MIN_PWM 1000

void motor_pwm_init(uint16_t arr, uint16_t psc)
{
	motor_in_init();
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(TIM_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(IN_CLK | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(PWM_CLK | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PWMA_pin | PWMB_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWMA_GPIO, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIMx, ENABLE);
	TIM_Cmd(TIMx, ENABLE);
}

void motor_in_init()
{
	RCC_APB2PeriphClockCmd(IN_CLK, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = (AIN1_pin | AIN2_pin | BIN1_pin | BIN2_pin);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(IN_GPIO, &GPIO_InitStructure);
}

void motor_stop()
{
	AIN1_set;
	AIN2_set;
	BIN1_set;
	BIN2_set;
}

void motor_forward()
{
	AIN1_out;
	AIN2_set;
	BIN1_out;
	BIN2_set;
}

void motor_right()
{
	AIN1_set;
	AIN2_out;
	BIN1_out;
	BIN2_set;
}

void motor_left()
{
	AIN1_out;
	AIN2_set;
	BIN1_set;
	BIN2_out;
}

void motor_back()
{
	AIN1_set;
	AIN2_out;
	BIN1_set;
	BIN2_out;
}

void motor_check(int Speed)
{
	TIM_SetCompare1(TIMx, Speed); // PWMA
	TIM_SetCompare4(TIMx, Speed); // PWMB
	AIN1_set;
	AIN2_out;
	BIN1_set;
	BIN2_out;
}

void load_pwm(uint16_t PWMA, uint16_t PWMB)
{
	if (PWMA > MAX_PWM)
		PWMA = MAX_PWM;
	if (PWMB < MIN_PWM)
		PWMB = MIN_PWM;

	if (PWMA > MAX_PWM)
		PWMA = MAX_PWM;
	if (PWMB < MIN_PWM)
		PWMB = MIN_PWM;

	TIM_SetCompare1(TIMx, PWMA);
	TIM_SetCompare4(TIMx, PWMB);
}
