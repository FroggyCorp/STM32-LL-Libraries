#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stm32l0xx_ll_utils.h>
#include <stm32l0xx_ll_rcc.h>
#include <stm32l0xx_ll_gpio.h>
#include <stm32l0xx_ll_bus.h>
#include <stm32l0xx_ll_usart.h>

int __io_putchar(int ch);   //fonction printf
extern "C" int _write(int file,char *ptr, int len);
void SerialInit(USART_TypeDef *USARTx, uint32_t baudrate);

void ReInit1msTick();
void mDelay(uint32_t delay);
#endif