#include "oled.h"

void oled_write_c(uint8_t cmd) {
	volatile char* oled_cmd_mem = (char*) 0x1200;
	oled_cmd_mem[0] = cmd;
}
void oled_write_d(uint8_t data) {
	volatile char* oled_data_mem = (char*) 0x1000;
	oled_data_mem[0] = data;
}
void oled_init_program() //This should be renamed to oled_init()
{
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

void oled_reset() {
	
}
void oled_home() {

}
void oled_goto_line(uint8_t line) {
	
}
void oled_clear_line(line) {
	
}
//Page == row: 0-7 e.g rows in the display, see p.33
void oled_pos(uint8_t row, uint8_t column) {
	oled_write_c(0xB0+row);
	oled_write_c(0x00+(column & (0b1111))); //Lower nibble
	oled_write_c(0x10+(column >> 4)); //Upper nibble
}
void oled_print(char*) {
	
}
void oled_set_brightness(uint8_t level) {
	
}
void OLED_print_arrow (uint8_t row , uint8_t col ){ //This should be placed in a draw.h file
	OLED_pos(row , col) ;
	OLED_write_data(0b00011000 ) ;
	OLED_write_data(0b00011000 ) ;
	OLED_write_data(0b01111110 ) ;
	OLED_write_data(0b00111100 ) ;
	OLED_write_data(0b00011000 ) ;
}
