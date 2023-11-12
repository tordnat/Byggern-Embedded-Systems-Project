#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "sam/sam3x/include/sam.h"
#include "timer.h"

int32_t reg_tick = 0;

void set_reg_tick() {
    reg_tick = 0;
}
int32_t get_reg_tick() {
    return reg_tick;
}
void timer_init() {

    TC0->TC_WPMR  &= ~TC_WPMR_WPEN;
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS; //Disable clock
    TC0->TC_CHANNEL[0].TC_IDR = 0xFFFFFFFF; //Disable all interrupts

    TC0->TC_CHANNEL[0].TC_SR; //Read status reg to clear

    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK5 | TC_CMR_CPCTRG;
    TC0->TC_CHANNEL[0].TC_RC = 32; //TICK_SIZE_S == 0.1ms
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;


    NVIC_EnableIRQ(TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; //Enable clock and software trigger
    while(!(TC0->TC_CHANNEL[0].TC_SR & TC_SR_CLKSTA)); //Wait for stabilize
}

void delay_us(uint32_t us)
{
    //Not correct implementation of timer, but works for places where accuracy is not needed
	for (volatile uint32_t i = 0; i < ((84 / 10) * us); i++) {}
}
void TC0_Handler(void) {
    TC0->TC_CHANNEL[0].TC_SR; //Clear status register
    reg_tick = 1;
}