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
#define K_P 70
#define K_I (float) 2000

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


int ref_pos = 50;
double sum_e = 0;

void regulator(int ref) {
    int max_steps_encoder = 1407;
    int encoder_val = encoder_read();
    int8_t current_pos = map(encoder_val, 0, max_steps_encoder, 0, 100);
    int e = current_pos - ref;
    sum_e += e;
    int u = K_P*e + 0.0001*K_I*sum_e;
    //printf("e u e_sum %d %d %d\n\r", e, u, (int) sum_e);
    motor_set_mapped_speed(u);
}

int prev_servo_pos = 0;
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
    timer_init();
    //adc_init_interrupt();
    uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((4-1) << CAN_BR_SJW_Pos) | ((7-1) << CAN_BR_PROPAG_Pos) | ((4-1) << CAN_BR_PHASE1_Pos) | ((4-1) << CAN_BR_PHASE2_Pos);
    //Tx = 0, RX = 1 or 2
    if(can_init_def_tx_rx_mb(can_br)) {
        printf("Can failed init\n\r");
    }
    printf("Everything inited\n\r");
    //printf("ADC: %d\n\r", adc_read());
    //printf("Enabled interrupt? %x\n\r", NVIC_GetEnableIRQ(ADC_IRQn));
    printf("Starting game\n\r");

    motor_calibrate();
    
    while (1) {
        //Return 
       if(get_reg_tick()) {
            ref_pos = (get_node1_msg().joystickX+100)/2;
            regulator(ref_pos);
            NVIC_DisableIRQ(TC0_IRQn);
            set_reg_tick();
            NVIC_EnableIRQ(TC0_IRQn);
            pwm_servo_set_pos((get_node1_msg().slider-50)*2, prev_servo_pos);
            prev_servo_pos = (((int)get_node1_msg().slider)-50)*2;
            if(adc_read() < 900) {
                printf("Goal active\n\r");
            }
            if(get_node1_msg().btn) {
                solenoid_on();
            } else {
                solenoid_off();
            }
       }
       
        //printf("ADC: %d\n\r", adc_read());
        //if(!goal_scored()) {
            //node2_msg data;
            //data.goal = 1;
            //printf("timer %d\n\r", timer_read());
            
            //CAN_MESSAGE msg;
            //encode_can_msg(&msg, data);
            //encode_can_msg(&msg, data);
            //printf("%d",can_send(&msg, CAN_TX_MAILBOX_ID));
            /*
            delay_us(1000000);
            solenoid_on();
            delay_us(1000000);
            //Add delay here to register goal
            solenoid_off();
            */
            //motor_set_speed(1, 1500);
            //delay(100000);
            //motor_set_speed(0, 1500);
            /*
            ref_pos = get_node1_msg().slider;
            delay_us(100);
            int16_t pos = encoder_read();
            //printf("Pos encoder: %d\n\r", pos);
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
            */
            
            //if(e == 0) {
            //    motor_set_mapped_speed(0);
            //}
            //printf("e %d sum %d \n\r", e, sum_e);
            //printf("Encoder %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c\n\r", BYTE_TO_BINARY(encoder_read()>>8), BYTE_TO_BINARY(encoder_read()));
            //printf("encoder ref timer %d %d %d\n\r", ref_pos, encoder_read(), timer_read());

        //}
    }
}


