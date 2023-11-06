#pragma once
#include <avr/builtins.h>
#include <avr/common.h>

typedef struct {
	int8_t x;
	int8_t y;
} position_t;

uint8_t array_avg(uint8_t* array, uint8_t size);
uint8_t byte_to_percentage(uint8_t val);