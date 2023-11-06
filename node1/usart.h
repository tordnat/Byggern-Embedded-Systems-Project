#pragma once
#include <avr/common.h>
#include <avr/builtins.h>
#define UBRR 31 //F_CPU/BAUD/16-1
#define F_CPU 4915200UL

void usart_trans(uint8_t data);
uint8_t usart_recv(void);
void usart_init(unsigned long ubrr);
