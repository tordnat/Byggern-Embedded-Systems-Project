//CAN Interrupt handler on INT0, falling edge

#include <avr/common.h>
#include <avr/interrupt.h>
#include "can_interrupt.h"
#include "mcp2515.h"
#include "can.h"

can_message_t receive_buffer;

void can_interrupt_init(void){ // Assuming SREG is set to enable interrupts
	GICR  |= (1<<INT0); //Enable INT0
	MCUCR |= (0x2); //Falling edge on INT0 generates interrupt
}

ISR(INT0_vect){
	can_interrupt_handle();
}

void can_interrupt_handle(void){
	mcp2515_read_rx0(&receive_buffer);
	printf("CAN Interrupt:  ID: %i DATA: %s \n\r", receive_buffer.id, receive_buffer.data);
}