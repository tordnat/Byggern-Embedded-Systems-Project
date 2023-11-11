#include <stdint.h>
#include "uart_and_printf/printf-stdarg.h"
#include <stdlib.h>
#include <pwm.h>
#include "servo.h"
#include "utils.h"

#define PWM_SERVO_DUTY_MIN 82
#define PWM_SERVO_DUTY_MAX 164

static int8_t prev_pos = 0;

void servo_init(void) {
    pwm_channel5_init();
}

void servo_set_pos(int8_t pos) {
    //Invalid position
    if(pos > 100) {
        printf("Pos outside valid range %d\n\r", pos);
        return;
    }
    //To prevent twitching from inaccurate slider values
    int32_t diff = pos - prev_pos;
    diff = abs(diff);
    if(diff < 3) {
        return;
    }
    prev_pos = pos;
    int32_t duty = map(pos, 100, 0, PWM_SERVO_DUTY_MIN, PWM_SERVO_DUTY_MAX);

    //Check if servo is outside 1ms and 2ms pulse width
    if((duty <= PWM_SERVO_DUTY_MAX) && (duty >= PWM_SERVO_DUTY_MIN)) {
        pwm_set_duty_channel5(duty);
    }
    else {
        printf("Calc error in servo_set_pos\n\r");
    }
}