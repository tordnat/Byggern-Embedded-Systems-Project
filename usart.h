#pragma once

#define F_CPU 4915200UL
#define BAUD 9600
#define UBRR 31 //F_CPU/BAUD/16-1

#include <avr/io.h>
#include <avr/common.h>
#include <util/delay.h>
#include <util/setbaud.h>

void usart_trans(uint8_t data);
uint8_t usart_recv(void);
void usart_init(unsigned long ubrr);