#include <avr/common.h>

#include "spi.h"

uint8_t mcp2515_init(){
	uint8_t value;
	spi_master_init();
	mcp2515_reset(); //Send reset command
	
	mcp2515_read(MCP_CANSTAT, &value);
	if((value & MODEMASK) != MODE_CONFIG){
		printf("ERR: MCP2515 not in configuration mode after reset!\n\r");
		return 1;
	}
	
	///MORE INIT HERE
	
	return 0;
}
uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	PORTB &= ~(1 << CAN_CS); // CS Done outside spi_transmit to avoid spikes
	spi_transmit(MCP_READ);
	spi_transmit(address);
	result = spi_read();
	
	PORTB |= (1 << CAN_CS);
	return result;
}

void mcp2515_write();
uint8_t mcp2515_request_to_send();
void mcp2515_bit_modify();
uint8_t mcp2515_reset();
uint8_t mcp2515_read_status();