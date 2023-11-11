#include "adc.h"
#include "joystick.h"
#include "utilities.h"
#include <avr/builtins.h>
#include <avr/common.h>
#include <stdlib.h>
#include <stdio.h>

#define JOYSTICK_X_THRESHOLD 70
#define JOYSTICK_Y_THRESHOLD 70

// Center calibration globals, initial values 127
static int16_t joystick_x_center_calibration = 127;
static int16_t joystick_y_center_calibration = 127;


joystick_t get_joystick(void){
	joystick_t joystick;
	joystick.axis = joystick_get_position();
	joystick.button = joystick_get_button();
	return joystick;
}

position_t joystick_get_position(void){
	position_t pos;
	int16_t x_meas = (int16_t) adc_read(ADC_JOYSTICK_X_CHAN);
	int16_t y_meas = (int16_t) adc_read(ADC_JOYSTICK_Y_CHAN);
	pos.x = (int16_t) ((x_meas - joystick_x_center_calibration)/(float)joystick_x_center_calibration*100.0);
	pos.y = (int16_t) ((y_meas - joystick_y_center_calibration)/(float)joystick_y_center_calibration*100.0);
	if (pos.x > 100) pos.x = 100;
	if (pos.y > 100) pos.y = 100;
	return pos;
}

uint8_t joystick_get_button(void){
	return byte_to_percentage(adc_read(ADC_JOYSTICK_BUTTON_CHAN));
}

uint8_t slider_get_right(void){
	return byte_to_percentage(adc_read(ADC_SLIDER_R_CHAN));
}

void joystick_full_calibration(uint8_t samples){
	//printf("Calibrating Joystick center\n\r");
	joystick_calibrate_x(samples);
	joystick_calibrate_y(samples);
	//printf("X Center: %i\n\rY Center: %i\n\r", joystick_x_center_calibration, joystick_y_center_calibration);
}

uint8_t joystick_calibrate_x(uint8_t samples){
	uint8_t sample_array[samples];
	adc_read_to_array(ADC_JOYSTICK_X_CHAN, sample_array, samples);	
	joystick_x_center_calibration = array_avg(sample_array, samples);
	return joystick_x_center_calibration;
}

uint8_t joystick_calibrate_y(uint8_t samples){
	uint8_t sample_array[samples];
	adc_read_to_array(ADC_JOYSTICK_X_CHAN, sample_array, samples);
	joystick_y_center_calibration = array_avg(sample_array, samples);
	return joystick_y_center_calibration;
}
direction_t joystick_get_direction (position_t pos) {
	const int8_t threshold = 90;
	if(pos.x >= threshold && abs(pos.y) <= threshold) {
		return RIGHT;
	}
	if(pos.x <= -threshold && abs(pos.y) <= threshold) {
		return LEFT;
	}
	if(pos.y >= threshold && abs(pos.x) <= threshold) {
		return UP;
	}
	if(pos.y <= -threshold && abs(pos.x) <= threshold) {
		return DOWN;
	}
	return NEUTRAL;
}