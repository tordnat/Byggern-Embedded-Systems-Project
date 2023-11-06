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
	int8_t selected_item = 0;
	static can_message_t test_message;
	static node1_msg_t msg;

	direction_t joystick_dir;
	direction_t prev_joystick_dir = NEUTRAL;
	gui_menu_item * gui_menu_current = gui_init();
	gui_draw_menu(gui_menu_current, selected_item);
	can_message_t* buffer = get_can_buffer_ptr();
	while (1) {
	msg = get_node1_msg();
	can_encode_node1_msg(&msg, &test_message);
	mcp2515_transmit_tx0(&test_message);
	_delay_ms(1);
	uint8_t value = buffer->data[0];
	if(can_is_interrupt){
		printf("Interupt!: %i \n\r", mcp2515_interrupt_flags);
		printf("Receive: %i \n\r", value);
		can_is_interrupt = 0;	
	}

	

	
		/*
		//Animation loop and sampling should be done independently. Important to have some time between samples. 
		position joystick_pos = joystick_get_position();
		joystick_dir = joystick_get_direction(joystick_pos);
		_delay_ms(30);
		//gui_goto_menu(&gui_menu_current, &selected_item, &joystick_dir, &prev_joystick_dir);
		{
			if(joystick_dir == DOWN && prev_joystick_dir == NEUTRAL) {
				selected_item += 1;
				if(selected_item >= gui_menu_current->num_children) selected_item = gui_menu_current->num_children-1; //Fix this, not effective
				gui_draw_menu(gui_menu_current, selected_item); //Should not draw entire menu for each selection. Clear page should be enough
			}
			if(joystick_dir == UP && prev_joystick_dir == NEUTRAL) {
				selected_item -= 1;
				if(selected_item <= 0) selected_item = 0;			 //Fix this, not effective
				gui_draw_menu(gui_menu_current, selected_item);
			}
			//Go to selected child
			if(joystick_dir == RIGHT && prev_joystick_dir == NEUTRAL) {
				if(gui_menu_current->num_children != 0) { //To ensure next if does not select from an empty array
					if(gui_menu_current->children[selected_item]->num_children != 0) {
						gui_menu_current = gui_menu_current->children[selected_item];
						selected_item = 0;
						gui_draw_menu(gui_menu_current, selected_item);
					}
				}
			}
			//Go to parent
			if(joystick_dir == LEFT && prev_joystick_dir == NEUTRAL) {
				if(gui_menu_current->parent != NULL){ //Check for no parent
					gui_menu_current = gui_menu_current->parent;
					gui_draw_menu(gui_menu_current, selected_item);
					selected_item = 0;
				}
			}
			prev_joystick_dir = joystick_dir;
		}
		_delay_ms(50);

		
		
		
		
		*/	
	}
}