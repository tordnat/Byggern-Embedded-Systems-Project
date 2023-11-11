#include "sam/sam3x/include/sam.h"

/**
 * Gets total number of encoder steps in the box
 * 0 is not valid, only valid values after encoder_calibrate has been called
*/
uint16_t encoder_get_total_encoder_steps();


void encoder_init(void);

/**
 * Sets encoder to 0, e.g set a reference point
*/
void encoder_reset(void);

/**
 * Returns raw value from encoder
*/
int16_t encoder_read(void);

/**
 * Calibrates encoder by moving motor to the left then to right and counting max steps
 * Sets total_encoder_steps variable
 * Can only be called after motor_init and encoder_init
*/
void encoder_calibrate(void);