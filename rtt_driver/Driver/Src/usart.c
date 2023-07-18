#include "usart.h"

#define usart_clk               RCC_APB1Periph_USART2
#define gpio_clk                RCC_APB2Periph_GPIOB
#define usart_gpio              GPIOB
#define usartx                  USART3
#define tx_pin                  GPIO_Pin_10
#define rx_pin                  GPIO_Pin_11

void usart_init(uint8_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStrue;
    USART_InitTypeDef USART_InitStrue;
    RCC_APB1PeriphClockCmd(usart_clk, ENABLE);
    RCC_APB2PeriphClockCmd(gpio_clk, ENABLE);

    GPIO_InitStrue.GPIO_Mode                    = GPIO_Mode_AF_PP;
    GPIO_InitStrue.GPIO_Pin                     = tx_pin;
    GPIO_InitStrue.GPIO_Speed                   = GPIO_Speed_10MHz;
    GPIO_Init(usart_gpio, &GPIO_InitStrue);

    GPIO_InitStrue.GPIO_Mode                    = GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin                     = rx_pin;
    GPIO_InitStrue.GPIO_Speed                   = GPIO_Speed_10MHz;
    GPIO_Init(usart_gpio, &GPIO_InitStrue);

    USART_InitStrue.USART_BaudRate              = baudrate;
    USART_InitStrue.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    USART_InitStrue.USART_Mode                  = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStrue.USART_Parity                = USART_Parity_No;
    USART_InitStrue.USART_StopBits              = USART_StopBits_1;
    USART_InitStrue.USART_WordLength            = USART_WordLength_8b;
    USART_Init(usartx, &USART_InitStrue);

    USART_Cmd(usartx, ENABLE);
}

int8_t usart_receive()
{
    return USART_ReceiveData(usartx);
}

void Serial_SendByte(uint8_t Byte)    //发送一个字节
{
	USART_SendData(USART1, Byte);    //发送数据，BYTE写入TDR
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待下一个标志位，标志位置1后会自动清零，
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)     //发送一个数组，数组和长度
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);     //一次发送数组中的元素
	}
}

void Serial_SendString(char *String)//发送字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);//一个个发送
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)    //拆分数字用的子函数
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)         //发送数字，数字的位数
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');   //拆分数字的每一个数发送
	}
}

int fputc(int ch, FILE *f)          //重定向fputc到串口，fputc是printf的底层，fputc一个个打印到printf
{
	Serial_SendByte(ch);     //一个个发送
	return ch;
}

void Serial_Printf(char *format, ...)     //封装sprintf
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

