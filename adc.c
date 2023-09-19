#include "adc.h"

void adc_init(void) {
	DDRD |= (1 << PD5);
	OCR1A = 0x02;
	TCCR1A |= (1 << COM1A0); //Toggle on Compare match
	TCCR1B |= (1 << CS10); //No prescale
	TCCR1B |= (1 << WGM12); //CTC
	
}

uint8_t adc_read(int channel) {
	volatile char* adc_mem = (char*) 0x1400;
	volatile char read_val; //init read
	
	adc_mem[0] = 0; //Write to adc to activate
	//_delay_ms((9*(1*2))/819200);
	_delay_us(20);
	for(int i = 0; i <= channel; i++) {
		read_val = adc_mem[0x0];
	}

	return read_val;
}

uint8_t* adc_sample_to_array(uint8_t channel, uint8_t* sample_array, uint8_t size){
	for (int i = 0; i <= size; i++){
		sample_array[i] = adc_read(channel);
	}
	return sample_array;
}