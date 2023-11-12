#pragma once
#include "can.h"

typedef struct{
	uint16_t high_score;
	uint16_t current_score;
	uint8_t game_is_playing;
} game_status_t;

void game_update(void);
void game_send_start(void);
void game_start_pong(void);
void game_stop_pong(void);
void game_set_score(uint16_t score);
uint8_t game_get_is_playing(void);
uint16_t game_get_score(void);
uint16_t game_get_high_score(void);