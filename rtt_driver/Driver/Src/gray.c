#include "gray.h"

#define IIC_CLK                 RCC_APB2Periph_GPIOB
#define IIC_GPIO                GPIOB
#define SCL_Pin                 GPIO_Pin_8
#define SDA_Pin                 GPIO_Pin_9

#define gray_addr 0x9E

void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(IIC_GPIO, SCL_Pin, (BitAction)BitValue);
    delay_us(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(IIC_GPIO, SDA_Pin, (BitAction)BitValue);
    delay_us(10);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(IIC_GPIO, SDA_Pin);
    delay_us(10);
    return BitValue;
}

void gray_iic_init(void)
{
    RCC_APB2PeriphClockCmd(IIC_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode                = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin                 = SCL_Pin | SDA_Pin;
    GPIO_InitStructure.GPIO_Speed               = GPIO_Speed_50MHz;
    GPIO_Init(IIC_GPIO, &GPIO_InitStructure);

    GPIO_SetBits(IIC_GPIO, SCL_Pin | SDA_Pin);
}

void myiic_start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void myiic_stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void myiic_send_byte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t myiic_receive_byte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void myiic_send_ack(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t myiic_receive_ack(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}

void gray_write_reg(uint8_t RegAddress, uint8_t Data)
{
    myiic_start();
    myiic_send_byte(gray_addr);
    myiic_receive_ack();
    myiic_send_byte(RegAddress);
    myiic_receive_ack();
    myiic_send_byte(Data);
    myiic_receive_ack();
    myiic_stop();
}

uint8_t gray_read_reg(uint16_t RegAddress)
{

    uint8_t Data;
    myiic_start();
    myiic_send_byte(gray_addr);
    myiic_receive_ack();
    myiic_send_byte(RegAddress);
    myiic_receive_ack();

    myiic_start();
    myiic_send_byte(gray_addr | 0x01);
    myiic_receive_ack();
    Data = myiic_receive_byte();
    myiic_send_ack(1);
    myiic_stop();

    return Data;
}

/*
 *获取灰度传感器的值
 *retval: 8位无符号数
 */
uint8_t get_gray_value()
{
    return gray_read_reg(0xDD);
}

int bias_list[8] = {0, 1, 2, 5, 6, 8, 10, 12};

int gray_bias_list(uint8_t bias)
{
    switch (bias)
    {
    case 0xFF:
        return bias_list[0];
    case 0xE7:
        return bias_list[0];

    /***************/
    case 0xF7:
        return -bias_list[1];
    case 0xF3:
        return -bias_list[2];
    case 0xFB:
        return -bias_list[3];
    case 0xF9:
        return -bias_list[4];
    case 0xFD:
        return -bias_list[5];
    case 0xFC:
        return -bias_list[6];
    case 0xFE:
        return -bias_list[7];

    /***************/
    case 0xEF:
        return bias_list[1];
    case 0xCF:
        return bias_list[2];
    case 0xDF:
        return bias_list[3];
    case 0x9F:
        return bias_list[4];
    case 0xBF:
        return bias_list[5];
    case 0x3F:
        return bias_list[6];
    case 0x7F:
        return bias_list[7];
    default:
        return 0;
    }
}
/*
 * 灰度传感器偏差
 * *A_BIAS A侧误差值
 * *B_BIAS B侧误差值
 */
void gray_bias(float *A_BIAS, float *B_BIAS)
{
    float bias;
    bias = gray_bias_list(get_gray_value());
    *A_BIAS = bias;
    *B_BIAS = -bias;
}
