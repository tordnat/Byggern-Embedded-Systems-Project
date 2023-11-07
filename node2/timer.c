#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "sam/sam3x/include/sam.h"
#include "timer.h"

//not finished
void timer_init() {
    TC0->TC_WPMR  &= ~TC_WPMR_WPEN; //Write protect
    
    TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;
    TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK4;

    PMC->PMC_PCER1 |= PMC_PCER1_PID36; //Enable clock for PWM module PWM PID == 36
}

int timer_read() {
    return TC0->TC_CHANNEL[0].TC_CV;
}
