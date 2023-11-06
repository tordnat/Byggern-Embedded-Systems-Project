#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "sam/sam3x/include/sam.h"
#include "pwm.h"
#define LED1 23
#define LED2 19
#define MCK 84000000


void adc_init() {
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;

}

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    pwm_init();
    adc_init();

    uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((4-1) << CAN_BR_SJW_Pos) | ((7-1) << CAN_BR_PROPAG_Pos) | ((4-1) << CAN_BR_PHASE1_Pos) | ((4-1) << CAN_BR_PHASE2_Pos);

    //Tx = 1, RX = 2
    if(can_init_def_tx_rx_mb(can_br)) {
        printf("Can failed init\n\r");
    }

    while (1) {

    }
}