#include <stdint.h>

void motor_init(void);

/**
 * Direction: 1 == right, 0 == left
 * Speed: 12bit resolution, 0 - 4095
*/
void motor_set_speed(uint8_t direction, uint16_t speed);

/** 
 * Sets speed of motor based on a signed integer
 * Speed: Must should be between -4000 - 4000. Values ouside will be capped
*/
void motor_set_directional_speed(int32_t speed);