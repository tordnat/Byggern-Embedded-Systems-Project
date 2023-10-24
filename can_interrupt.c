//CAN Interrupt handler on INT0, falling edge

#include <avr/common.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "can_interrupt.h"
#include "mcp2515.h"
#include "can.h"

void can_interrupt_init(void){ // Assuming SREG is set to enable interrupts
	GICR  |= (1<<INT0); //Enable INT0
	MCUCR |= (0x2); //Falling edge on INT0 generates interrupt
}

ISR(INT0_vect){
	can_interrupt_handle();
}

void can_interrupt_handle(void){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		mcp2515_read_rx0(get_can_buffer_ptr());
	}	
}