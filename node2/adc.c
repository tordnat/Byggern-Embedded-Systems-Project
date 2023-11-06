#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

void adc_init() {
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;    //Disable write protection
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; //Enable clock for ADC module ADC PID == 37
    //PIOA->PIO_WPMR &= ~PIO_WPMR_WPEN; //Disable write protection for PIOA
    ADC->ADC_MR |= (ADC_MR_PRESCAL_Msk & 90) | ADC_MR_TRGEN_DIS;
    
    ADC->ADC_CHER |= ADC_CHER_CH0; //Enable channel 0 (CH0 => pin == PA2 == A7 på due)
}

uint16_t adc_read() {
    ADC->ADC_CR |= ADC_CR_START;
    return ADC->ADC_LCDR;
}

void adc_init_interrupt() {
    //Hits 700 at max

    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;    //Disable write protection
    PMC->PMC_PCER1 |= PMC_PCER1_PID37; //Enable clock for ADC module ADC PID == 37

    ADC->ADC_IER = ADC_IER_COMPE; //Eable interrupt
    ADC->ADC_IDR = ~(ADC_IDR_COMPE); //Disable all other interrupts

    ADC->ADC_CWR |= (ADC_CWR_LOWTHRES_Msk & 900);//ADC low threshold

    ADC->ADC_MR |= (ADC_MR_PRESCAL_Msk & 90) | ADC_MR_TRGEN_EN; //Prescaler and trigger select 
    ADC->ADC_MR |= (ADC_MR_TRGSEL_Msk & 0x0); // External trigger

    ADC->ADC_EMR = (ADC_EMR_TAG & 0x0) | (ADC_EMR_CMPFILTER_Msk & 0xFF) | (ADC_EMR_CMPALL & 0) | (ADC_EMR_CMPSEL_Msk & 0x00) | (ADC_EMR_CMPMODE_LOW);
    NVIC_SetPriority(ADC_IRQn, 0);
    NVIC_EnableIRQ(ADC_IRQn);
    ADC->ADC_CHER |= ADC_CHER_CH0; //Enable channel 0 (CH0 => pin == PA2)
    
    //printf("%x\n\r", ADC->ADC_OVER);

}

void ADC_Handler(void) {
   // ADC->ADC_IDR |= ADC_IDR_COMPE;
    //printf("%x\n\r", ADC->ADC_OVER);
    printf("test\n\r");
    //NVIC_ClearPendingIRQ(ID_ADC);
    //DC->ADC_ISR |= ADC_IER_COMPE; 
}