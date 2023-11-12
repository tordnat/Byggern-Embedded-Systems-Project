#include <avr/common.h>
#include "usart.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "spi.h"
#include "mcp2515.h"
#include "can.h"

// Static
static uint8_t mcp2515_load_tx0_buffer(can_message_t* message);
static void mcp2515_request_to_send(uint8_t selected_rts_buffer);
static uint8_t mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t value);
static uint8_t mcp2515_register_verify(uint8_t address, uint8_t expected_value, uint8_t bit_mask);
static void mcp2515_reset(void);
static void mcp2515_enable(void);
static void mcp2515_disable(void);


uint8_t mcp2515_init(){
	spi_master_init();
	mcp2515_reset(); //Send reset command
	_delay_ms(1);
	uint8_t status = mcp2515_read(MCP_CANSTAT);
	
	if ((status & MODE_MASK) != MODE_CONFIG){
		printf("CAN controller not in CONFIG, mode=%i", status);
		return 1;
	}
	
	// Speed 500kb/s, see datasheet for bitrate calculation
	mcp2515_write(MCP_CNF3, MCP_PHSEG2);
	mcp2515_write(MCP_CNF2, (MCP_PRSEG) | (MCP_PHSEG1 << 3) | (1 << 7));
	mcp2515_write(MCP_CNF1, (MCP_SJW << 6) | MCP_BRP );
	
	
	mcp2515_bit_modify(MCP_RXB0CTRL, MCP_RX_FILTER_MASK, 0x60); //Accept all msgs
	mcp2515_write(MCP_RXB1CTRL, 0x0);
	// Enable interrupt on receive
	mcp2515_write(MCP_CANINTE, 0x01); //Interrupt on message received in RXB0
	mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_ONESHOT); // Setting initial mode
	mcp2515_bit_modify(MCP_CANINTE, 0x1, 0xFF); //Set bits RX0IF HIGH
	// Set initial mode
	return 0;
}

uint8_t mcp2515_transmit_tx0(can_message_t* message){
	if(mcp2515_read_status(STATUS_TX0REQ)){
		printf("Err: TX0 Request Flag set \n\r");
		return 1;
	}
	mcp2515_load_tx0_buffer(message);
	mcp2515_request_to_send(MCP_RTS_ALL);
	return 0;
}

uint8_t mcp2515_read_rx0_to_buffer(can_message_t* message_buffer){
	
	if (!(mcp2515_read_status(STATUS_RX0IF))){
		printf("Err: Interrupt flag not handled\n\r");
		return 1;
	}
	message_buffer->id = (mcp2515_read(MCP_RXB0SIDL) >> SIDL_ROFFSET); // = to clear previous values 
	message_buffer->id |= (mcp2515_read(MCP_RXB0SIDH) << SIDH_LOFFSET);
	message_buffer->data_length = mcp2515_read(MCP_RXB0DLC);
	for (uint8_t i = 0; i < message_buffer->data_length; ++i){
		message_buffer->data[i] = mcp2515_read(MCP_RXB0D0 + i);
	}
	return 0;
}

static uint8_t mcp2515_load_tx0_buffer(can_message_t* message){
	if(message->data_length > MCP_MAX_DATA_LENGTH){
		printf("Err: Too long data length\n\r");
		return 1;
	}
	mcp2515_write(MCP_TXB0SIDH, (message->id >> SIDH_LOFFSET)); // Bits [3:10]
	mcp2515_write(MCP_TXB0SIDL, (SIDL_MASK & (message->id << SIDL_ROFFSET))); //Bits [0:2]
	mcp2515_write(MCP_TXB0DLC, message->data_length);
	for (uint8_t i = 0; i < message->data_length; ++i){
		mcp2515_write(MCP_TXB0D0+i, message->data[i]);
	}
	
	
	return 0;
}

static void mcp2515_request_to_send(uint8_t selected_rts_buffer){ // Instruct controller to begin message transmission for selected buffer i.e. MCP_RTS_TX0
	mcp2515_enable();
	spi_transmit(selected_rts_buffer);
	mcp2515_disable();
}

static uint8_t mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t value){
	mcp2515_enable();
	spi_transmit(MCP_BITMOD);
	spi_transmit(address);
	spi_transmit(mask);
	spi_transmit(value);
	mcp2515_disable();
	
	if(mcp2515_register_verify(address, value, mask)){
		printf("ERR: Unexpected value verifying\n\r");
		return 1;
	}
	return 0;
}

uint8_t mcp2515_read_status(uint8_t status_mask){
	uint8_t status;
	mcp2515_enable();
	spi_transmit(MCP_READ_STATUS);
	status = (spi_read() & status_mask);
	mcp2515_disable();
	return status;	
}

uint8_t mcp2515_write(uint8_t address, uint8_t value){
	mcp2515_enable();
	spi_transmit(MCP_WRITE); //Send instruction
	spi_transmit(address);   //Send write address
	spi_transmit(value);	 //Send value
	mcp2515_disable();
	
	if(mcp2515_register_verify(address, value, 0xFF)){
		printf("ERR: Unexpected value verifying\n\r");
		return 1;
	}
	return 0;
}

//Debug tool
static uint8_t mcp2515_register_verify(uint8_t address, uint8_t expected_value, uint8_t bit_mask){ 
	if (!MCP_DEBUG) return 0;
	_delay_ms(2); // Do NOT deploy: Will affect CAN transmission rate!!
	uint8_t data = mcp2515_read(address);
	if ((data & bit_mask) != expected_value){
		printf("R:%i Expected: %u, Read: %u\n\r", address, expected_value, data&bit_mask);
		mcp2515_disable();
		return 1;
	}
	return 0;
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

static void mcp2515_reset(void){
	mcp2515_enable(); //Disable and enable to make sure falling edge is detected
	spi_transmit(MCP_RESET);
	mcp2515_disable();
}

static void mcp2515_enable(void){
	PORTB &= ~(1 << PB4); //PB4 is chip select for SPI slave
}

static void mcp2515_disable(void){
	PORTB |= (1 << PB4); // mulig � legge inn en sjekk her for at verdien blir endret
}
