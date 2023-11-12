#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "dac.h"
#include "timer.h"
#include "encoder.h"
#define MOTOR_EN_PIN PIO_PD9
#define MOTOR_DIR_PIN PIO_PD10

void motor_init(void) {
    PIOD->PIO_WPMR &= ~PIO_WPMR_WPEN; //Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID14; //Enable clock for PIOD

    PIOD->PIO_PER |= MOTOR_DIR_PIN; //Enable pin
    PIOD->PIO_OER |= MOTOR_DIR_PIN; //Enable output
    PIOD->PIO_SODR = MOTOR_DIR_PIN; //Set high
}

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

void motor_set_directional_speed(int32_t speed) {
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

