#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"

#include "sam.h"

#define LED1 23
#define LED2 19




int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PIOA -> PIO_WPMR &= ~PIO_WPMR_WPEN;	
    PMC->PMC_PCER0 |= PMC_PCER0_PID11;
    //PMC->PMC_PCER1 = PMC_PCER1_PID33;
    PIOA->PIO_PER |= PIO_PER_P19; //enable pin
    PIOA->PIO_OER |= PIO_OER_P19; //enable output
    PIOA->PIO_PER |= PIO_PER_P23; //enable pin
    PIOA->PIO_OER |= PIO_OER_P23; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P19;
    PIOA->PIO_SODR |= PIO_SODR_P23;

    PIOA->PIO_PER |= PIO_PER_P20; //enable pin
    PIOA->PIO_OER |= PIO_OER_P20; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P20;
    while (1) {

    }
    
}
