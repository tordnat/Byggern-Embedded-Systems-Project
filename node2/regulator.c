#include <stdint.h>
#include "regulator.h"
#include "motor.h"
#include "utils.h"

//Position regulator
#define K_P 6//70
#define K_I (float) 100000//4000

//Speed regulator
#define SPEED_K_P 15
#define SPEED_K_I 0

#define TICK_SIZE_S 0.0001

static int32_t sum_e_pos = 0; 
static int32_t e_sum = 0;

void regulator_pos(int32_t ref, int32_t *prev_encoder_pos) {
    int32_t max_steps_encoder = 1407; //get_encoder_calibration();
    int32_t encoder_val = encoder_read();
    int32_t current_pos = map(encoder_val, 0, max_steps_encoder, 0, 100);
    int32_t e = ref - current_pos;
    if((e < 0) && (e > -0)) {
        e = 0;
    }
    int32_t u = K_P*e + K_I*sum_e_pos;
    //printf("PosU %d PosE %d Ref %d Prev_pos %d\n\r", u, e, ref, *prev_encoder_pos);
    regulator_speed(encoder_val, *prev_encoder_pos, u);
    *prev_encoder_pos = encoder_val;
}

void regulator_speed(int32_t current_pos, int32_t prev_pos, int32_t ref) {
    int32_t speed = (current_pos-prev_pos)/0.1; //Will be very large. Needs 32bit
    int32_t e = ref - speed;

    int32_t u = e*SPEED_K_P + TICK_SIZE_S*e_sum*SPEED_K_I + 0;

    e_sum += e;
    //printf("ref e u speed: %d %d %d %d\n\r",ref, e, u, speed);
    if((u < 700) && (u > 0)) {
        if(u < 2) {
            u = 0;
        } else {
            u = u+700;
        }
    }
    if((u > -700) && (u < 0)) {
        if(u > -2) {
            u = 0;
        }  else {
            u = u-700;
        }
    }
    motor_set_mapped_speed(u);
}
