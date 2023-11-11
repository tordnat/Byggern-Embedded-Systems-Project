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
#include "utils.h"
#include "servo.h"
#include "regulator.h"

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

CAN_MESSAGE can_msg;


int32_t goals = 0;  
int32_t ref_pos = 0; //Input from node 1
int32_t prev_encoder_pos = 0; //Prev encoder value
int32_t current_encoder_pos = 0; //Current encoder value

int main() {
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    servo_init();
    dac_channel1_init();
    adc_channel0_init();
    solenoid_init();
    motor_init();
    encoder_init();
    timer_init();
    uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((4-1) << CAN_BR_SJW_Pos) | ((7-1) << CAN_BR_PROPAG_Pos) | ((4-1) << CAN_BR_PHASE1_Pos) | ((4-1) << CAN_BR_PHASE2_Pos);
    if(can_init_def_tx_rx_mb(can_br)) {
        printf("Can failed init\n\r");
    }
    printf("Everything inited\n\r");
    motor_calibrate();
    
    /*
    current_encoder_pos = encoder_read();
    prev_encoder_pos = encoder_read();
    while(current_encoder_pos != prev_encoder_pos) {
        current_encoder_pos = encoder_read();
        prev_encoder_pos = encoder_read();
        dac_write(0);
    }
    */
    while (1) {
       if(get_reg_tick()) {
            ref_pos = (get_node1_msg().joystickX+100)/2;

            regulator_pos(ref_pos, &prev_encoder_pos);

            set_reg_tick();
            int32_t slider_pos = get_node1_msg().slider; //0-100
            servo_set_pos(slider_pos);
            if(adc_read() < 900) {
                goals += 1; //send can message here
                motor_set_speed(1, 0);
                printf("%d\n\r", adc_read());
                while(get_node1_msg().btn); //Goal scored, game must be reset
            }
            if(get_node1_msg().btn) {
                solenoid_on();
            } else {
                solenoid_off();
            }
       }
        //printf("e %d sum %d \n\r", e, sum_e);
        //printf("encoder ref timer %d %d %d\n\r", ref_pos, encoder_read(), timer_read());
    }
}


