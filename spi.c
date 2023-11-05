#include <avr/common.h>
#include <avr/interrupt.h>
#include "mem.h"
#include "spi.h"

#define DDR_SPI DDRB
#define DD_MOSI DDB5
#define DD_SCK DDB7 //0b11111


void spi_master_init(void){
	DDRB = (1 << DDB5) | (1 << DDB7) | (1 << DDB4); //Set SS port output
	// Set SCK and MOSI output, all others input
	DDR_SPI |= (1 <<DD_MOSI) | (1<<DD_SCK);
	// Enable SPI, Master, set clock rate at fck/16
	SPCR &= ~((1<<SPI2X) | (1<<SPR1));
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0); 
}

void spi_transmit(char cData){
	cli(); //Disable interrupts
	SPDR = cData;
	while(!(SPSR & (1<<SPIF))); //Wait for transmission completed
	sei();
}

uint8_t spi_read(void){
	spi_transmit('U'); //Don't care
	return SPDR;
}
