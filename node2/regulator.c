#include <stdint.h>
#include "regulator.h"
#include "motor.h"
#include "utils.h"
#include "encoder.h"

//Position regulator
#define K_P 6//70
#define K_I 10000//4000

//Speed regulator
#define SPEED_K_P 15
#define SPEED_K_I 0

#define TICK_SIZE_S 0.1

static int32_t sum_e_pos = 0; 
static int32_t e_sum = 0;
static void regulator_speed(int32_t current_pos, int32_t prev_pos, int32_t ref);

void regulator_pos(int32_t ref, int32_t *prev_encoder_pos) {
    int16_t max_steps_encoder = encoder_get_total_encoder_steps();
    int32_t encoder_val = encoder_read();
    int32_t current_pos = map(encoder_val, 0, max_steps_encoder, 0, 100);
    int32_t e = ref - current_pos;
    if((e < 0) && (e > -0)) {
        e = 0;
    }
    int32_t u = K_P*e + K_I*sum_e_pos;
    regulator_speed(encoder_val, *prev_encoder_pos, u);
    *prev_encoder_pos = encoder_val;
}

static void regulator_speed(int32_t current_pos, int32_t prev_pos, int32_t ref) {
    int32_t speed = (current_pos-prev_pos)/TICK_SIZE_S; //Will be very large. Needs 32bit
    int32_t e = ref - speed;
    int32_t u = e*SPEED_K_P + TICK_SIZE_S*e_sum*SPEED_K_I + ref;
    e_sum += e;
    printf("u %d, ref %d, I-ledd\n\r", u, ref, TICK_SIZE_S*e_sum*SPEED_K_I);
    //To account for non-linear response when applying voltage (e.g voltage not relating to speed linearly)
    const uint16_t voltage_threshold = 700; 
    const uint8_t jitter_threshold = 2; //To prevent jumping between +- voltage thershold
    if((u < voltage_threshold) && (u > 0)) {
        if(u < jitter_threshold) {
            u = 0;
        } else {
            u = u+voltage_threshold;
        }
    }
    if((u > -voltage_threshold) && (u < 0)) {
        if(u > -jitter_threshold) {
            u = 0;
        }  else {
            u = u-voltage_threshold;
        }
    }
    motor_set_directional_speed(u);
}
