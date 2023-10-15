#include <avr/common.h>
#include <avr/io.h>
#include <stdio.h>
#include "spi.h"
#include "mcp2515.h"
/*
Plan: 
- Kun bruke ett transmit register B0

*/
uint8_t mcp2515_init(){
	uint8_t value;
	spi_master_init();
	mcp2515_reset(); //Send reset command
	
	value = mcp2515_read(MCP_CANSTAT);
	if((value & MODE_MASK) != MODE_CONFIG){
		printf("ERR: MCP2515 not in configuration mode after reset!\n\r");
		return 1;
	}
	
	mcp2515_bit_modify(MCP_CANCTRL, MCP_INIT_MODE, MODE_MASK); // Setting initial mode
	value = mcp2515_read(MCP_CANSTAT); 
	if((value & MODE_MASK) != MCP_INIT_MODE){
		printf("ERR: Error setting MCP2515 mode to MCP_INIT_MODE!\n\r");
		return 1;
	}
	///MORE INIT HERE
	
	return 0;
}

uint8_t mcp2515_reset(void){
	mcp2515_disable();
	mcp2515_enable(); //Disable and enable to make sure falling edge is detected
	spi_transmit(MCP_RESET);
	mcp2515_disable();
	return 1;
}

void mcp2515_enable(void){
	PORTB &= ~(1 << PB4); //PB4 is chip select for SPI slave	
}

void mcp2515_disable(void){
	PORTB |= 1 << PB4; // mulig å legge inn en sjekk her for at verdien blir endret
}

uint8_t mcp2515_transmit_tx0(uint8_t data, uint8_t id){ //We do not use more than 2^8 ids in this implementation
	uint8_t data_length_code = 0x01; //We only send 1 byte
	mcp2515_load_tx0_buffer(data,id);
	mcp2515_write(MCP_TXB0DLC, &data_length_code);
	mcp2515_request_to_send(MCP_RTS_TX0);
	while ((mcp2515_read_status() & (1 << 0x02))); //Wait for successful transmission
	return 0;
}

uint8_t mcp2515_read_rx0(void){
	uint8_t data;
	data = mcp2515_read_rx_buffer(0b10010010); //Could read sequentially
	return data;
}

void mcp2515_write(uint8_t address, uint8_t* value){
	mcp2515_enable();
	spi_transmit(MCP_WRITE); //Send instruction
	spi_transmit(address);   //Send write address
	spi_transmit(*value);	 //Send value
	mcp2515_disable();
}

uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	mcp2515_enable();
	spi_transmit(MCP_READ);
	spi_transmit(address);
	result = spi_read();
	mcp2515_disable();
	return result;
}

uint8_t mcp2515_read_rx_buffer(uint8_t rx_buffer_addr){
	uint8_t data;
	mcp2515_enable();
	spi_transmit(rx_buffer_addr);
	data = spi_read();
	mcp2515_disable();
	return data;
}

void mcp2515_load_tx0_buffer(uint8_t data, uint8_t id){
	mcp2515_enable();
	spi_transmit(MCP_LOAD_TX0);
	spi_transmit(id);
	spi_transmit(MCP_LOAD_TX0+1);
	spi_transmit(data);
	mcp2515_disable();
}

uint8_t mcp2515_request_to_send(uint8_t selected_rts_buffer){ // Instruct controller to begin message transmission for selected buffer i.e. MCP_RTS_TX0
	spi_transmit(selected_rts_buffer);
	return selected_rts_buffer;
}

void mcp2515_bit_modify(uint8_t address, uint8_t value, uint8_t mask){
	mcp2515_enable();
	spi_transmit(MCP_BITMOD);
	spi_transmit(address);
	spi_transmit(mask);
	spi_transmit(value);
	mcp2515_disable();
}
uint8_t mcp2515_read_status(void){
	uint8_t status;
	mcp2515_enable();
	spi_transmit(MCP_READ_STATUS);
	status = spi_read();
	mcp2515_disable();
	return status;
	
}