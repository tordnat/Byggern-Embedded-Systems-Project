#include <avr/builtins.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "usart.h"
#include <util/delay.h>
#include "adc.h"
#include "joystick.h"
#include "mem.h"
#include "oled.h"
#include "gui.h"
#include "can.h"
#include "mcp2515.h"
#include "can_interrupt.h"

static gui_menu_item *gui_menu_current;

int main(void)
{
	_delay_ms(30);
	usart_init(UBRR);
	exmem_init();
	adc_init();
	mcp2515_init();
	can_interrupt_init();
	sei(); //enable interrupts
		
	joystick_full_calibration(200);
	oled_init();
	_delay_ms(30);
	oled_reset();
	_delay_ms(30);
	
	
	//enum character c = char_arrow;
	static can_message_t test_message;
	static node1_msg_t msg;

	gui_menu_current = gui_init();
	can_message_t* buffer = get_can_buffer_ptr();
	
	_delay_ms(30);

	while (1) {
		gui_goto_menu(&gui_menu_current);

		msg = get_node1_msg();
		can_encode_node1_msg(&msg, &test_message);
		mcp2515_transmit_tx0(&test_message);
		//printf("Joystick: %i \n\r", msg.joystick.axis.x);
		if(can_is_interrupt){
			can_is_interrupt = 0;
		}
		//Animation loop and sampling should be done independently. Important to have some time between samples. 
	
		_delay_ms(30);
	}
}