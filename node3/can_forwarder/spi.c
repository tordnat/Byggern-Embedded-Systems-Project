#include <avr/builtins.h>
#include <avr/io.h>

#define SPI_DDR DDRB
#define CS     PINB1
#define DD_MOSI   PINB3
#define DD_MISO   PINB4
#define DD_SCK    PINB5
#define DDR_SPI DDRB
/* This implementation is heavily inspired from the ATmega328P datasheet
* url: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

void spi_master_init(void){
	DDRB = (1 << DD_MOSI) | (1 << CS) | (1 << DDB4); //Set SS port output
  DDRB &= ~((1<<PB4));	// Enable SPI, Master, set clock rate at fck/16

	//SPCR &= ~((1<<SPI2X) | (1<<SPR1));
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1); 
}



void spi_transmit(char data){
  SPDR = data; // Start transmission
  while (!(SPSR & (1 << SPIF))); // Wait for transmission to complete
}

uint8_t spi_read(void){
  spi_transmit('U'); // Don't care
  return  SPDR;
}

