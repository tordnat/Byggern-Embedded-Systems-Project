

/*
* current_pos: in encoder steps (0-4095)
* prev_pos: in encoder_step (0-4095)
* ref: in encoder_steps/S (0-4095)
*/
void regulator_pos(int ref, int *prev_encoder_pos);
void regulator_speed(int current_pos, int prev_pos, int ref);
