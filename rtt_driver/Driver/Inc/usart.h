#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

void usart_init(uint8_t baudrate);
int8_t usart_receive(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
int fputc(int ch, FILE *f);
void Serial_Printf(char *format, ...);

#endif
