#pragma once
#include <avr/builtins.h>
#include <avr/common.h>

typedef struct {
	int x;
	int y;
} position;

typedef enum{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} direction;

uint8_t get_slider_right(void);
uint8_t get_slider_left(void);
position joystick_get_position(void);
void joystick_full_calibration(uint8_t samples);
uint8_t joystick_calibrate_x(uint8_t samples);
uint8_t joystick_calibrate_y(uint8_t samples);