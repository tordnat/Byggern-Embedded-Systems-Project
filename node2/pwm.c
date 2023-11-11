#include "sam/sam3x/include/sam.h"
#include "pwm.h"

#define PWM_PERIOD 1641
#define PWM_SERVO_DUTY_MID 123

static const int32_t channel = 5;

void pwm_channel5_init(void) {
    PWM->PWM_WPCR &= ~PIO_WPMR_WPEN; // Disable write protection for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; // Enable clock for PWM module PWM PID == 36
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN; // Disable write protect for PIOC

    PIOC->PIO_ABSR |= PIO_ABSR_P19; // Periphiral select B for pin 19
    PIOC->PIO_PDR |= PIO_PDR_P19; // Disable GPIO functionality

    PWM->PWM_CLK |= (1 << PWM_CLK_DIVA_Pos); // Enable CLKA
    PWM->PWM_SCM |= PWM_SCM_UPDM_MODE0; // Sync mode
    PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL; // Use CLKA 
    PWM->PWM_CH_NUM[channel].PWM_CPRD = PWM_PERIOD; // Period = (20ms*84Mhz)/1024
    PWM->PWM_CH_NUM[channel].PWM_CDTY = PWM_SERVO_DUTY_MID; // 1.5 ms pulse width

    PWM->PWM_ENA |= PWM_ENA_CHID5; // Enable channel 5
}

void pwm_set_duty_channel5(uint32_t duty_cycle) {
    if(duty_cycle < PWM_PERIOD) {
        PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = duty_cycle;
        return;
    }
    else {
        printf("Duty Cycle higher than period\n\r");
        return; //add error code
    }
}

