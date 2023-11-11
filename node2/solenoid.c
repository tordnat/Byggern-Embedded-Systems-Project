#include <stdio.h>
#include <stdarg.h>

#include "sam/sam3x/include/sam.h"

void solenoid_init(void) {
    PIOA->PIO_WPMR &= ~PIO_WPMR_WPEN; //Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID11; //Enable clock for PIOA

    PIOA->PIO_PER |= PIO_PER_P11; //enable pin
    PIOA->PIO_OER |= PIO_OER_P11; //enable output
}

void solenoid_on(void) {
    PIOA->PIO_SODR = PIO_SODR_P11; //Set output data
}
void solenoid_off(void) {
    PIOA->PIO_CODR = PIO_SODR_P11; //Clear output data
}