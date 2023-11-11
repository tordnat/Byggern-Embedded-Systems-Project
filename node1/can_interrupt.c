// CAN Interrupt handler on INT0, falling edge

#include <avr/common.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "can_interrupt.h"
#include "mcp2515.h"
#include "can.h"

void can_interrupt_init(void){
	DDRD  &= ~(1 << PD2); //PIN2 input
	GICR  |= (1<<INT0); //Enable INT0
	MCUCR |= (0x2); //Interrupt on falling edge
}

ISR(INT0_vect){
	can_rx_interrupt_handle();
}

void can_rx_interrupt_handle(void){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		mcp2515_read_rx0_to_buffer(get_can_receive_buffer_ptr());
		mcp2515_interrupt_flags = mcp2515_read(MCP_CANINTF);
		mcp2515_write(MCP_CANINTF, 0x0); //Clear interrupt
		can_new_message_received = 1;
	}	
}