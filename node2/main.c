#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "sam/sam3x/include/sam.h"


#define LED1 23
#define LED2 19




#define PWM_PERIOD 1641
#define PWM_SERVO_DUTY_MIN 82
#define PWM_SERVO_DUTY_MAX 164

void pwm_init(void) {
    PWM->PWM_WPCR &= ~PIO_WPMR_WPEN; //Disable write protection for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; //Enable clock for PWM module PWM PID == 36
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN; //Disable write protect for PIOC

    PIOC->PIO_ABSR |= PIO_ABSR_P19; //Periphiral select B for pin 19

    PWM->PWM_CLK |= (1 << PWM_CLK_DIVA_Pos); //Enable CLKA
    PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0; //sync mode
    PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL; //Use CLKA 
    PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_PERIOD; //Period = (20ms*84Mhz)/1024
    PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_SERVO_DUTY_MAX - PWM_SERVO_DUTY_MIN; // 1.5 ms 

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
void pwm_servo_set_pos(int pos) {
    //Invalid position
    if((pos < -100) || (pos > 100)) {
        return; //add error code
    }
    //Position to duty cycle calulation
    // X: pos
    //
    int duty = (((pos - (-100)) * (PWM_SERVO_DUTY_MAX - PWM_SERVO_DUTY_MIN))/(100 - (-100))) + PWM_SERVO_DUTY_MIN;

    printf("Pos: %d \n\rDuty Cycle value: %d\n\r", pos, duty);
    if((duty <= PWM_SERVO_DUTY_MAX) && (duty >= PWM_SERVO_DUTY_MIN)) {
        pwm_set_duty(duty);
        return; //add error code
    }
    else {
        printf("Calc error\n\r");
    }
}

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PIOA -> PIO_WPMR &= ~PIO_WPMR_WPEN;	
    PMC->PMC_PCER0 |= PMC_PCER0_PID11;
    //PMC->PMC_PCER1 = PMC_PCER1_PID33;
    PIOA->PIO_PER |= PIO_PER_P19; //enable pin
    PIOA->PIO_OER |= PIO_OER_P19; //enable output
    PIOA->PIO_PER |= PIO_PER_P23; //enable pin
    PIOA->PIO_OER |= PIO_OER_P23; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P19;
    PIOA->PIO_SODR |= PIO_SODR_P23;

    PIOA->PIO_PER |= PIO_PER_P20; //enable pin
    PIOA->PIO_OER |= PIO_OER_P20; //enable output

    PIOA->PIO_SODR |= PIO_SODR_P20; //Set output data

    PIOA->PIO_CODR = PIO_SODR_P20 | PIO_SODR_P19;

    //CAN br = 
    //can_init_def_tx_rx_mb(); //Tx = 1, RX = 2
    //CAN_MESSAGE *recieved_message;
    //can_receive(recieved_message, 1);

    
    /* p.533 for setup
    *  Pin 45 == PC19
    *  PC19 uses signal PWMH5
    */
    
    pwm_init();
    printf("Finished init\n\r");

    while (1) {
        

            for(int j = -100; j < 101; j++) {
                for(int i = 0; i < 1000000; i++) {
                    volatile int x = i;  
                }
                pwm_servo_set_pos(j); 
            }   
    }

    //BAUD = T_q * bitrate

}
