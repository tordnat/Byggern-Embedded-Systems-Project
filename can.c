#include <avr/common.h>
#include <stdio.h>
#include "mcp2515.h"
#include "can.h"
/*
This driver assumes CANopen standard message ID format. 
*/

/*
Plan:
- implement needed low-level functions
- implement high-level CAN send functions (node 1 only sends)
- Should be done with function pointers to eliminate boilerplate code
*/

uint8_t can_message_transmit(can_message_t* message){
	mcp2515_transmit_tx0(message);
	return 0;
}

can_message_t* get_can_buffer_ptr(void){
	return &receive_buffer;
}
