#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "dac.h"

#define MOTOR_EN_PIN PIO_PD9
#define MOTOR_DIR_PIN PIO_PD10

//Active low PIN25
#define ENCODER_OUT_EN_PIN PIO_PD0
//Active low PIN26
#define ENCODER_RST_PIN PIO_PD1
//PIN27
#define ENCODER_SEL_LOW_BYTE_PIN PIO_PD2

#define BRAKE_PIN 


//1 left, 0 right 
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

void motor_set_speed(uint8_t direction, uint16_t speed) {
    if(direction == 0) {
        PIOD->PIO_SODR = MOTOR_DIR_PIN;

    } else if (direction == 1) {
        PIOD->PIO_CODR = MOTOR_DIR_PIN;
    }
    else {
        //Add error print/code here
        printf("err");
        return;
    }
    dac_write(speed);
}
//0 to 100
void motor_set_pos() {

}

void encoder_init() {
    PIOD->PIO_WPMR &= ~PIO_WPMR_WPEN; //Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID14; //Enable clock for PIOD
    //init mj2
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN;
    PMC->PMC_PCER0 |= PMC_PCER0_PID13; 
    PIOD->PIO_PER |= ENCODER_OUT_EN_PIN | ENCODER_RST_PIN | ENCODER_SEL_LOW_BYTE_PIN; //enable pin
    PIOD->PIO_OER |= ENCODER_OUT_EN_PIN | ENCODER_RST_PIN | ENCODER_SEL_LOW_BYTE_PIN; //enable output
    PIOD->PIO_SODR = ENCODER_OUT_EN_PIN | ENCODER_RST_PIN; //RST and OE active low

    PIOD->PIO_CODR = ENCODER_OUT_EN_PIN | ENCODER_SEL_LOW_BYTE_PIN;

    

    PIOC->PIO_PER |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;
    //Input only
    PIOC->PIO_PDR |= PIO_PC1 | PIO_PC2 | PIO_PC3 | PIO_PC4 | PIO_PC5 | PIO_PC6 | PIO_PC7 | PIO_PC8;


}
//Reads bits 1-8 in port C
static uint8_t mj2_read() {
    /*
    uint32_t bit0 = (PIOC->PIO_PDSR & PIO_PC1);
    uint32_t bit1 = (PIOC->PIO_PDSR & PIO_PC2);
    uint32_t bit2 = (PIOC->PIO_PDSR & PIO_PC3);
    uint32_t bit3 = (PIOC->PIO_PDSR & PIO_PC4);
    uint32_t bit4 = (PIOC->PIO_PDSR & PIO_PC5);
    uint32_t bit5 = (PIOC->PIO_PDSR & PIO_PC6);
    uint32_t bit6 = (PIOC->PIO_PDSR & PIO_PC7);
    uint32_t bit7 = (PIOC->PIO_PDSR & PIO_PC8);


    uint8_t byte = bit0 | (bit1 << 1) | (bit2 << 2) | (bit3 << 3) | (bit4 << 4) | (bit5 << 5) | (bit6 << 6) | (bit7 << 7);
    return byte;
    */
    return ((PIOC->PIO_PDSR >> 1) & 0xFF);
}
uint16_t encoder_read() {
    PIOD->PIO_CODR |= ENCODER_OUT_EN_PIN;
    PIOD->PIO_CODR |= ENCODER_SEL_LOW_BYTE_PIN;

    delay(20);

    uint8_t high_byte = mj2_read();
    PIOD->PIO_SODR = ENCODER_SEL_LOW_BYTE_PIN;

    delay(20);

    uint8_t low_byte = mj2_read();
    PIOD->PIO_SODR = ENCODER_OUT_EN_PIN;
    
    return ((high_byte << 8) | (low_byte));
}

