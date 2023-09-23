#include "oled.h"
#include "fonts.h"

void oled_write_c(uint8_t cmd) {
	volatile char* oled_cmd_mem = (char*) 0x1000;
	oled_cmd_mem[0] = cmd;
}
void oled_write_d(uint8_t data) {
	volatile char* oled_data_mem = (char*) 0x1200;
	oled_data_mem[0] = data;
}
void oled_init()
{
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
//Not tested

void oled_clear_line(uint8_t line) {
	oled_pos(line, 0);
	for(int j = 0; j < NUM_COLS; j++) {
		oled_write_d(0x0);
	}
}
void oled_reset() {
	oled_pos(0, 0);
	for(int i = 0; i < NUM_PAGES; i++) {
		oled_clear_line(i);
	}
	oled_pos(0, 0);
}
void oled_home() {

}
void oled_goto_line(uint8_t line) {
	
}

//Page == row: 0-7 e.g rows in the display, see p.33
void oled_pos(uint8_t row, uint8_t column) {
	oled_write_c(0xB0+row);
	oled_write_c(0x00+(column & (0b1111))); //Lower nibble
	oled_write_c(0x10+(column >> 4)); //Upper nibble
}
void oled_print(char c) {
	c -= 32; //This stupid, why not ascii?
	oled_write_d(pgm_read_byte(&font8[c][0]));
	oled_write_d(pgm_read_byte(&font8[c][1]));
	oled_write_d(pgm_read_byte(&font8[c][2]));
	oled_write_d(pgm_read_byte(&font8[c][3]));
	oled_write_d(pgm_read_byte(&font8[c][4]));
	oled_write_d(pgm_read_byte(&font8[c][5]));
	oled_write_d(pgm_read_byte(&font8[c][6]));
	oled_write_d(pgm_read_byte(&font8[c][7]));
}

void oled_print_arrow(uint8_t row , uint8_t col){ //This should be placed in a draw.h file
	oled_pos(row, col) ;
	oled_write_d(0b00011000);
	oled_write_d(0b00011000);
	oled_write_d(0b01111110);
	oled_write_d(0b00111100);
	oled_write_d(0b00011000);
}