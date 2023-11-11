#include "usart.h"
#include "adc.h"

#include <avr/io.h>
#include <util/delay.h>
#define ADC_ADDRESS 0x1400



void adc_init(void) {
	DDRD |= (1 << PD5);
	OCR1A = 0x02;
	TCCR1A |= (1 << COM1A0); //Toggle on Compare match
	TCCR1B |= (1 << CS10); //No prescale
	TCCR1B |= (1 << WGM12); //CTC
	
}

uint8_t adc_read(uint8_t channel) {
	volatile char* adc_mem = (char*) ADC_ADDRESS;
	volatile char read_val; //Init read
	
	adc_mem[0] = 0; //Activate ADC by writing to it
	_delay_us(20);
	for(uint8_t i = 0; i <= channel; i++) { //Channel X is by X consecutive writes
		read_val = adc_mem[0x0];
	}
	return read_val;
}

uint8_t* adc_read_to_array(uint8_t channel, uint8_t* sample_array, uint8_t size){
	for (uint8_t i = 0; i <= size; i++){
		sample_array[i] = adc_read(channel);
	}
	return sample_array;
}