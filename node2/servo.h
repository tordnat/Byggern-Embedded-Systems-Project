#include <stdint.h>

void servo_init();

/**
 * Sets position of servo.
 * pos: 
 * prev_set_pos: Pointer to previous set position variable, will modify 
*/
void servo_set_pos(int8_t pos);