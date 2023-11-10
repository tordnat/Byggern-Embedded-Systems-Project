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
#define LED1 23
#define LED2 19
#define MCK 84000000
#define CAN_TX_MAILBOX_ID 0

#define K_P 6//70
#define K_I (float) 100000//4000

#define SPEED_K_P 15
#define SPEED_K_I 0

#define TICK_SIZE_S 0.0001

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
int prev_servo_pos = 0;

int ref_pos = 0; //Input from node 1
int ref_speed = 0; //From pos regulator
double sum_e_pos = 0; 
double e_sum = 0;

int prev_encoder_pos = 0; //Prev encoder value
int current_pos = 0; //Current encoder value
CAN_MESSAGE can_msg;

/*
* current_pos: in encoder steps (0-4095)
* prev_pos: in encoder_step (0-4095)
* ref: in encoder_steps/S (0-4095)
*
*/
int regulator_speed(int current_pos, int prev_pos, int ref) {
    int32_t speed = (current_pos-prev_pos)/0.1; //Will be very large. Needs 32bit
    int32_t e = ref - speed;

    int32_t u = e*SPEED_K_P + TICK_SIZE_S*e_sum*SPEED_K_I + 0;

    e_sum += e;
    //printf("ref e u speed: %d %d %d %d\n\r",ref, e, u, speed);
    if((u < 700) && (u > 0)) {
        if(u < 2) {
            u = 0;
        } else {
            u = u+700;
        }
    }
    if((u > -700) && (u < 0)) {
        if(u > -2) {
            u = 0;
        }  else {
            u = u-700;
        }
    }
    motor_set_mapped_speed(u);
    return current_pos;
}
   
void regulator_pos(int ref) {
    int max_steps_encoder = 1407;
    int encoder_val = encoder_read();
    int32_t current_pos = map(encoder_val, 0, max_steps_encoder, 0, 100);
    int e = ref - current_pos;
    if((e < 0) && (e > -0)) {
        e = 0;
    }
    int u = K_P*e + K_I*sum_e_pos;

    //printf("PosU %d PosE %d\n\r", u, e);


    regulator_speed(encoder_val, prev_encoder_pos, u);
    prev_encoder_pos = encoder_val;
}

int main()
{


    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    NVIC_DisableIRQ(TC0_IRQn);
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

    motor_calibrate();
    current_pos = encoder_read();
    prev_encoder_pos = encoder_read();
    int mapped_ref_speed = 0;
    while(current_pos != prev_encoder_pos) {
        current_pos = encoder_read();
        prev_encoder_pos = encoder_read();
        dac_write(0);
    }
    while (1) {
    
       if(get_reg_tick()) {
        /*
            current_pos = encoder_read();
            ref_speed = get_node1_msg().joystickX;

            mapped_ref_speed = ref_speed;
            prev_encoder_pos = regulator_speed(current_pos, prev_encoder_pos, mapped_ref_speed);
        */
            ref_pos = (get_node1_msg().joystickX+100)/2;
            regulator_pos(ref_pos);
            
            set_reg_tick();
            int slider_pos = get_node1_msg().slider; //0-100
            pwm_servo_set_pos(slider_pos, &prev_servo_pos);

            if(adc_read() < 900) {
                //printf("Goal active\n\r");
            }
            if(get_node1_msg().btn) {
                solenoid_on();
            } else {
                solenoid_off();
            }
        
       }
        //printf("e %d sum %d \n\r", e, sum_e);
        //printf("Encoder %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c\n\r", BYTE_TO_BINARY(encoder_read()>>8), BYTE_TO_BINARY(encoder_read()));
        //printf("encoder ref timer %d %d %d\n\r", ref_pos, encoder_read(), timer_read());

    }
}


