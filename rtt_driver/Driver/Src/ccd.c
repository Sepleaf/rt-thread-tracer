#include "ccd.h"

#define ADCx                            ADC1
#define ADC_CLK                         RCC_APB2Periph_ADC1
#define GPIO_CLK                        RCC_APB2Periph_GPIOA
#define ADC_GPIO                        GPIOA
#define ADC_Pin                         GPIO_Pin_4

#define CCD_CLK                         RCC_APB2Periph_GPIOA
#define CCD_GPIO                        GPIOA
#define SI_Pin                          GPIO_Pin_3
#define CLK_Pin                         GPIO_Pin_2

#define GPIOA_ODR_Addr                  (GPIOA_BASE + 12) // 0x4001080C
#define BITBAND(addr, bitnum)           ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)                  *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum)          MEM_ADDR(BITBAND(addr, bitnum))
#define PAout(n)                        BIT_ADDR(GPIOA_ODR_Addr, n)

#define tsl_si PAout(3)
#define tsl_clk PAout(2)
// #define si_set                  GPIO_SetBits(CCD_GPIO, SI_Pin)
// #define clk_set                 GPIO_SetBits(CCD_GPIO, CLK_Pin)
// #define si_reset                GPIO_ResetBits(CCD_GPIO, SI_Pin)
// #define clk_reset               GPIO_ResetBits(CCD_GPIO, CLK_Pin)

void ccd_adc_init()
{

    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(GPIO_CLK | ADC_CLK, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitStructure.GPIO_Pin                 = ADC_Pin;
    GPIO_InitStructure.GPIO_Mode                = GPIO_Mode_AIN;
    GPIO_Init(ADC_GPIO, &GPIO_InitStructure);
    ADC_DeInit(ADCx);

    ADC_InitStructure.ADC_Mode                  = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode          = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode    = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv      = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel          = 1;
    ADC_Init(ADCx, &ADC_InitStructure);

    ADC_Cmd(ADCx, ENABLE);
    ADC_ResetCalibration(ADCx);
    while (ADC_GetResetCalibrationStatus(ADCx));

    ADC_StartCalibration(ADCx);
    while (ADC_GetCalibrationStatus(ADCx));
}

void ccd_init()
{
    ccd_adc_init();
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(CCD_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin                 = CLK_Pin;
    GPIO_InitStructure.GPIO_Mode                = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed               = GPIO_Speed_50MHz;
    GPIO_Init(CCD_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(CCD_GPIO, CLK_Pin);

    GPIO_InitStructure.GPIO_Pin                 = SI_Pin;
    GPIO_Init(CCD_GPIO, &GPIO_InitStructure);
    GPIO_SetBits(CCD_GPIO, SI_Pin);
}

/*
 *获取ADC通道值
 *channel：通道 ADC_Channel_0 ~ ADC_Channel_9
 */
uint16_t get_ccd_value(uint8_t channel)
{
    ADC_RegularChannelConfig(ADCx, channel, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
    while (!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
        ;
    return ADC_GetConversionValue(ADCx);
}
CCD_TYPE ccd = {
    100, 0, 0, 0, 0, 0, 0, 0, 0};
/*
 *计算CCD中值
 */
void data_acquisition()
{
    tsl_clk = 1;
    tsl_si = 0;
    // clk_set;
    // si_reset;
    delay_us(ccd.times);

    tsl_si = 1;
    tsl_clk = 0;
    // si_set;
    // clk_reset;
    delay_us(ccd.times);

    tsl_clk = 1;
    tsl_si = 0;
    // clk_set;
    // si_reset;
    delay_us(ccd.times);

    uint8_t i = 0;
    uint8_t t = 0;
    uint8_t ccd_adc[128];
    for (i = 0; i < 128; i++)
    {
        tsl_clk = 0;
        // clk_reset;
        delay_us(ccd.times); // 曝光时间
        ccd_adc[t] = (uint8_t)((float)get_ccd_value(ADC_Channel_4) / 4096 * 255);
        t++;
        tsl_clk = 1;
        // clk_set;
        delay_us(ccd.times);
    }

    ccd.value_max = ccd_adc[0]; // 动态阈值算法，读取最大和最小值
    for (i = 5; i < 123; i++)   // 两边各去掉5个点
    {
        if (ccd.value_max <= ccd_adc[i])
            ccd.value_max = ccd_adc[i];
    }

    ccd.value_min = ccd_adc[0]; // 最小值
    for (i = 5; i < 123; i++)
    {
        if (ccd.value_min >= ccd_adc[i])
        {
            ccd.value_min = ccd_adc[i];
        }
    }
    ccd.threshold = (ccd.value_max + ccd.value_min) / 2;

    for (ccd.left_acq = 5; ccd.left_acq < 118; ccd.left_acq++) // 寻找左边跳变沿
    {
        if (ccd_adc[ccd.left_acq] > ccd.threshold &&
            ccd_adc[ccd.left_acq + 1] > ccd.threshold &&
            ccd_adc[ccd.left_acq + 2] > ccd.threshold &&
            ccd_adc[ccd.left_acq + 3] < ccd.threshold &&
            ccd_adc[ccd.left_acq + 4] < ccd.threshold &&
            ccd_adc[ccd.left_acq + 5] < ccd.threshold)
        {
            ccd.left_value = ccd.left_acq;
            break;
        }
    }

    for (ccd.right_acq = 118; ccd.right_acq > 5; ccd.right_acq--) // 寻找右边跳变沿
    {
        if (ccd_adc[ccd.right_acq] < ccd.threshold &&
            ccd_adc[ccd.right_acq + 1] < ccd.threshold &&
            ccd_adc[ccd.right_acq + 2] < ccd.threshold &&
            ccd_adc[ccd.right_acq + 3] > ccd.threshold &&
            ccd_adc[ccd.right_acq + 4] > ccd.threshold &&
            ccd_adc[ccd.right_acq + 5] > ccd.threshold)
        {
            ccd.right_value = ccd.right_acq;
            break;
        }
    }
    ccd.median = (ccd.right_value + ccd.left_value) / 2;
}

/*
 * CCD偏差值
 * a & b 侧偏差值互为相反数
 * *a_bias：a侧偏差
 * *b_bias：b侧偏差
 */
void ccd_bias(float *a_bias, float *b_bias)
{
    *a_bias = (float)(ccd.median - 64) / 10;
    *b_bias = (float)(64 - ccd.median) / 10;
}
