#pragma once
#include <avr/io.h>
#include "usart.h"
#define ADC_ADDRESS 0x1400

#define ADC_JOYSTICK_Y_CHAN 0
#define ADC_JOYSTICK_X_CHAN 1
#define ADC_SLIDER_R_CHAN 2
#define ADC_SLIDER_L_CHAN 3

void adc_init(void);
uint8_t adc_read();
uint8_t* adc_sample_to_array(uint8_t channel, uint8_t* sample_array, uint8_t size);