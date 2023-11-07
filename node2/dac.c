#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "sam/sam3x/include/sam.h"

void dac_init() {

    DACC->DACC_WPMR &= ~PIO_WPMR_WPEN; //Disable write protection for DAC
    PMC->PMC_PCER1 |= PMC_PCER1_PID38; //Enable clock for DACC module DACC PID == 38

    DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_TRGEN_DIS | DACC_MR_WORD_HALF; //Disable external and sel CHAN 0 and only transfer 16bits
    DACC->DACC_CHER |= DACC_CHER_CH1; //| DACC_CHER_CH0; //Enable channel 0

    //PIOB->PIO_PDR |= PIO_PDR_P15; //Disable GPIO functionality

    
}

void dac_write(uint16_t val) {
    if(val > 4095) return; //Invalid, only 12 bits
    DACC->DACC_CDR = val;
}