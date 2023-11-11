#pragma once
#include "can.h"
#include "oled.h"

typedef struct{
	uint8_t game_is_playing;
	uint16_t high_score;
	uint16_t current_score;
} game_status_t;


void game_update(void);
void game_send_input(void);
uint8_t game_is_playing(void);
void game_set_high_score(uint16_t high_score);
void game_set_score(uint16_t score);
uint16_t game_get_high_score(void);
uint16_t game_get_current_score(void);
void game_start_pong(void);
void game_stop_pong(void);