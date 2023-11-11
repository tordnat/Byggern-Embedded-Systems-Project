#include <stdint.h>
void motor_init();
void motor_set_speed(uint8_t direction, uint16_t speed);
void encoder_init();
int16_t encoder_read();
void encoder_reset();
void motor_calibrate();
void motor_set_mapped_speed(int32_t speed);

