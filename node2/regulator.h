

/** Position regulator.
 * ref: in encoder_steps/mS (0-4095)
 * *prev_pos: in encoder_step (0-4095). Is modified by function.
 */
void regulator_pos(int32_t ref, int32_t *prev_encoder_pos);
