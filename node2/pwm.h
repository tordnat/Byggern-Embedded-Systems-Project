
//Init PWM. Inits to position 0
void pwm_init(void);

//Takes in position between -100 and 100
void pwm_servo_set_pos(int8_t pos, int8_t prev_pos);