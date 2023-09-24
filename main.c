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
	gui_menu_item * gui_menu_root = gui_init();
	gui_draw_menu(gui_menu_root);
	char* debug = gui_menu_root->text;
	
	while (1)
	{
		//printf("%s \n\r", debug);
		//_delay_ms(1000);
		
		//oled_print_str("     ");
		//_delay_ms(9);
		//oled_print(c);
		//_delay_ms(9);
		
		//position pos = joystick_get_position();
		//printf("X: %i\r\nY: %i\r\n", pos.x, pos.y);
		//printf("X: %i\r\n RAW: %i \r\n", pos.x, adc_read(ADC_JOYSTICK_X_CHAN));

		//for(int i = 0; i < 255; i++) {
			//_delay_us(100);
			//printf("i: %i \n\r",i);
			//oled_write_d(i);
		//}
		//printf("Slider R: %i\n\r", slider_get_right());
		//printf("Slider L: %i\n\n\r", slider_get_left());
	}
}