#pragma once

//Initializes timer for PWM signal to ADC
void adc_init(void);
//Read sample (byte) from ADC
uint8_t adc_read(int channel);
//Read multiple samples to an array
uint8_t* adc_read_to_array(uint8_t channel, uint8_t* sample_array, uint8_t size);