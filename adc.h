#pragma once
#include <avr/io.h>
#include "usart.h"
#define ADC_ADDRESS 0x1800

enum joystick{
	y = 0, 
	x = 1, 
	b = 2
};

void adc_init(void);
uint8_t adc_read();
