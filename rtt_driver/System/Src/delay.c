#include "Delay.h"

/**
 * @brief  微秒级延时
 * @param  us 延时时长，范围：0~233015
 * @retval 无
 */
void delay_us(uint32_t us)
{
	u16 i = 0;
	while (us--)
	{
		i = 10; // 自己定义
		while (i--)
			;
	}
}
/**
 * @brief  毫秒级延时
 * @param  xms 延时时长，范围：0~4294967295
 * @retval 无
 */
void delay_ms(uint32_t xms)
{
	while (xms--)
	{
		delay_us(1000);
	}
}

/**
 * @brief  秒级延时
 * @param  xs 延时时长，范围：0~4294967295
 * @retval 无
 */
void delay_s(uint32_t xs)
{
	while (xs--)
	{
		delay_ms(1000);
	}
}
