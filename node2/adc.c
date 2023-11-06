#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

void adc_init() {
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;    //Disable write protection
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; //Enable clock for ADC module ADC PID == 37
    //PIOA->PIO_WPMR &= ~PIO_WPMR_WPEN; //Disable write protection for PIOA
    
    ADC->ADC_MR |= (ADC_MR_PRESCAL_Msk & 90) | ADC_MR_TRGEN_EN;
    ADC->ADC_MR |= (ADC_MR_TRGSEL_ADC_TRIG0);
    ADC->ADC_CHER |= ADC_CHER_CH0; //Enable channel 0 (CH0 => pin == PA2)
}

uint16_t adc_read() {
    ADC->ADC_CR |= ADC_CR_START;
    return ADC->ADC_LCDR;
}

void adc_init_interrupt() {
    //Hits 700 at max
    NVIC_ClearPendingIRQ(ID_ADC);

    
    ADC->ADC_IER |= ADC_IER_COMPE; 
    
    ADC->ADC_CWR |= (ADC_CWR_LOWTHRES_Msk & 900);//ADC low threshold

    ADC->ADC_EMR |= ADC_EMR_CMPMODE_LOW;
    ADC->ADC_EMR |= (ADC_EMR_CMPFILTER_Msk & 0xFF); //5 readings below threshold to trigger
    ADC->ADC_EMR &= ~(ADC_EMR_CMPSEL_Msk & 0xFF); //Select channel 0

    NVIC_EnableIRQ(ID_ADC);
}

void ADC_Handler(void) {
   // ADC->ADC_IDR |= ADC_IDR_COMPE;
    printf("test\n\r");
    NVIC_ClearPendingIRQ(ID_ADC);
    //DC->ADC_IER |= ADC_IER_COMPE; 
}