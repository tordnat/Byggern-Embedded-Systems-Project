#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"
#include "mcp2515.h"
#include "spi.h"

static can_message_t msg;
int main(void) {
  //mcp2515_init();
  msg.id = 0x69;
  msg.data_length = 1;
  msg.data[0] = 'k';

    // Set pin 13 (PB5 on ATmega328P) as an output
    DDRB |= (1 << DDB5);

    while (1) {
        // Turn the LED on
        PORTB |= (1 << PORTB5);
        _delay_ms(500); // Delay for 1000 milliseconds
        //spi_transmit('k');
        // Turn the LED off
        PORTB &= ~(1 << PORTB5);
        //can_message_transmit(&msg);
        _delay_ms(500); // Delay for 1000 milliseconds
    }

    return 0;
}
