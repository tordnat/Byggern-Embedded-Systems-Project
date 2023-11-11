#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "dac.h"
#include "timer.h"
#define MOTOR_EN_PIN PIO_PD9
#define MOTOR_DIR_PIN PIO_PD10

//Active low PIN25
#define ENCODER_OUT_EN_PIN PIO_PD0
//Active low PIN26
#define ENCODER_RST_PIN PIO_PD1
//PIN27
#define ENCODER_SEL_LOW_BYTE_PIN PIO_PD2




void motor_init() {
    PIOD->PIO_WPMR &= ~PIO_WPMR_WPEN; //Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID14; //Enable clock for PIOD

    PIOD->PIO_PER |= MOTOR_DIR_PIN; //enable pin
    PIOD->PIO_OER |= MOTOR_DIR_PIN; //enable output
    PIOD->PIO_SODR = MOTOR_DIR_PIN;

    //PIOD->PIO_PER |= MOTOR_EN_PIN; //enable pin
    //PIOD->PIO_OER |= MOTOR_EN_PIN; //enable output
    //PIOD->PIO_SODR = MOTOR_EN_PIN; //Enable motor (disable break)
}
//1 left, 0 right 
void motor_set_speed(uint8_t direction, uint16_t speed) {
    if(direction == 0) {
        PIOD->PIO_SODR = MOTOR_DIR_PIN;

    } else if (direction == 1) {
        PIOD->PIO_CODR = MOTOR_DIR_PIN;
    }
    else {
        printf("Invalid direction in motor_set_speed\n\r");
        return;
    }
    dac_write(speed);
}
void motor_set_mapped_speed(int32_t speed) {
    int32_t abs_speed;
    if(speed > 0) {
        abs_speed = speed;
    } else {
        abs_speed = -speed;
    }
    if((abs_speed > 4000)) {
        abs_speed = 4000;
    }

    if(speed < 0) {
        motor_set_speed(0, abs_speed);
    }
    else if(speed > 0) {
        motor_set_speed(1, abs_speed);
    } else {
        motor_set_speed(0, 0);
    }
}

//0 to 100
void motor_set_pos() {

}

#define DIR PIO_PD10
#define EN PIO_PD9
#define SEL PIO_PD2
#define NOT_RST PIO_PD1
#define NOT_OE PIO_PD0
#define BUTTON PIO_PD6
#define DO_MASK (0xFF << 1)


void encoder_init() {
    PIOD->PIO_WPMR &= ~PIO_WPMR_WPEN; //Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID14; //Enable clock for PIOD
    PIOD->PIO_PER |= ENCODER_OUT_EN_PIN | ENCODER_RST_PIN | ENCODER_SEL_LOW_BYTE_PIN; //enable pin
    PIOD->PIO_OER |= ENCODER_OUT_EN_PIN | ENCODER_RST_PIN | ENCODER_SEL_LOW_BYTE_PIN; //enable output
    PIOD->PIO_SODR = ENCODER_OUT_EN_PIN | ENCODER_RST_PIN; //RST and OE active low

    PIOD->PIO_CODR = ENCODER_OUT_EN_PIN | ENCODER_SEL_LOW_BYTE_PIN;

    
    //init mj2
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN;
    PMC->PMC_PCER0 |= PMC_PCER0_PID13; 

    PIOC->PIO_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
    //Input only
    PIOC->PIO_ODR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;

}


//Reads bits 1-8 in port C
static uint8_t mj2_read() {
    return (((PIOC->PIO_PDSR & (0xFF << 1)) >> 1));
}
int16_t encoder_read() {
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

void encoder_reset() {
    PIOD->PIO_CODR = ENCODER_RST_PIN;
    delay_us(20);
    PIOD->PIO_SODR = ENCODER_RST_PIN;
}

void motor_calibrate() {
    motor_set_speed(0, 1500);
    delay_us(2000000);
    motor_set_speed(1, 0);
    encoder_reset();
}