#pragma once
#include <avr/io.h>
#include "usart.h"
#define ADC_ADDRESS 0x1400

#define AXIS_Y_ADC_CHAN 0
#define AXIS_X_ADC_CHAN 1
#define SLIDER_R_ADC_CHAN 2
#define SLIDER_L_ADC_CHAN 3

void adc_init(void);
uint8_t adc_read();
