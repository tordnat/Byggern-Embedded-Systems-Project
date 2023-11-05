#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "sam/sam3x/include/sam.h"

#define LED1 23
#define LED2 19
#define MCK 84000000

#define PWM_PERIOD 1641
#define PWM_SERVO_DUTY_MIN 82
#define PWM_SERVO_DUTY_MAX 164
#define PWM_SERVO_DUTY_MID 123
void pwm_init(void) {
    PWM->PWM_WPCR &= ~PIO_WPMR_WPEN; //Disable write protection for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; //Enable clock for PWM module PWM PID == 36
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN; //Disable write protect for PIOC

    PIOC->PIO_ABSR |= PIO_ABSR_P19; //Periphiral select B for pin 19
    PIOC->PIO_PDR |= PIO_PDR_P19; //Disable GPIO functionality

    PWM->PWM_CLK |= (1 << PWM_CLK_DIVA_Pos); //Enable CLKA
    PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0; //sync mode
    PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL; //Use CLKA 
    PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_PERIOD; //Period = (20ms*84Mhz)/1024
    PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_SERVO_DUTY_MID; // 1.5 ms 

    PWM->PWM_ENA |= PWM_ENA_CHID5; //Enable channel 5
}
//Takes in raw value to set in register
static void pwm_set_duty(uint32_t duty) {
    if(duty < PWM_PERIOD) {
        PWM->PWM_CH_NUM[5].PWM_CDTYUPD = duty;
        return;
    }
    else {
        printf("Duty Cycle higher than period\n\r");
        return; //add error code
    }
}
//Takes in position between -100 and 100
int8_t global_prev_pos = 0;
void pwm_servo_set_pos(int8_t pos) {
    pos = -pos; // To map joystick direction to intuitive servo direction
    //Invalid position
    if((pos < -100) || (pos > 100)) {
        printf("Pos outside valid range\n\r");
        return; //add error code
    }
    //Position to duty cycle calulation
    if(pos == global_prev_pos) {
        return;
    }
    int duty = (((pos - (-100)) * (PWM_SERVO_DUTY_MAX - PWM_SERVO_DUTY_MIN))/(100 - (-100))) + PWM_SERVO_DUTY_MIN;

    //printf("Pos: %d \n\rDuty Cycle value: %d\n\r", pos, duty);
    if((duty <= PWM_SERVO_DUTY_MAX) && (duty >= PWM_SERVO_DUTY_MIN)) {
        pwm_set_duty(duty);
        return; //add error code
    }
    else {
        printf("Calc error\n\r");
    }
}
void adc_init() {
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;

}

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    printf("Hello World\n\r");
    
    PIOA->PIO_WPMR &= ~PIO_WPMR_WPEN;	//Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID11; //Enable clock for PIOA

    //PMC->PMC_PCER1 = PMC_PCER1_PID33;
    PIOA->PIO_PER |= PIO_PER_P19; //enable pin
    PIOA->PIO_OER |= PIO_OER_P19; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P19; //Set output data

    PIOA->PIO_PER |= PIO_PER_P20; //enable pin
    PIOA->PIO_OER |= PIO_OER_P20; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P20; //Set output data

    PIOA->PIO_CODR = PIO_SODR_P20 | PIO_SODR_P19;

    //4TQ SJW, 7TQ TPROP, 4TQ PHASE1, 4TQ PHASE2
    uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((4-1) << CAN_BR_SJW_Pos) | ((7-1) << CAN_BR_PROPAG_Pos) | ((4-1) << CAN_BR_PHASE1_Pos) | ((4-1) << CAN_BR_PHASE2_Pos);
    //uint32_t can_br = ((42-1) << CAN_BR_BRP_Pos) | ((2-1) << CAN_BR_SJW_Pos) | ((2-1) << CAN_BR_PROPAG_Pos) | ((8-1) << CAN_BR_PHASE1_Pos) | ((7-1) << CAN_BR_PHASE2_Pos);

    //Tx = 1, RX = 2
    if(can_init_def_tx_rx_mb(can_br)) {
        printf("Can failed init\n\r");
    }
    //CAN_MESSAGE *recieved_message;
    //can_receive(recieved_message, 1);

    
    CAN_MESSAGE msg;
    msg.data[0] = 'u';
    msg.data_length = 1;
    msg.id = 40;

    pwm_init();
    adc_init();
    printf("Finished init\n\r");

    for(int i = 0; i < 1000000; i++) {__asm__("nop");}
    pwm_servo_set_pos(-100);
    printf("-100\n\r");
    for(int i = 0; i < 10000000; i++) {__asm__("nop");}
    pwm_servo_set_pos(0);
    printf("0\n\r");
    for(int i = 0; i < 10000000; i++) {__asm__("nop");}
    pwm_servo_set_pos(100);
    printf("100\n\r");

    while (1) {
        /*
            for(int j = -100; j < 101; j++) {
                pwm_servo_set_pos(j);
                

                printf("Attempting to send can message\n\r");

                int err = can_send(&msg, 0);
                if(err) {
                    printf("Can failed to send, error: %d \n\r", err);
                } else {
                    printf("Succesfully sent\n\r");
                }
                

            }
        */   
    }
    //BAUD = T_q * bitrate
}