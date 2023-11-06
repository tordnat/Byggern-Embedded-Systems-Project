#pragma once
#include <avr/builtins.h>
#include <avr/common.h>

#define ADC_JOYSTICK_Y_CHAN 0
#define ADC_JOYSTICK_X_CHAN 1
#define ADC_SLIDER_R_CHAN 2
#define ADC_JOYSTICK_BUTTON_CHAN 3

//Initializes timer for PWM signal to ADC
void adc_init(void);
//Read sample (byte) from ADC
uint8_t adc_read(int channel);
//Read multiple samples to an array
uint8_t* adc_read_to_array(uint8_t channel, uint8_t* sample_array, uint8_t size);