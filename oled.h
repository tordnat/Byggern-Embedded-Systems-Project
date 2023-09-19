#pragma once
#include <avr/io.h>

void oled_write_c(uint8_t cmd);
void oled_write_d(uint8_t data);
void oled_init_program();
