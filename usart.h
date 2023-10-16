#pragma once

#define F_CPU 4915200UL

void usart_trans(uint8_t data);
uint8_t usart_recv(void);
void usart_init(unsigned long ubrr);
