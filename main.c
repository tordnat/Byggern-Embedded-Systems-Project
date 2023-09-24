#include <avr/builtins.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"
#include "joystick.h"
#include "mem.h"
#include "oled.h"
#include "gui.h"

int main(void)
{
	usart_init(UBRR);
	exmem_init();
	adc_init();
	joystick_full_calibration(200);
	oled_init();
	oled_reset();

	
	
	//enum character c = arrow;
	int8_t selected_item = 0;
	direction joystick_dir;
	direction prev_joystick_dir = NEUTRAL;
	gui_menu_item * gui_menu_root = gui_init();
	gui_draw_menu(gui_menu_root, selected_item);

	while (1) {	
		//Animation loop and sampling should be done independently. Important to have some time between samples. 
		joystick_dir = joystick_get_direction(joystick_get_position());
		_delay_ms(10);

		if(joystick_dir == DOWN && prev_joystick_dir == NEUTRAL) {
			selected_item += 1;
			if(selected_item >= gui_menu_root->num_children) selected_item = gui_menu_root->num_children-1; //Fix this, not effective
			gui_draw_menu(gui_menu_root, selected_item); //Should not draw entire menu for each selection. Clear page should be enough
		}
		if(joystick_dir == UP && prev_joystick_dir == NEUTRAL) {
			selected_item -= 1;
			if(selected_item <= 0) selected_item = 0;			 //Fix this, not effective
			gui_draw_menu(gui_menu_root, selected_item);
		}
		prev_joystick_dir = joystick_dir;
		
		_delay_ms(10);

		
		
		//_delay_ms(10);
		
		//oled_print_str("     ");
		//_delay_ms(9);
		//oled_print(c);
		//_delay_ms(9);
		
		/*if(dir == DOWN) {
			printf("DOWN\n\r");
		}
		if(dir == UP) {
			printf("UP\n\r");
		}
		if(dir == NEUTRAL) {
			printf("NEUTRAL\n\r");
		}
		if(dir == RIGHT) {
			printf("RIGHT\n\r");
		}
		if(dir == LEFT) {
			printf("LEFT\n\r");
		}
		*/
	
	}
}