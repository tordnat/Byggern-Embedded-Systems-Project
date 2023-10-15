#include <avr/common.h>
#include <avr/interrupt.h>
#include "mem.h"
#include "spi.h"

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_SCK DDB7 //0b11111


void spi_master_init(void){
	DDRB |= 1 << DDB4; //Set SS port output
	// Set SCK and MOSI output, all others input
	DDR_SPI |= (1 <<DD_MOSI) | (1<<DD_SCK);
	// Enable SPI, Master, set clock rate at fck/16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0); 
}

void spi_transmit(char cData){
	SPDR = cData;
	while(!(SPSR & (1<<SPIF))); //Wait for transmission completed
}

uint8_t spi_read(void){
	spi_transmit('U'); //Don't care
	return SPDR;
}


void spi_transmit_string(char* string, int length){ //length in bytes
	for (int i = 0; i <= length; i++){
		spi_transmit(string[i]);
	}
}