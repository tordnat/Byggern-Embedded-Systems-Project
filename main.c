

#include <avr/builtins.h>
#include <avr/common.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "usart.h"

#include "mem.h"
#include "pwm.h"



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


int main(void)
{
	_delay_ms(100);
	usart_init(UBRR);
	exmem_init();
	DDRD |= (1 << PD5); //Output
	OCR1A = 0x3;
	TCCR1A |= (1 << COM1A1); //Clear OC1A on Compare Match (Set output to low level).
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << WGM12) | (1 << WGM13); // 8 bit fast PWM with OCR1A as TOP
	TCCR1B |= (1 << CS10); //prescaler = 0
	//Max uses CLK betwwen 0.5Mhz and 5Mhz
	
	//SRAM_test();
	//Output
	//DDRA |= (1 << PA0;)
	//PORTA |= (1 << PA0);
	//PORTA |= (1 << PA1);

	
	printf("%i\n\r", exmem_read(0));
	exmem_write(0xFF, 0);
	printf("%i\n\r", exmem_read(0));

	
	sei();
	while (1)
	{
		printf("test\n\r");
		_delay_ms(1000);
		
		
	}
}

