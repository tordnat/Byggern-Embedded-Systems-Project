

/*
* current_pos: in encoder steps (0-4095)
* prev_pos: in encoder_step (0-4095)
* ref: in encoder_steps/S (0-4095)
*/
void regulator_pos(int32_t ref, int32_t *prev_encoder_pos);
void regulator_speed(int32_t current_pos, int32_t prev_pos, int32_t ref);
