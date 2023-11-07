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
#include "solenoid.h"
#include "dac.h"
#include "timer.h"
#include "motor.h"

#define LED1 23
#define LED2 19
#define MCK 84000000
#define CAN_TX_MAILBOX_ID 0

int map(int val_to_map, int in_min, int in_max, int out_min, int out_max) {
  return (val_to_map - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t goal_scored() {
    uint16_t adc_val = adc_read();
    const uint16_t threshold = 1000;
    if(adc_val < threshold) {
        return 1;
    } else {
        return 0;
    }
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')


int ref_pos = 90;
int sum_e = 0;
int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    pwm_init();
    adc_init();
    solenoid_init();
    dac_init();
    motor_init();
    encoder_init();
    //adc_init_interrupt();
    uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((4-1) << CAN_BR_SJW_Pos) | ((7-1) << CAN_BR_PROPAG_Pos) | ((4-1) << CAN_BR_PHASE1_Pos) | ((4-1) << CAN_BR_PHASE2_Pos);
    //Tx = 0, RX = 1 or 2
    if(can_init_def_tx_rx_mb(can_br)) {
        printf("Can failed init\n\r");
    }
    printf("Everything inited\n\r");
    printf("ADC: %d\n\r", adc_read());
    //printf("Enabled interrupt? %x\n\r", NVIC_GetEnableIRQ(ADC_IRQn));
    printf("Starting game\n\r");

    motor_calibrate();
    
    while (1) {
        //printf("ADC: %d\n\r", adc_read());
        //if(!goal_scored()) {
            node2_msg data;
            data.goal = 1;
            //printf("timer %d\n\r", timer_read());
            
            CAN_MESSAGE msg;
            //encode_can_msg(&msg, data);
            //encode_can_msg(&msg, data);
            //printf("%d",can_send(&msg, CAN_TX_MAILBOX_ID));

            //solenoid_on();
            //Add delay here to register goal
            //solenoid_off();
            
            //motor_set_speed(1, 1500);
            //delay(100000);
            //motor_set_speed(0, 1500);
            delay_us(10);
            int16_t pos = encoder_read();
            int8_t mapped_pos = map(pos, 0, 1407, 0, 100);
            int e = ref_pos - mapped_pos;
            if((sum_e < 3000) && (sum_e > -3000)) {
                sum_e += e/3;
                if(e < 3) {
                    sum_e += e;
                }
            }
            
            motor_set_mapped_speed(e*20+sum_e);
            //Total range == 1407 steps of encoder
            if(e == 0) {
                motor_set_mapped_speed(0);
            }
            printf("e %d sum %d \n\r", e, sum_e);
            //printf("Leading text %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c\n\r", BYTE_TO_BINARY(encoder_read()>>8), BYTE_TO_BINARY(encoder_read()));
            //printf("encoder %d\n\r", encoder_read());
        //}
    }
}


