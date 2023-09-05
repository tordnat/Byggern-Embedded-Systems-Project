

//#include "uart.h"
#define F_CPU 4915200UL
#define BAUD 9600
#define UBRR 31
//F_CPU/BAUD/16-1
#include "mem.h"

#include <avr/builtins.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include <avr/common.h>
#include <stdlib.h>
void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\r\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}
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
	_delay_ms(100);
	USART_init(UBRR);
	init_exmem();
	SRAM_test();
	
	//Output
	DDRE |= (1 << PE1);
	DDRA |= (1 << PA0);
	DDRA |= (1 << PA1);
	
	
	
	PORTE |= (1 << PE1); //LE=H
	_delay_ms(10); //Turn off leds
	PORTA &= ~(1 << PA0);
	PORTA &= ~(1 << PA1);
	_delay_ms(1000); //Turn on leds
	PORTA |= (1 << PA0);
	PORTA |= (1 << PA1);
	PORTE &= ~(1 << PE1); //Latch 
	_delay_ms(10);
	PORTA &= ~(1 << PA0); //Nothing happens
	PORTA &= ~(1 << PA1);
	
	
	
	while (1)
	{
		printf("test\n\r");
		_delay_ms(1000);
		
		
	}
}
