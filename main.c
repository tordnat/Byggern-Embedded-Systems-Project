#include <avr/builtins.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"
#include "joystick.h"
#include "mem.h"
#include "oled.h"
#include "gui.h"
#include "spi.h"

int main(void)
{
	_delay_ms(10);
	usart_init(UBRR);
	exmem_init();
	adc_init();
	spi_master_init();
	
	joystick_full_calibration(200);
	oled_init();
	_delay_ms(10);
	oled_reset();
	_delay_ms(10);
	
	
	//enum character c = arrow;
	int8_t selected_item = 0;
	direction joystick_dir;
	direction prev_joystick_dir = NEUTRAL;
	gui_menu_item * gui_menu_current = gui_init();
	gui_draw_menu(gui_menu_current, selected_item);

	while (1) {
		//SPI Test
		spi_transmit('a');
		spi_read();
		//Animation loop and sampling should be done independently. Important to have some time between samples. 
		position joystick_pos = joystick_get_position();
		joystick_dir = joystick_get_direction(joystick_pos);
		_delay_ms(10);

		//gui_goto_menu(gui_menu_current, selected_item, &joystick_dir, &prev_joystick_dir);

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
		
		_delay_ms(10);

		
		
		//_delay_ms(10);
		
		//oled_print_str("     ");
		//_delay_ms(9);
		//oled_print(c);
		//_delay_ms(9);
		printf("x: %i\n\r", joystick_pos.x);
		if(joystick_dir == DOWN) {
			printf("DOWN\n\r");
		}
		if(joystick_dir == UP) {
			printf("UP\n\r");
		}
		if(joystick_dir == NEUTRAL) {
			printf("NEUTRAL\n\r");
		}
		if(joystick_dir == RIGHT) {
			printf("RIGHT\n\r");
		}
		if(joystick_dir == LEFT) {
			printf("LEFT\n\r");
		}
		
	
	}
}