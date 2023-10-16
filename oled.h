#pragma once
#define LIGHT_MODE 0
#define NUM_PAGES 8
#define NUM_COLS 128

/*
Number of pages/rows:   8 (0-7)
Number of columns:    128 (0-127)

*/

enum character {
	char_arrow = 127,
};
void oled_write_c(uint8_t cmd);
void oled_write_d(uint8_t data);
void oled_init();
/*
	Clear entire display
*/
void oled_reset();
/*
	Goto page without moving column pointed to
*/
void oled_goto_page(uint8_t line);
/*
	Clears entire page and resets pointer to start of page
*/
void oled_clear_page(uint8_t line);
void oled_pos(uint8_t page, uint8_t column);
void oled_print(uint8_t c);
void oled_set_brightness(uint8_t level);
void oled_write_d_inv(uint8_t data);
void oled_print_str(char *str);
