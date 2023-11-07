#include <stdint.h>
void motor_init();
void motor_set_speed(uint8_t direction, uint16_t speed);
void encoder_init();
uint16_t encoder_read();