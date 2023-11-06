#include "sam/sam3x/include/sam.h"
#include "pwm.h"

#define PWM_PERIOD 1641
#define PWM_SERVO_DUTY_MIN 82
#define PWM_SERVO_DUTY_MAX 164
#define PWM_SERVO_DUTY_MID 123

static void pwm_set_duty(uint32_t duty);

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
void pwm_servo_set_pos(int8_t pos, int8_t prev_pos) {
    pos = -pos; // To map joystick direction to intuitive servo direction
    //Invalid position
    if((pos < -100) || (pos > 100)) {
        printf("Pos outside valid range\n\r");
        return; //add error code
    }
    //Position to duty cycle calulation
    if(pos == prev_pos) {
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