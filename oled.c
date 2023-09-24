#include "oled.h"
#include "fonts.h"

void oled_init() {
	//From SSD1780 datasheet
	oled_write_c(0xae); // display off
	oled_write_c(0xa1); //segment remap
	oled_write_c(0xda); //common pads hardware: alternative
	oled_write_c(0x12);
	oled_write_c(0xc8); //common output scan direction:com63~com0
	oled_write_c(0xa8); //multiplex ration mode:63
	oled_write_c(0x3f);
	oled_write_c(0xd5); //display divide ratio/osc. freq. mode
	oled_write_c(0x80);
	oled_write_c(0x81); //contrast control
	oled_write_c(0x50);
	oled_write_c(0xd9); //set pre-charge period
	oled_write_c(0x21);
	oled_write_c(0x20); //Set Memory Addressing Mode
	oled_write_c(0x02);
	oled_write_c(0xdb); //VCOM deselect level mode
	oled_write_c(0x30);
	oled_write_c(0xad); //master configuration
	oled_write_c(0x00);
	oled_write_c(0xa4); //out follows RAM content
	oled_write_c(0xa6); //set normal display
	oled_write_c(0xaf); // display on
}
void oled_write_c(uint8_t cmd) {
	volatile char* oled_cmd_mem = (char*) 0x1000;
	oled_cmd_mem[0] = cmd;
	
}
void oled_write_d(uint8_t data) {
	volatile char* oled_data_mem = (char*) 0x1200;
	#if LIGHT_MODE
		oled_data_mem[0] = ~data;
		return;
	#endif
	oled_data_mem[0] = data;
}
void oled_write_d_inv(uint8_t data) {
	volatile char* oled_data_mem = (char*) 0x1200;
	#if LIGHT_MODE
		oled_data_mem[0] = data;
		return;
	#endif
	oled_data_mem[0] = ~data;
}
void oled_goto_page(uint8_t page) {
	oled_write_c(0xB0+page);
}
void oled_clear_page(uint8_t page) {
	oled_pos(page, 0);
	for(int j = 0; j < NUM_COLS; j++) {
		oled_write_d(0x0);
	}
}
void oled_reset() {
	oled_pos(0, 0);
	for(int i = 0; i < NUM_PAGES; i++) {
		oled_clear_page(i);
	}
	oled_pos(0, 0);
}
void oled_pos(uint8_t page, uint8_t column) {
	oled_write_c(0xB0+page);
	oled_write_c(0x00+(column & (0b1111))); //Lower nibble
	oled_write_c(0x10+(column >> 4)); //Upper nibble
}
void oled_print(uint8_t c) {
	c -= 32; //This stupid, why not ascii?
	for(int i = 0; i < 8; i++) {
		oled_write_d(pgm_read_byte(&font8[c][i]));
	}
}
void oled_print_str(char *str) {
	uint8_t *c = (uint8_t*) str;
	while(*c) {
		oled_print(*c++);
	}
}
void oled_print_arrow(uint8_t page , uint8_t col) { //This should be placed in a draw.h file
	oled_pos(page, col) ;
	oled_write_d(0b00011000);
	oled_write_d(0b00011000);
	oled_write_d(0b01111110);
	oled_write_d(0b00111100);
	oled_write_d(0b00011000);
}