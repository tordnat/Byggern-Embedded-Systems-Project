#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "sam/sam3x/include/sam.h"

void timer_init() {

    //TC0->TC_CHANNEL[0].TC_CCR |= 

    PWM->PWM_WPCR &= ~PIO_WPMR_WPEN; //Disable write protection for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; //Enable clock for PWM module PWM PID == 36
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN; //Disable write protect for PIOC

    PIOC->PIO_ABSR |= PIO_ABSR_P19; //Periphiral select B for pin 19
    PIOC->PIO_PDR |= PIO_PDR_P19; //Disable GPIO functionality


}