#include <avr/builtins.h>
#include <avr/common.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"
#include "joystick.h"
#include "mem.h"
#include "oled.h"

int main(void)
{
	_delay_ms(1000);
	usart_init(UBRR);
	exmem_init();
	adc_init();
	joystick_full_calibration(200);
	oled_init();
	
	oled_reset();
	oled_print_arrow(0, 0);
	oled_print_arrow(1, 5);
	oled_print_arrow(2, 10);
	oled_print_arrow(3, 15);
	oled_print_arrow(4, 20);
	oled_print_arrow(5, 25);
	oled_print_arrow(6, 30);
	oled_print_arrow(7, 35);
	oled_pos(0,0);
	oled_print('H');
	oled_print('e');
	oled_print('i');

	
	while (1)
	{
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