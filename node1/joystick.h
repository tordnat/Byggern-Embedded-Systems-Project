#pragma once
#include <avr/common.h>
#include <avr/builtins.h>
#include "utilities.h"


typedef struct {
	position_t axis;
	uint8_t button;
} joystick_t;

typedef enum{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL
} direction_t;

uint8_t slider_get_right(void);
uint8_t joystick_get_button(void);
position_t joystick_get_position(void);
joystick_t get_joystick(void);
void joystick_full_calibration(uint8_t samples);
uint8_t joystick_calibrate_x(uint8_t samples);
uint8_t joystick_calibrate_y(uint8_t samples);
direction_t joystick_get_direction(position_t pos);