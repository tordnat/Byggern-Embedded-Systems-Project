#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

void adc_channel0_init(void) {
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;    //Disable write protection
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; //Enable clock for ADC module ADC PID == 37
    ADC->ADC_MR |= (ADC_MR_PRESCAL_Msk & 90) | ADC_MR_TRGEN_DIS; //Set prescaler and Enable software only trigger
    ADC->ADC_CHER |= ADC_CHER_CH0; //Enable channel 0 (CH0 => pin == PA2 == A7 on due)
}

uint16_t adc_read(void) {
    ADC->ADC_CR |= ADC_CR_START;
    return ADC->ADC_LCDR;
}