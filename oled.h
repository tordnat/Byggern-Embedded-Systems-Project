#pragma once
#include <avr/io.h>

#define NUM_PAGES 8
#define NUM_COLS 128

void oled_write_c(uint8_t cmd);
void oled_write_d(uint8_t data);
void oled_init();
void oled_reset();
void oled_home();
void oled_goto_line(uint8_t line);
void oled_clear_line(uint8_t line);
//Page == row: 0-7 e.g rows in the display, see p.33
void oled_pos(uint8_t row, uint8_t column);
void oled_print(char c);
void oled_set_brightness(uint8_t level);
void oled_print_arrow(uint8_t row , uint8_t col);