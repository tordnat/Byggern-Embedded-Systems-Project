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
#include "can.h"
#include "pong_game.h"

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
	
	

	gui_menu_current = gui_init();
	_delay_ms(30);
	while (1) {
		gui_goto_menu(&gui_menu_current);
		can_handle_inbox();
		game_update();
		_delay_ms(5);
	}
}