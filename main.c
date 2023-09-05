//#include "uart.h"
#define F_CPU 4915200UL
#define BAUD 9600
#define UBRR 31
//F_CPU/BAUD/16-1

#include <avr/io.h>
#include <avr/builtins.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include <avr/common.h>
//#include <avr/io.h>
void USART_trans(uint8_t data) {
	//Wait for transmit buffer to clear
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
	
}
uint8_t USART_recv(void) {
	//Wait for byte
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}
void USART_init(unsigned long ubrr) {
	//Set BAUD
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) (ubrr);
	
	//Enable recv and trans
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	//UBRRH == UCSRC. SELECT UCSRC
	//Frame format: 1 stop bit, 8 bit data
	UCSR0C = (1 << URSEL0) |(1 << UCSZ01) | (1 << UCSZ00);
	fdevopen(USART_trans, USART_recv);
}


int main(void)
{
	//Output
	DDRA |= (1 << PA0);
	USART_init(UBRR);
	
	while (1)
	{
		//PORTA |= (1 << PA0);
		//_delay_ms(10);
		//PORTA &= ~(1 << PA0);
		printf("test\n");
		_delay_ms(10);
		
	}
}

