#include "sam/sam3x/include/sam.h"

#include "timer.h"
#include "motor.h"
#include "encoder.h"

#define ENCODER_OUT_EN_PIN PIO_PD0
#define ENCODER_RST_PIN PIO_PD1
#define ENCODER_SEL_LOW_BYTE_PIN PIO_PD2

//Total number of steps. Only accessed by getter. 0 is invalid value
static uint16_t total_encoder_steps = 0;

uint16_t encoder_get_total_encoder_steps() {
    return total_encoder_steps;
}

void encoder_calibrate(void) {
    motor_set_speed(0, 1500);
    delay_us(1500000);
    encoder_reset();
    motor_set_speed(1, 1500);
    delay_us(2000000);
    total_encoder_steps = encoder_read();
    motor_set_speed(1, 0);
}

void encoder_init(void) {
    PIOD->PIO_WPMR &= ~PIO_WPMR_WPEN; //Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID14; //Enable clock for PIOD
    PIOD->PIO_PER |= ENCODER_OUT_EN_PIN | ENCODER_RST_PIN | ENCODER_SEL_LOW_BYTE_PIN; //enable pin
    PIOD->PIO_OER |= ENCODER_OUT_EN_PIN | ENCODER_RST_PIN | ENCODER_SEL_LOW_BYTE_PIN; //enable output
    PIOD->PIO_SODR = ENCODER_OUT_EN_PIN | ENCODER_RST_PIN; //RST and OE active low
    PIOD->PIO_CODR = ENCODER_SEL_LOW_BYTE_PIN; 
    
    //init mj2
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN;
    PMC->PMC_PCER0 |= PMC_PCER0_PID13; 
    PIOC->PIO_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
    //Input only on mj2 pins
    PIOC->PIO_ODR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;

}

void encoder_reset(void) {
    PIOD->PIO_CODR = ENCODER_RST_PIN;
    delay_us(20);
    PIOD->PIO_SODR = ENCODER_RST_PIN;
}

//Reads bits 1-8 in port C
static uint8_t mj2_read(void) {
    return (((PIOC->PIO_PDSR & (0xFF << 1)) >> 1));
}

int16_t encoder_read(void) {
    PIOD->PIO_CODR |= ENCODER_OUT_EN_PIN;
    PIOD->PIO_CODR |= ENCODER_SEL_LOW_BYTE_PIN;

    delay_us(30);

    int32_t high_byte = mj2_read();
    PIOD->PIO_SODR = ENCODER_SEL_LOW_BYTE_PIN;

    delay_us(30);

    int32_t low_byte = mj2_read();
    PIOD->PIO_SODR = ENCODER_OUT_EN_PIN;
    
    return ((high_byte << 8) | (low_byte));
}