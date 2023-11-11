#include <stdint.h>

void adc_channel0_init(void);

//Returns "last converted" register
uint16_t adc_read(void);
