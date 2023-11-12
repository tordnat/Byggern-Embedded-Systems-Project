#include "uart_and_printf/printf-stdarg.h"
#include "game.h"
#include "can_controller.h"
#include "solenoid.h"
#include "motor.h"
#include "servo.h"
#include "adc.h"
#include "timer.h"

#define ADC_STOP_THRESHOLD 900
static enum game_state_enum state = STOPPED;

static uint32_t time_ticks = 0;
static uint16_t score = 0;

void game_stop() {
    state = STOPPED;
    CAN_MESSAGE stop_msg;
    stop_msg.id = CAN_ID_GAME_STOP;
    stop_msg.data_length = 0;
    can_send(&stop_msg, CAN_TX_MAILBOX_ID);
    printf("Sent stop message\n\r");
    score = 0;
    time_ticks = 0;
}

static void game_send_score(uint32_t score) {
    CAN_MESSAGE score_msg;
    score_msg.id = CAN_ID_GAME_SCORE;
    score_msg.data_length = 2;
    score_msg.data[0] = (uint8_t) (score);
    score_msg.data[1] = (uint8_t) (score >> 8);
    can_send(&score_msg, CAN_TX_MAILBOX_ID);
}

static void game_update_score() {
    time_ticks += 1;
    if(time_ticks % 1000 == 0) { //Score is only 16 bit 
        score += 1;
        time_ticks = 0;
        game_send_score(score);
    }
}
void game_loop(int32_t servo_pos, uint8_t solenoid) {
    servo_set_pos(servo_pos);
    if (adc_read() < ADC_STOP_THRESHOLD) {
        delay_us(10000);
        if(adc_read() > ADC_STOP_THRESHOLD) return;

        motor_set_speed(1, 0);
        game_stop();
        return;
    }

    if (solenoid) {
        solenoid_on();
    } else {
        solenoid_off();
    }
    game_update_score();
    return;
}

void game_set_node1_state() {
    state = NODE1_PLAYING;
}

enum game_state_enum get_state() {
    return state;
}