#include <stdint.h>
#include "regulator.h"
#include "motor.h"
#include "utils.h"
#include "encoder.h"

//Position regulator
#define K_P 6//70
#define K_I 0.15//4000

//Speed regulator
#define SPEED_K_P 15
#define SPEED_K_I 0

#define TICK_SIZE_S 0.05

static int32_t e_sum_pos = 0; 
static int32_t e_sum_speed = 0;
static void regulator_speed(int32_t current_pos, int32_t prev_pos, int32_t ref);

void regulator_pos(int32_t ref, int32_t *prev_encoder_pos) {
    int16_t max_steps_encoder = encoder_get_total_encoder_steps();
    int32_t encoder_val = encoder_read();
    int32_t current_pos = map(encoder_val, 0, max_steps_encoder, 0, 100);
    int32_t e = ref - current_pos;
    if((e < 0) && (e > -0)) {
        e = 0;
    }
    int32_t reg_I = K_I*e_sum_pos*TICK_SIZE_S;
    //Anti windup
    if(reg_I > 2000) { //2000 is somewhat arbitrary. Not at 4000 (max) because then P will not be able to counteract it 
        reg_I = 2000;
    }
    if(reg_I < -2000) {
        reg_I = -2000;
    }
    //Prevent reg_I from doing anything if error is big
    if(abs(e) > 20) {
        reg_I = 0;
    }
    int32_t u = K_P*e + reg_I;
    //printf("u %d, ref %d, I-ledd %d\n\r", u, ref, reg_I);
    regulator_speed(encoder_val, *prev_encoder_pos, u);
    *prev_encoder_pos = encoder_val;
    e_sum_pos += e;
}

static void regulator_speed(int32_t current_pos, int32_t prev_pos, int32_t ref) {
    int32_t speed = (current_pos-prev_pos)/TICK_SIZE_S; //Will be very large. Needs 32bit
    int32_t e = ref - speed;
    int32_t u = e*SPEED_K_P + TICK_SIZE_S*e_sum_speed*SPEED_K_I + ref;
    e_sum_speed += e;
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
