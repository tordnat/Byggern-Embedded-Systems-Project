#include "usart.h"
#define BAUD 9600

#include <avr/io.h>
#include <avr/common.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <stdlib.h>

void usart_trans(uint8_t data) {
	//Wait for transmit buffer to clear
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
	
}
uint8_t usart_recv(void) {
	//Wait for byte
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}
void usart_init(unsigned long ubrr) {
	//Set BAUD
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) (ubrr);
	
	//Enable recv and trans
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	//UBRRH == UCSRC. SELECT UCSRC
	//Frame format: 1 stop bit, 8 bit data
	UCSR0C = (1 << URSEL0) |(1 << UCSZ01) | (1 << UCSZ00);
	fdevopen(usart_trans, usart_recv);
}
