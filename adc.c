#include "adc.h"

void adc_init(void) {
	DDRB |= (1 << PB0);
	OCR0 = 0x02;
	TCCR0 |= (1 << WGM01); //CTC
	TCCR0 |= (1 << COM00); //Toggle, non PWM
	TCCR0 |= (1 << CS00); // No prescale
	
}

uint8_t adc_read() {
	volatile char* adc_mem = (char*) ADC_ADDRESS;
	volatile char read_val; //init read
	
	//uint8_t channel = 0;
	adc_mem[0] = 0;
	_delay_ms((9*(1*2))/819200);

	//for(int i = 0; i <= channel; i++) {} read channel number of times
	read_val = adc_mem[0];
	return read_val;
}