#pragma once

void timer_init();

//Not correct implementation of timer, but works for places where accuracy is not needed
void delay_us(uint32_t us);

//Sets reg_tick to 0, only TC0 should set it to 1
void set_reg_tick();

//Get reg_tick. Only one when TC0 has set it to one
int32_t get_reg_tick();