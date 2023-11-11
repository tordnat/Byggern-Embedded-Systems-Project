#include <avr/io.h>

#define SPI_DDR DDRB
#define CS     PINB2
#define MOSI   PINB3
#define MISO   PINB4
#define SCK    PINB5

/* This implementation is heavily inspired from the ATmega328P datasheet
* url: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

void spi_master_init(void){
   SPI_DDR |= (1 << CS) | (1 << MOSI) | (1 << SCK); // Set CS, MOSI, MISo as output
   SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0); //fosc/128
}

void spi_transmit(char data){
  SPDR = data; // Start transmission
  while (!(SPSR & (1 << SPIF))); // Wait for transmission to complete
}

uint8_t spi_read(void){
  spi_transmit('U'); // Don't care
  return  SPDR;
}
