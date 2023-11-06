#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "sam/sam3x/include/sam.h"
#include "pwm.h"
#include "adc.h"

#define LED1 23
#define LED2 19
#define MCK 84000000
#define CAN_TX_MAILBOX_ID 0



uint8_t goal_scored() {
    uint16_t adc_val = adc_read();
    const uint16_t threshold = 1000;
    if(adc_val < threshold) {
        return 1;
    } else {
        return 0;
    }
}

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    pwm_init();
    adc_init();
    //adc_init_interrupt();
    uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((4-1) << CAN_BR_SJW_Pos) | ((7-1) << CAN_BR_PROPAG_Pos) | ((4-1) << CAN_BR_PHASE1_Pos) | ((4-1) << CAN_BR_PHASE2_Pos);

    //Tx = 0, RX = 1 or 2
    if(can_init_def_tx_rx_mb(can_br)) {
        printf("Can failed init\n\r");
    }
    printf("Everything inited\n\r");
    printf("ADC: %d\n\r", adc_read());
    printf("Enabled interrupt? %x\n\r", NVIC_GetEnableIRQ(ADC_IRQn));
    for(int i = 0; i < 10000000; i++) {__asm__("nop");}
    printf("Starting game\n\r");
    while (1) {
        //printf("ADC: %d\n\r", adc_read());
        if(!goal_scored()) {
            node2_msg data;
            data.goal = 1;
            
            CAN_MESSAGE msg;
            //encode_can_msg(&msg, data);
            encode_can_msg(&msg, data);
            printf("%d",can_send(&msg, CAN_TX_MAILBOX_ID));

            //Add delay here to register goal
            for(int i = 0; i < 10000000; i++) {__asm__("nop");}
        }
        
    }
}