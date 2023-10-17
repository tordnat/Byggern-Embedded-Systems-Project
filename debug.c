#include <stdio.h>
#include "debug.h"
#include "adc.h"
#include "joystick.h"
#include "oled.h"

void debug_print_joystick_directions(direction joystick_dir) {
	oled_print_str("     ");
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