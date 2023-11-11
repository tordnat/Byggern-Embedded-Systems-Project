#include "utilities.h"
#include "usart.h"
#include "pong_game.h"
#include "joystick.h"
#include "can.h"

static game_status_t pong;
static game_controller_msg_t game_controller;

void game_update(void){
	if(pong.game_is_playing){
		game_send_input();
	}	
}

void game_send_input(void){
	can_message_t* can_tx_buff_ptr = get_can_transmit_buffer_ptr();
	game_controller = get_game_controller_msg();
	can_encode_game_controller_msg(&game_controller, can_tx_buff_ptr);
	can_message_transmit(can_tx_buff_ptr);
}

uint8_t game_is_playing(void){
	return pong.game_is_playing;
}

void game_set_high_score(uint16_t high_score){
	pong.high_score = high_score;
}

void game_set_score(uint16_t score){
	pong.current_score = score;
}

uint16_t game_get_high_score(void){
	return pong.high_score;
}

uint16_t game_get_current_score(void){
	return pong.current_score;
}

void game_start_pong(void){
	printf("Starting game!\n\r");
	pong.game_is_playing = 1;
	can_send_game_start();
}

void game_stop_pong(void){
	printf("Stopping game!\n\r");
	pong.game_is_playing = 0;
	can_send_game_stop();
}