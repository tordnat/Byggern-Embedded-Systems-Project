#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "sam/sam3x/include/sam.h"
#include "timer.h"

//not finished
int32_t reg_tick;
void set_reg_tick() {
    reg_tick = 0;
}
int32_t get_reg_tick() {
    return reg_tick;
}
void timer_init() {

    TC0->TC_WPMR  &= ~TC_WPMR_WPEN;
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
    TC0->TC_CHANNEL[0].TC_IDR = 0xFFFFFFFF;

    TC0->TC_CHANNEL[0].TC_SR;

    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK5 | TC_CMR_CPCTRG;//TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4;
    TC0->TC_CHANNEL[0].TC_RC = 32; //TICK_SIZE_S == 0.1ms
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;


    NVIC_EnableIRQ(TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
    while(!(TC0->TC_CHANNEL[0].TC_SR & TC_SR_CLKSTA)); // Wait for clock to stabilize

}

void delay_us(uint32_t us)
{
    //Wrong calc
	for (volatile uint32_t i = 0; i < ((84 / 10) * us); i++) {}
}
void TC0_Handler(void) {
    TC0->TC_CHANNEL[0].TC_SR;
    reg_tick = 1;
}